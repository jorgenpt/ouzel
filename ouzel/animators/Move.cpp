// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "Move.h"
#include "scene/Node.h"

namespace ouzel
{
    namespace scene
    {
        Move::Move(float pLength, const Vector2& pPosition, bool pRelative):
            Animator(pLength), position(pPosition), relative(pRelative)
        {
        }

        void Move::start(Node* targetNode)
        {
            Animator::start(targetNode);

            if (targetNode)
            {
                startPosition = targetNode->getPosition();
                targetPosition = relative ? startPosition + position : position;

                diff = targetPosition - startPosition;
            }
        }

        void Move::updateProgress()
        {
            Animator::updateProgress();

            if (node)
            {
                node->setPosition(startPosition + (diff * progress));
            }
        }
    } // namespace scene
} // namespace ouzel
