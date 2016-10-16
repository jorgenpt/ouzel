// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "SceneManager.h"
#include "Scene.h"
#include "core/Engine.h"
#include "Node.h"

namespace ouzel
{
    namespace scene
    {
        SceneManager::SceneManager()
        {
        }

        SceneManager::~SceneManager()
        {
        }

        bool SceneManager::setScene(Scene& scene)
        {
            if (currentScene != &scene)
            {
                if (currentScene)
                {
                    currentScene->leave();
                }

                currentScene = &scene;

                if (currentScene)
                {
                    currentScene->enter();
                }

                return true;
            }
            else
            {
                return false;
            }
        }

        bool SceneManager::removeScene(Scene& scene)
        {
            if (currentScene == &scene)
            {
                scene.leave();

                currentScene = nullptr;

                return true;
            }
            else
            {
                return false;
            }
        }

        bool SceneManager::eraseScene(Scene& scene)
        {
            if (currentScene == &scene)
            {
                currentScene = nullptr;
                
                return true;
            }
            else
            {
                return false;
            }
        }

        void SceneManager::draw()
        {
            if (currentScene)
            {
                currentScene->draw();
            }
        }
    } // namespace scene
} // namespace ouzel
