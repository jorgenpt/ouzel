// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "Animator.h"
#include "scene/Node.h"
#include "utils/Utils.h"
#include "core/Engine.h"

namespace ouzel
{
    namespace scene
    {
        Animator::Animator(float aLength):
            length(aLength)
        {
            updateCallback.callback = std::bind(&Animator::update, this, std::placeholders::_1);
        }

        Animator::~Animator()
        {
            if (node && node->currentAnimator == this) node->currentAnimator = nullptr;
        }

        void Animator::update(float delta)
        {
            if (running)
            {
                if (currentTime + delta >= length)
                {
                    done = true;
                    running = false;
                    progress = 1.0f;
                    currentTime = length;
                    if (finishHandler) finishHandler();
                }
                else
                {
                    currentTime += delta;
                    progress = currentTime / length;
                }

                updateProgress();
            }
        }

        void Animator::animate(Node& targetNode)
        {
            if (targetNode.currentAnimator != this)
            {
                if (targetNode.currentAnimator)
                {
                    targetNode.currentAnimator->remove();
                }

                targetNode.currentAnimator = this;
                sharedEngine->scheduleUpdate(updateCallback);
                start(targetNode);
            }
        }

        void Animator::start(Node& targetNode)
        {
            running = true;
            node = &targetNode;
        }

        void Animator::remove()
        {
            if (node)
            {
                sharedEngine->unscheduleUpdate(updateCallback);
                node->currentAnimator = nullptr;
                node = nullptr;
            }

            running = false;
        }

        void Animator::resume()
        {
            if (!running)
            {
                sharedEngine->scheduleUpdate(updateCallback);
                running = true;
            }
        }

        void Animator::stop(bool resetAnimation)
        {
            if (running)
            {
                sharedEngine->unscheduleUpdate(updateCallback);
                running = false;
            }

            if (resetAnimation)
            {
                reset();
            }
        }

        void Animator::reset()
        {
            done = false;
            currentTime = 0.0f;
            setProgress(0.0f);
        }

        void Animator::setProgress(float newProgress)
        {
            progress = newProgress;
            currentTime = progress * length;

            updateProgress();
        }

        void Animator::updateProgress()
        {
        }
    } // namespace scene
} // namespace ouzel
