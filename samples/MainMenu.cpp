// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "MainMenu.h"
#include "SpritesSample.h"
#include "GUISample.h"
#include "RTSample.h"
#include "AnimationsSample.h"
#include "InputSample.h"
#include "SoundSample.h"

using namespace std;
using namespace ouzel;

MainMenu::MainMenu(Samples& pSamples):
    samples(pSamples),
    spritesButton("button.png", "button_selected.png", "button_down.png", "", "Sprites", graphics::Color(20, 0, 0, 255), "arial.fnt"),
    GUIButton("button.png", "button_selected.png", "button_down.png", "", "GUI", graphics::Color(20, 0, 0, 255), "arial.fnt"),
    renderTargetButton("button.png", "button_selected.png", "button_down.png", "", "Render target", graphics::Color(20, 0, 0, 255), "arial.fnt"),
    animationsButton("button.png", "button_selected.png", "button_down.png", "", "Animations", graphics::Color(20, 0, 0, 255), "arial.fnt"),
    inputButton("button.png", "button_selected.png", "button_down.png", "", "Input", graphics::Color(20, 0, 0, 255), "arial.fnt"),
    soundButton("button.png", "button_selected.png", "button_down.png", "", "Sound", graphics::Color(20, 0, 0, 255), "arial.fnt")
{
    eventHandler.uiHandler = bind(&MainMenu::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&MainMenu::handleKeyboard, this, placeholders::_1, placeholders::_2);

    sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);

    addLayer(layer);

    layer.addCamera(camera);

    layer.addChild(menu);

    spritesButton.setPosition(Vector2(0.0f, 80.0f));
    menu.addWidget(spritesButton);

    GUIButton.setPosition(Vector2(0.0f, 40.0f));
    menu.addWidget(GUIButton);

    renderTargetButton.setPosition(Vector2(0.0f, 0.0f));
    menu.addWidget(renderTargetButton);

    animationsButton.setPosition(Vector2(0.0f, -40.0f));
    menu.addWidget(animationsButton);

    inputButton.setPosition(Vector2(0.0f, -80.0f));
    menu.addWidget(inputButton);

    soundButton.setPosition(Vector2(0.0f, -120.0f));
    menu.addWidget(soundButton);
}

bool MainMenu::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
{
    if (event.key == ouzel::input::KeyboardKey::ESCAPE)
    {
        if (type == Event::Type::KEY_DOWN)
        {
            sharedEngine->exit();
        }

        return false;
    }

    return true;
}

bool MainMenu::handleUI(Event::Type type, const UIEvent& event) const
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.node == &spritesButton)
        {
            samples.setSample("sprites");
        }
        else if (event.node == &GUIButton)
        {
            samples.setSample("gui");
        }
        else if (event.node == &renderTargetButton)
        {
            samples.setSample("render_target");
        }
        else if (event.node == &animationsButton)
        {
            samples.setSample("animations");
        }
        else if (event.node == &inputButton)
        {
            samples.setSample("input");
        }
        else if (event.node == &soundButton)
        {
            samples.setSample("sound");
        }
    }

    return true;
}
