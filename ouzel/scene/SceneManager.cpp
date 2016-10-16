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

        void SceneManager::setScene(Scene& newScene)
        {
            if (scene != &newScene)
            {
                if (scene)
                {
                    scene->leave();
                }

                scene = &newScene;

                if (scene)
                {
                    scene->enter();
                }
            }
        }

        void SceneManager::draw()
        {
            if (scene)
            {
                scene->draw();
            }
        }
    } // namespace scene
} // namespace ouzel
