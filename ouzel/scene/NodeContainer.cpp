// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "NodeContainer.h"
#include "Node.h"

namespace ouzel
{
    namespace scene
    {
        NodeContainer::NodeContainer()
        {
        }

        NodeContainer::~NodeContainer()
        {
            for (Node* node : children)
            {
                if (entered) node->leave();
                node->setParent(nullptr);
            }
        }

        bool NodeContainer::addChild(Node& node)
        {
            if (!hasChild(node) && !node.hasParent())
            {
                node.setParent(this);
                if (entered) node.enter();
                children.push_back(&node);

                return true;
            }
            else
            {
                return false;
            }
        }

        bool NodeContainer::removeChild(Node& node)
        {
            std::vector<Node*>::iterator i = std::find(children.begin(), children.end(), &node);

            if (i != children.end())
            {
                if (entered) node.leave();
                node.setParent(nullptr);
                children.erase(i);

                return true;
            }
            else
            {
                return false;
            }
        }

        bool NodeContainer::eraseChild(Node& node)
        {
            std::vector<Node*>::iterator i = std::find(children.begin(), children.end(), &node);

            if (i != children.end())
            {
                children.erase(i);

                return true;
            }
            else
            {
                return false;
            }
        }

        void NodeContainer::removeAllChildren()
        {
            auto childrenCopy = children;

            for (auto& node : childrenCopy)
            {
                if (entered) node->leave();
                node->setParent(nullptr);
            }

            children.clear();
        }

        bool NodeContainer::hasChild(Node& node, bool recursive) const
        {
            for (std::vector<Node*>::const_iterator i = children.begin(); i != children.end(); ++i)
            {
                Node* child = *i;

                if (child == &node || (recursive && child->hasChild(node, true)))
                {
                    return true;
                }
            }

            return false;
        }

        void NodeContainer::enter()
        {
            entered = true;

            for (Node* node : children)
            {
                node->enter();
            }
        }

        void NodeContainer::leave()
        {
            entered = false;

            for (Node* node : children)
            {
                node->leave();
            }
        }

        void NodeContainer::findNodes(const Vector2& position, std::vector<Node*>& nodes) const
        {
            for (auto i = children.rbegin(); i != children.rend(); ++i)
            {
                Node* node = *i;

                if (!node->isHidden())
                {
                    if (node->isPickable() && node->pointOn(position))
                    {
                        auto upperBound = std::upper_bound(nodes.begin(), nodes.end(), node,
                                                           [](const Node* a, Node* b) {
                                                               return a->getWorldZ() < b->getWorldZ();
                                                           });

                        nodes.insert(upperBound, node);
                    }

                    node->findNodes(position, nodes);
                }
            }
        }

        void NodeContainer::findNodes(const std::vector<Vector2>& edges, std::vector<Node*>& nodes) const
        {
            for (auto i = children.rbegin(); i != children.rend(); ++i)
            {
                Node* node = *i;

                if (!node->isHidden())
                {
                    if (node->isPickable() && node->shapeOverlaps(edges))
                    {
                        auto upperBound = std::upper_bound(nodes.begin(), nodes.end(), node,
                                                           [](const Node* a, Node* b) {
                                                               return a->getWorldZ() < b->getWorldZ();
                                                           });

                        nodes.insert(upperBound, node);
                    }

                    node->findNodes(edges, nodes);
                }
            }
        }
    } // namespace scene
} // namespace ouzel
