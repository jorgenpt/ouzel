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
        }

        Animator::~Animator()
        {
            if (node) node->removeAnimation();
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

        void Animator::start(Node* targetNode)
        {
            if (!running)
            {
                running = true;
                node = targetNode;
            }
        }

        void Animator::remove()
        {
            node = nullptr;
        }

        void Animator::resume()
        {
            if (!running)
            {
                running = true;
            }
        }

        void Animator::stop(bool resetAnimation)
        {
            if (running)
            {
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
