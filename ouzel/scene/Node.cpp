// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "Node.h"
#include "core/Engine.h"
#include "SceneManager.h"
#include "Layer.h"
#include "animators/Animator.h"
#include "Camera.h"
#include "utils/Utils.h"
#include "math/MathUtils.h"
#include "Component.h"

namespace ouzel
{
    namespace scene
    {
        Node::Node()
        {
            animationUpdateCallback.callback = std::bind(&Node::updateAnimation, this, std::placeholders::_1);
        }

        Node::~Node()
        {
            if (parent) parent->removeChild(*this);
            if (currentAnimator) currentAnimator->remove();

            for (Component* component : components)
            {
                component->setNode(nullptr);
            }
        }

        void Node::visit(const Matrix4& newTransformMatrix,
                         bool parentTransformDirty,
                         Camera* camera,
                         Layer* currentLayer,
                         float depth)
        {
            if (parentTransformDirty)
            {
                updateTransform(newTransformMatrix);
            }

            if (transformDirty)
            {
                calculateTransform();
            }

            AABB2 boundingBox = getBoundingBox();

            if (cullDisabled || (!boundingBox.isEmpty() && camera->checkVisibility(getTransform(), boundingBox)))
            {
                currentLayer->addToDrawQueue(*this, depth + z);
            }

            for (Node* child : children)
            {
                if (!child->isHidden())
                {
                    child->visit(transform, updateChildrenTransform, camera, currentLayer, depth + z);
                }
            }

            updateChildrenTransform = false;
        }

        void Node::draw(Camera* camera)
        {
            if (transformDirty)
            {
                calculateTransform();
            }

            graphics::Color drawColor(color.r, color.g, color.b, static_cast<uint8_t>(color.a * opacity));

            for (Component* component : components)
            {
                if (!component->isHidden())
                {
                    component->draw(transform, drawColor, camera);
                }
            }
        }

        void Node::drawWireframe(Camera* camera)
        {
            if (transformDirty)
            {
                calculateTransform();
            }

            graphics::Color drawColor(color.r, color.g, color.b, 255);

            for (Component* component : components)
            {
                if (!component->isHidden())
                {
                    component->drawWireframe(transform, drawColor, camera);
                }
            }
        }

        bool Node::addChild(Node& node)
        {
            if (NodeContainer::addChild(node))
            {
                node.updateTransform(getTransform());

                return true;
            }
            else
            {
                return false;
            }
        }

        void Node::setZ(float newZ)
        {
            z = newZ;

            // Currently z does not affect transformation
            //localTransformDirty = transformDirty = inverseTransformDirty = true;
        }

        void Node::setPosition(const Vector2& newPosition)
        {
            position = newPosition;

            localTransformDirty = transformDirty = inverseTransformDirty = true;
        }

        void Node::setRotation(float newRotation)
        {
            rotation = newRotation;

            localTransformDirty = transformDirty = inverseTransformDirty = true;
        }

        void Node::setScale(const Vector2& newScale)
        {
            scale = newScale;

            localTransformDirty = transformDirty = inverseTransformDirty = true;
        }

        void Node::setColor(const graphics::Color& newColor)
        {
            color = newColor;
        }

        void Node::setOpacity(float newOpacity)
        {
            opacity = clamp(newOpacity, 0.0f, 1.0f);
        }

        void Node::setFlipX(bool newFlipX)
        {
            flipX = newFlipX;

            localTransformDirty = transformDirty = inverseTransformDirty = true;
        }

        void Node::setFlipY(bool newFlipY)
        {
            flipY = newFlipY;

            localTransformDirty = transformDirty = inverseTransformDirty = true;
        }

        void Node::setHidden(bool newHidden)
        {
            hidden = newHidden;
        }

        bool Node::pointOn(const Vector2& worldPosition) const
        {
            Vector2 localPosition = convertWorldToLocal(worldPosition);

            for (Component* component : components)
            {
                if (component->pointOn(localPosition))
                {
                    return true;
                }
            }

            return false;
        }

        bool Node::shapeOverlaps(const std::vector<Vector2>& edges) const
        {
            Matrix4 inverse = getInverseTransform();

            std::vector<Vector2> transformedEdges;

            for (const Vector2& edge : edges)
            {
                Vector3 transformedEdge = edge;

                inverse.transformPoint(transformedEdge);

                transformedEdges.push_back(Vector2(transformedEdge.x, transformedEdge.y));
            }

            for (Component* component : components)
            {
                if (component->shapeOverlaps(transformedEdges))
                {
                    return true;
                }
            }

            return false;
        }

        void Node::updateTransform(const Matrix4& newParentTransform)
        {
            parentTransform = newParentTransform;
            transformDirty = inverseTransformDirty = true;
        }

        Vector2 Node::convertWorldToLocal(const Vector2& worldPosition) const
        {
            Vector3 localPosition = worldPosition;

            const Matrix4& currentInverseTransform = getInverseTransform();
            currentInverseTransform.transformPoint(localPosition);

            return Vector2(localPosition.x, localPosition.y);
        }

        Vector2 Node::convertLocalToWorld(const Vector2& localPosition) const
        {
            Vector3 worldPosition = localPosition;

            const Matrix4& currentTransform = getTransform();
            currentTransform.transformPoint(worldPosition);

            return Vector2(worldPosition.x, worldPosition.y);
        }

        void Node::animate(const AnimatorPtr& animator)
        {
            removeAnimation();
            currentAnimator = animator;

            if (currentAnimator)
            {
                currentAnimator->start(this);
                sharedEngine->scheduleUpdate(animationUpdateCallback);
            }
        }

        void Node::removeAnimation()
        {
            if (currentAnimator)
            {
                currentAnimator->stop();
                currentAnimator->remove();
                currentAnimator.reset();
                sharedEngine->unscheduleUpdate(animationUpdateCallback);
            }
        }

        void Node::calculateLocalTransform() const
        {
            localTransform = Matrix4::IDENTITY;
            localTransform.translate(Vector3(position.x, position.y, 0.0f));
            if (rotation != 0.0f) localTransform.rotateZ(-rotation);

            Vector3 realScale = Vector3(scale.x * (flipX ? -1.0f : 1.0f),
                                        scale.y * (flipY ? -1.0f : 1.0f),
                                        1.0f);

            localTransform.scale(realScale);

            localTransformDirty = false;
        }

        void Node::calculateTransform() const
        {
            if (localTransformDirty)
            {
                calculateLocalTransform();
            }

            transform = parentTransform * localTransform;
            transformDirty = false;

            updateChildrenTransform = true;
        }

        void Node::calculateInverseTransform() const
        {
            if (transformDirty)
            {
                calculateTransform();
            }

            inverseTransform = transform;
            inverseTransform.invert();
            inverseTransformDirty = false;
        }

        bool Node::addComponent(Component& component)
        {
            if (component.isAddedToNode())
            {
                return false;
            }

            components.push_back(&component);
            component.setNode(this);

            return true;
        }

        bool Node::removeComponent(uint32_t index)
        {
            if (index >= components.size())
            {
                return false;
            }

            Component* component = components[index];
            component->setNode(nullptr);

            components.erase(components.begin() + static_cast<int>(index));

            return true;
        }

        bool Node::removeComponent(Component& component)
        {
            for (auto i = components.begin(); i != components.end();)
            {
                if (*i == &component)
                {
                    component.setNode(nullptr);
                    components.erase(i);
                    return true;
                }
                else
                {
                    ++i;
                }
            }

            return false;
        }

        void Node::removeAllComponents()
        {
            components.clear();
        }

        void Node::updateAnimation(float delta)
        {
            if (currentAnimator)
            {
                currentAnimator->update(delta);

                if (currentAnimator->isDone())
                {
                    removeAnimation();
                }
            }
            else
            {
                sharedEngine->unscheduleUpdate(animationUpdateCallback);
            }
        }

        AABB2 Node::getBoundingBox() const
        {
            AABB2 boundingBox;

            for (Component* component : components)
            {
                if (!component->isHidden())
                {
                    boundingBox.merge(component->getBoundingBox());
                }
            }

            return boundingBox;
        }
    } // namespace scene
} // namespace ouzel
