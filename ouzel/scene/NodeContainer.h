// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <list>
#include <memory>
#include "utils/Types.h"
#include "utils/Noncopyable.h"

namespace ouzel
{
    namespace scene
    {
        class Layer;
        class Node;

        class NodeContainer: public Noncopyable
        {
        public:
            NodeContainer();
            virtual ~NodeContainer();

            virtual bool addChild(Node& node);
            virtual bool removeChild(Node& node);
            virtual void removeAllChildren();
            virtual bool hasChild(Node& node, bool recursive = false) const;
            virtual const std::list<Node*>& getChildren() const { return children; }

        protected:
            virtual void enter();
            virtual void leave();

            std::list<Node*> children;
            bool entered = false;
        };
    } // namespace scene
} // namespace ouzel
