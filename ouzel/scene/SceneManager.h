// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "utils/Types.h"
#include "utils/Noncopyable.h"

namespace ouzel
{
    class Engine;

    namespace scene
    {
        class Scene;
        
        class SceneManager: public Noncopyable
        {
            friend Engine;
            friend Scene;
        public:
            virtual ~SceneManager();

            void draw();

            bool setScene(Scene& scene);
            bool removeScene(Scene& scene);
            Scene* getScene() const { return currentScene; }

        protected:
            bool eraseScene(Scene& scene);

            SceneManager();

            Scene* currentScene = nullptr;
        };
    } // namespace scene
} // namespace ouzel
