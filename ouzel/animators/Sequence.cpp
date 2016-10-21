// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <numeric>
#include "Sequence.h"

namespace ouzel
{
    namespace scene
    {
        Sequence::Sequence(const std::vector<Animator*>& aAnimators):
            Animator(std::accumulate(aAnimators.begin(), aAnimators.end(), 0.0f, [](float a, Animator* b) { return a + b->getLength(); })), animators(aAnimators)
        {
        }

        void Sequence::start(Node& targetNode)
        {
            Animator::start(targetNode);

            for (const auto& animator : animators)
            {
                animator->start(targetNode);
            }
        }

        void Sequence::reset()
        {
            Animator::reset();

            for (const auto& animator : animators)
            {
                animator->reset();
            }
        }

        void Sequence::remove()
        {
            for (Animator* animator : animators)
            {
                animator->remove();
            }
        }

        void Sequence::updateProgress()
        {
            Animator::updateProgress();

            float time = 0.0f;

            for (const auto& animator : animators)
            {
                if (animator->getLength() <= 0.0f || currentTime > time + animator->getLength())
                {
                    animator->setProgress(1.0f);
                }
                else if (currentTime <= time)
                {
                    animator->setProgress(0.0f);
                }
                else
                {
                    animator->setProgress((currentTime - time) / animator->getLength());
                }

                time += animator->getLength();
            }
        }
    } // namespace scene
} // namespace ouzel
