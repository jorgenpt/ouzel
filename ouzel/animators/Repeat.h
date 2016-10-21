// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "animators/Animator.h"
#include "utils/Types.h"

namespace ouzel
{
    namespace scene
    {
        class Repeat: public Animator
        {
        public:
            Repeat(Animator* aAnimator, uint32_t aCount = 0);

            virtual void start(Node& targetNode) override;
            virtual void reset() override;
            virtual void remove() override;

        protected:
            virtual void updateProgress() override;

            Animator* animator;
            uint32_t count = 0;
            uint32_t currentCount = 0;
        };
    } // namespace scene
} // namespace ouzel
