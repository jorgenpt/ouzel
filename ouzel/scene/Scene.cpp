// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "Scene.h"
#include "Layer.h"
#include "Camera.h"
#include "core/Engine.h"
#include "events/EventDispatcher.h"

namespace ouzel
{
    namespace scene
    {
        Scene::Scene():
            eventHandler(EventHandler::PRIORITY_MAX + 1)
        {
            eventHandler.windowHandler = std::bind(&Scene::handleWindow, this, std::placeholders::_1, std::placeholders::_2);
            eventHandler.mouseHandler = std::bind(&Scene::handleMouse, this, std::placeholders::_1, std::placeholders::_2);
            eventHandler.touchHandler = std::bind(&Scene::handleTouch, this, std::placeholders::_1, std::placeholders::_2);
        }

        Scene::~Scene()
        {
            if (entered)
            {
                for (Layer* layer : layers)
                {
                    layer->leave();
                }
            }
        }

        void Scene::draw()
        {
            layers.sort([](Layer* a, Layer* b) {
                return a->getOrder() > b->getOrder();
            });

            for (Layer* layer : layers)
            {
                layer->draw();
            }
        }

        void Scene::addLayer(Layer& layer)
        {
            if (!hasLayer(layer))
            {
                layers.push_back(&layer);

                if (entered) layer.enter();
            }
        }

        void Scene::removeLayer(Layer& layer)
        {
            std::list<Layer*>::iterator i = std::find(layers.begin(), layers.end(), &layer);

            if (i != layers.end())
            {
                if (entered)
                {
                    layer.leave();
                }

                layers.erase(i);
            }
        }

        void Scene::removeAllLayers()
        {
            if (entered)
            {
                for (Layer* layer : layers)
                {
                    layer->leave();
                }
            }

            layers.clear();
        }

        bool Scene::hasLayer(Layer& layer) const
        {
            std::list<Layer*>::const_iterator i = std::find(layers.begin(), layers.end(), &layer);

            return i != layers.end();
        }

        void Scene::recalculateProjection()
        {
            for (Layer* layer : layers)
            {
                layer->recalculateProjection();
            }
        }

        Node* Scene::pickNode(const Vector2& position) const
        {
            for (std::list<Layer*>::const_reverse_iterator i = layers.rbegin(); i != layers.rend(); ++i)
            {
                Layer* layer = *i;

                if (Node* result = layer->pickNode(position))
                {
                    return result;
                }
            }

            return nullptr;
        }

        std::set<Node*> Scene::pickNodes(const std::vector<Vector2>& edges) const
        {
            std::set<Node*> result;

            for (std::list<Layer*>::const_reverse_iterator i = layers.rbegin(); i != layers.rend(); ++i)
            {
                std::set<Node*> nodes = (*i)->pickNodes(edges);

                result.insert(nodes.begin(), nodes.end());
            }

            return result;
        }

        void Scene::enter()
        {
            entered = true;

            recalculateProjection();
            sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);

            for (Layer* layer : layers)
            {
                layer->enter();
            }
        }

        void Scene::leave()
        {
            entered = false;

            sharedEngine->getEventDispatcher()->removeEventHandler(eventHandler);

            for (Layer* layer : layers)
            {
                layer->leave();
            }
        }

        bool Scene::handleWindow(Event::Type type, const WindowEvent&)
        {
            if (type == Event::Type::WINDOW_RESOLUTION_CHANGE)
            {
                recalculateProjection();
            }

            return true;
        }

        bool Scene::handleMouse(Event::Type type, const MouseEvent& event)
        {
            switch (type)
            {
                case Event::Type::MOUSE_DOWN:
                {
                    scene::Node* node = pickNode(event.position);
                    pointerDownOnNode(0, node, event.position);
                    break;
                }
                case Event::Type::MOUSE_UP:
                {
                    scene::Node* node = pickNode(event.position);
                    pointerUpOnNode(0, node, event.position);
                    break;
                }
                case Event::Type::MOUSE_MOVE:
                {
                    scene::Node* node = pickNode(event.position);
                    pointerEnterNode(0, node, event.position);

                    if (scene::Node* pointerDownOnNode = getPointerDownOnNode(0))
                    {
                        pointerDragNode(0, pointerDownOnNode, event.position);
                    }
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        bool Scene::handleTouch(Event::Type type, const TouchEvent& event)
        {
            switch (type)
            {
                case Event::Type::TOUCH_BEGIN:
                {
                    scene::Node* node = pickNode(event.position);
                    pointerDownOnNode(event.touchId, node, event.position);
                    break;
                }
                case Event::Type::TOUCH_END:
                {
                    scene::Node* node = pickNode(event.position);
                    pointerUpOnNode(event.touchId, node, event.position);
                    break;
                }
                case Event::Type::TOUCH_MOVE:
                {
                    scene::Node* node = pickNode(event.position);
                    pointerEnterNode(0, node, event.position);

                    if (scene::Node* pointerDownOnNode = getPointerDownOnNode(event.touchId))
                    {
                        pointerDragNode(event.touchId, pointerDownOnNode, event.position);
                    }
                    break;
                }
                case Event::Type::TOUCH_CANCEL:
                {
                    scene::Node* node = pickNode(event.position);
                    pointerUpOnNode(event.touchId, node, event.position);
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        scene::Node* Scene::getPointerOnNode(uint64_t pointerId) const
        {
            scene::Node* result = nullptr;

            auto i = pointerOnNodes.find(pointerId);

            if (i != pointerOnNodes.end())
            {
                result = i->second;
            }

            return result;
        }

        scene::Node* Scene::getPointerDownOnNode(uint64_t pointerId) const
        {
            scene::Node* result = nullptr;

            auto i = pointerDownOnNodes.find(pointerId);

            if (i != pointerDownOnNodes.end())
            {
                result = i->second;
            }

            return result;
        }

        void Scene::pointerEnterNode(uint64_t pointerId, scene::Node* node, const Vector2& position)
        {
            scene::Node* pointerOnNode = getPointerOnNode(pointerId);

            if (pointerOnNode)
            {
                if (pointerOnNode == node)
                {
                    return;
                }
                else
                {
                    pointerLeaveNode(pointerId, pointerOnNode, position);
                }
            }

            pointerOnNodes[pointerId] = node;

            if (node && node->isReceivingInput())
            {
                Event event;
                event.type = Event::Type::UI_ENTER_NODE;

                event.uiEvent.node = node;
                event.uiEvent.position = node->convertWorldToLocal(position);

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }

        void Scene::pointerLeaveNode(uint64_t pointerId, scene::Node* node, const Vector2& position)
        {
            if (node && node->isReceivingInput())
            {
                Event event;
                event.type = Event::Type::UI_LEAVE_NODE;

                event.uiEvent.node = node;
                event.uiEvent.position = node->convertWorldToLocal(position);

                sharedEngine->getEventDispatcher()->postEvent(event);
            }

            pointerOnNodes.erase(pointerId);
        }

        void Scene::pointerDownOnNode(uint64_t pointerId, scene::Node* node, const Vector2& position)
        {
            pointerDownOnNodes[pointerId] = node;

            if (node && node->isReceivingInput())
            {
                Event event;
                event.type = Event::Type::UI_PRESS_NODE;

                event.uiEvent.node = node;
                event.uiEvent.position = node->convertWorldToLocal(position);

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }

        void Scene::pointerUpOnNode(uint64_t pointerId, scene::Node* node, const Vector2& position)
        {
            scene::Node* pointerDownOnNode = getPointerDownOnNode(pointerId);

            if (pointerDownOnNode && pointerDownOnNode->isReceivingInput())
            {
                Event releaseEvent;
                releaseEvent.type = Event::Type::UI_RELEASE_NODE;

                releaseEvent.uiEvent.node = pointerDownOnNode;
                releaseEvent.uiEvent.position = pointerDownOnNode->convertWorldToLocal(position);

                sharedEngine->getEventDispatcher()->postEvent(releaseEvent);

                if (pointerDownOnNode == node)
                {
                    Event clickEvent;
                    clickEvent.type = Event::Type::UI_CLICK_NODE;

                    clickEvent.uiEvent.node = pointerDownOnNode;
                    clickEvent.uiEvent.position = pointerDownOnNode->convertWorldToLocal(position);

                    sharedEngine->getEventDispatcher()->postEvent(clickEvent);
                }
            }

            pointerDownOnNodes.erase(pointerId);
        }

        void Scene::pointerDragNode(uint64_t, scene::Node* node, const Vector2& position)
        {
            if (node && node->isReceivingInput())
            {
                Event event;
                event.type = Event::Type::UI_DRAG_NODE;

                event.uiEvent.node = node;
                event.uiEvent.position = node->convertWorldToLocal(position);

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }
    } // namespace scene
} // namespace ouzel
