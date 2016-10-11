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

    eventHandler.keyboardHandler = bind(&Samples::handleKeyboard, this, placeholders::_1, placeholders::_2);

    sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);

    mainMenu = make_shared<MainMenu>(*this);

    scene::ScenePtr newScene;

    if (!sample.empty())
    {
        if (sample == "sprites")
        {
            newScene = make_shared<SpritesSample>(*this);
        }
        else if (sample == "gui")
        {
            newScene = make_shared<GUISample>(*this);
        }
        else if (sample == "render_target")
        {
            newScene = make_shared<RTSample>(*this);
        }
        else if (sample == "animations")
        {
            newScene = make_shared<AnimationsSample>(*this);
        }
        else if (sample == "input")
        {
            newScene = make_shared<InputSample>(*this);
        }
        else if (sample == "sound")
        {
            newScene = make_shared<SoundSample>(*this);
        }
    }

    if (newScene)
    {
        sharedEngine->getSceneManager()->setScene(*newScene);
    }
    else
    {
        sharedEngine->getSceneManager()->setScene(*mainMenu);
    }
}

void Samples::back()
{
    sharedEngine->getInput()->setCursorVisible(true);
    sharedEngine->getSceneManager()->setScene(mainMenu);
}

bool Samples::handleKeyboard(Event::Type type, const KeyboardEvent& event)
{
    if (event.key == ouzel::input::KeyboardKey::ESCAPE)
    {
        if (type == Event::Type::KEY_DOWN)
        {
            if (sharedEngine->getSceneManager()->getScene() != mainMenu)
            {
                back();
            }
            else
            {
                sharedEngine->exit();
            }
        }

        return false;
    }

    return true;
}
