// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <cassert>
#include <algorithm>
#include "Camera.h"
#include "Layer.h"
#include "core/Engine.h"
#include "graphics/Renderer.h"
#include "Layer.h"
#include "graphics/Texture.h"
#include "graphics/RenderTarget.h"
#include "math/Matrix4.h"
#include "utils/Utils.h"

namespace ouzel
{
    namespace scene
    {
        Camera::Camera()
        {
        }

        Camera::~Camera()
        {
            if (layer) layer->removeCamera(*this);
        }

        void Camera::recalculateProjection()
        {
            Size2 renderTargetSize = renderTarget ?
                renderTarget->getTexture()->getSize() :
                sharedEngine->getRenderer()->getSize();

            renderViewport.x = renderTargetSize.width * viewport.x;
            renderViewport.y = renderTargetSize.height * viewport.y;
            renderViewport.width = renderTargetSize.width * viewport.width;
            renderViewport.height = renderTargetSize.height * viewport.height;

            assert(renderViewport.width > 0.0f && renderViewport.height > 0.0f);

            if (targetContentSize.width > 0.0f && targetContentSize.height > 0.0f)
            {
                contentScale.x = renderViewport.width / targetContentSize.width;
                contentScale.y = renderViewport.height / targetContentSize.height;

                switch (scaleMode)
                {
                    case ScaleMode::NONE:
                    {
                        break;
                    }
                    case ScaleMode::EXACT_FIT:
                    {
                        contentScale.x = 1.0f;
                        contentScale.y = 1.0f;
                        break;
                    }
                    case ScaleMode::NO_BORDER:
                    {
                        contentScale.x = contentScale.y = std::max(contentScale.x, contentScale.y);
                        break;
                    }
                    case ScaleMode::SHOW_ALL:
                    {
                        contentScale.x = contentScale.y = std::min(contentScale.x, contentScale.y);
                        break;
                    }
                }

                contentSize = Size2(renderViewport.width / contentScale.x, renderViewport.height / contentScale.y);
                contentPosition = Vector2((contentSize.width - targetContentSize.width) / 2.0f,
                                          (contentSize.height - targetContentSize.height) / 2.0f);
            }
            else
            {
                contentSize = Size2(renderViewport.width, renderViewport.height);
                contentScale = Vector2(1.0f, 1.0f);
            }

            Matrix4::createOrthographic(contentSize.width, contentSize.height, -1.0f, 1.0f, projection);

            inverseProjection = projection;
            inverseProjection.invert();

            viewProjectionDirty = true;
        }

        const Matrix4& Camera::getViewProjection() const
        {
            if (viewProjectionDirty || transformDirty)
            {
                viewProjection = projection * getTransform();
                viewProjectionDirty = false;
            }

            return viewProjection;
        }

        void Camera::calculateLocalTransform() const
        {
            Matrix4 translationMatrix = Matrix4::IDENTITY;
            translationMatrix.translate(-position.x, -position.y, 0.0f);

            Matrix4 rotationMatrix = Matrix4::IDENTITY;
            rotationMatrix.rotate(Vector3(0.0f, 0.0f, -1.0f), -rotation);

            Matrix4 scaleMatrix = Matrix4::IDENTITY;
            Vector3 inverseScale(0.0f, 0.0f, 1.0f);
            if (scale.x != 0.0f) inverseScale.x = 1.0f / scale.x;
            if (scale.y != 0.0f) inverseScale.y = 1.0f / scale.y;
            scaleMatrix.scale(inverseScale);

            localTransform = scaleMatrix * rotationMatrix * translationMatrix;

            localTransformDirty = false;
        }

        Vector2 Camera::convertNormalizedToWorld(const Vector2& position) const
        {
            Matrix4 inverseViewMatrix = getViewProjection();
            inverseViewMatrix.invert();

            // convert window normalized to viewport clip position
            Vector3 result = Vector3(((position.x - viewport.x) / viewport.width - 0.5f) * 2.0f,
                                     ((position.y - viewport.y) / viewport.height - 0.5f) * 2.0f,
                                     0.0f);

            inverseViewMatrix.transformPoint(result);

            return Vector2(result.x, result.y);
        }

        Vector2 Camera::convertWorldToNormalized(const Vector2& position) const
        {
            Vector3 result = Vector3(position.x, position.y, 0.0f);
            getViewProjection().transformPoint(result);

            // convert viewport clip position to window normalized
            return Vector2((result.x / 2.0f + 0.5f) * viewport.width + viewport.x,
                           (result.y / 2.0f + 0.5f) * viewport.height + viewport.y);
        }

        bool Camera::checkVisibility(const Matrix4& boxTransform, const AABB2& boundingBox) const
        {
            // calculate center point of the bounding box
            Vector2 diff = boundingBox.max - boundingBox.min;

            // offset the center point, so that it is relative to 0,0
            Vector3 v3p(boundingBox.min.x + diff.x / 2.0f, boundingBox.min.y + diff.y / 2.0f, 0.0f);

            // apply local transform to the center point
            boxTransform.transformPoint(v3p);

            // tranform the center to viewport's clip space
            Vector4 clipPos;
            getViewProjection().transformVector(Vector4(v3p.x, v3p.y, v3p.z, 1.0f), clipPos);

            assert(clipPos.w != 0.0f);

            // normalize position of the center point
            Vector2 v2p((clipPos.x / clipPos.w + 1.0f) * 0.5f,
                        (clipPos.y / clipPos.w + 1.0f) * 0.5f);

            // calculate half size
            Size2 halfSize(diff.x / 2.0f, diff.y / 2.0f);

            // convert content size to world coordinates
            Size2 halfWorldSize;

            halfWorldSize.width = std::max(fabsf(halfSize.width * boxTransform.m[0] + halfSize.height * boxTransform.m[4]),
                                           fabsf(halfSize.width * boxTransform.m[0] - halfSize.height * boxTransform.m[4]));
            halfWorldSize.height = std::max(fabsf(halfSize.width * boxTransform.m[1] + halfSize.height * boxTransform.m[5]),
                                            fabsf(halfSize.width * boxTransform.m[1] - halfSize.height * boxTransform.m[5]));

            // scale half size by camera projection to get the size in clip space coordinates
            halfWorldSize.width *= (fabsf(viewProjection.m[0]) + fabsf(viewProjection.m[4])) / 2.0f;
            halfWorldSize.height *= (fabsf(viewProjection.m[1]) + fabsf(viewProjection.m[5])) / 2.0f;

            // create visible rect in clip space
            Rectangle visibleRect(-halfWorldSize.width,
                                  -halfWorldSize.height,
                                  1.0f + halfWorldSize.width * 2.0f,
                                  1.0f + halfWorldSize.height * 2.0f);

            return visibleRect.containsPoint(v2p);
        }

        void Camera::setViewport(const Rectangle& newViewport)
        {
            viewport = newViewport;
            recalculateProjection();
        }

        void Camera::setScaleMode(ScaleMode newScaleMode)
        {
            scaleMode = newScaleMode;
            recalculateProjection();
        }

        void Camera::setTargetContentSize(const Size2& newTargetContentSize)
        {
            targetContentSize = newTargetContentSize;
            recalculateProjection();
        }

        void Camera::addToLayer(Layer* newLayer)
        {
            layer = newLayer;
        }

        void Camera::removeFromLayer()
        {
            layer = nullptr;
        }

        void Camera::setRenderTarget(const graphics::RenderTargetPtr& newRenderTarget)
        {
            renderTarget = newRenderTarget;

            recalculateProjection();
        }
    } // namespace scene
} // namespace ouzel
