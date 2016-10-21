// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <functional>
#include "utils/Noncopyable.h"
#include "utils/Types.h"
#include "core/UpdateCallback.h"

namespace ouzel
{
    namespace scene
    {
        class Node;
        
        class Animator: public Noncopyable
        {
        public:
            Animator(float aLength);
            virtual ~Animator();

            virtual void animate(Node& targetNode);
            virtual void start(Node& targetNode);

            virtual void resume();
            virtual void stop(bool resetAnimation = false);
            virtual void reset();
            virtual void remove();

            bool isRunning() const { return running; }
            bool isDone() const { return done; }

            float getLength() const { return length; }
            float getCurrentTime() const { return currentTime; }

            float getProgress() const { return progress; }
            virtual void setProgress(float newProgress);

            void setFinishHandler(const std::function<void()>& handler) { finishHandler = handler; }

        protected:
            virtual void update(float delta);
            virtual void updateProgress();

            float length = 0.0f;
            float currentTime = 0.0f;
            float progress = 0.0f;
            bool done = false;
            bool running = false;

            Node* node = nullptr;

            std::function<void()> finishHandler;
            UpdateCallback updateCallback;
        };
    } // namespace scene
} // namespace ouzel
