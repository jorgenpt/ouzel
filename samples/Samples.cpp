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

    sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);

    setSample(sample);
}

void Samples::setSample(const std::string& sample)
{
    if (!sample.empty())
    {
        if (sample == "sprites")
        {
            current.reset(new SpritesSample(*this));
        }
        else if (sample == "gui")
        {
            current.reset(new GUISample(*this));
        }
        else if (sample == "render_target")
        {
            current.reset(new RTSample(*this));
        }
        else if (sample == "animations")
        {
            current.reset(new AnimationsSample(*this));
        }
        else if (sample == "input")
        {
            current.reset(new InputSample(*this));
        }
        else if (sample == "sound")
        {
            current.reset(new SoundSample(*this));
        }
    }

    if (!current)
    {
        current.reset(new MainMenu(*this));
    }

    sharedEngine->getSceneManager()->setScene(*current);
}

void Samples::back()
{
    sharedEngine->getInput()->setCursorVisible(true);

    current.reset(new MainMenu(*this));
    sharedEngine->getSceneManager()->setScene(*current);
}
