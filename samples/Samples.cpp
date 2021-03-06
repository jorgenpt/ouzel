// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "Samples.h"
#include "MainMenu.h"
#include "SpritesSample.h"
#include "GUISample.h"
#include "RTSample.h"
#include "AnimationsSample.h"
#include "InputSample.h"
#include "SoundSample.h"

using namespace std;
using namespace ouzel;

void Samples::begin(const std::string& sample)
{
    sharedEngine->getInput()->startGamepadDiscovery();

#if OUZEL_PLATFORM_LINUX || OUZEL_PLATFORM_RASPBIAN || OUZEL_PLATFORM_EMSCRIPTEN
    sharedApplication->getFileSystem()->addResourcePath("Resources");
#elif OUZEL_PLATFORM_WINDOWS
    sharedApplication->getFileSystem()->addResourcePath("Resources");
    sharedApplication->getFileSystem()->addResourcePath("../Resources");
    sharedApplication->getFileSystem()->addResourcePath("../../Resources");
#endif

    sharedEngine->getRenderer()->setClearColor(graphics::Color(64, 0, 0));
    sharedEngine->getWindow()->setTitle("Samples");

    setSample(sample);
}

void Samples::setSample(const std::string& sample)
{
    currentScene.reset();

    if (!sample.empty())
    {
        if (sample == "sprites")
        {
            currentScene.reset(new SpritesSample(*this));
        }
        else if (sample == "gui")
        {
            currentScene.reset(new GUISample(*this));
        }
        else if (sample == "render_target")
        {
            currentScene.reset(new RTSample(*this));
        }
        else if (sample == "animations")
        {
            currentScene.reset(new AnimationsSample(*this));
        }
        else if (sample == "input")
        {
            currentScene.reset(new InputSample(*this));
        }
        else if (sample == "sound")
        {
            currentScene.reset(new SoundSample(*this));
        }
    }

    if (!currentScene)
    {
        currentScene.reset(new MainMenu(*this));
    }

    sharedEngine->getSceneManager()->setScene(currentScene.get());
}
