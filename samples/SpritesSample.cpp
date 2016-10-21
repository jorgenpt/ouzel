// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "SpritesSample.h"
#include "MainMenu.h"

using namespace std;
using namespace ouzel;

SpritesSample::SpritesSample(Samples& pSamples):
    samples(pSamples),
    characterSprite("run.json"),
    fireSprite("fire.json"),
    triangleSprite("triangle.json"),
    hideButton("button.png", "button_selected.png", "button_down.png", "", "Show/hide", graphics::Color::BLACK, "arial.fnt"),
    wireframeButton("button.png", "button_selected.png", "button_down.png", "", "Wireframe", graphics::Color::BLACK, "arial.fnt"),
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", graphics::Color::BLACK, "arial.fnt"),
    move(4.0f, Vector2(300.0f, 0.0f))
{
    eventHandler.uiHandler = bind(&SpritesSample::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&SpritesSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);

    addLayer(layer);
    layer.addCamera(camera);

    // character
    characterSprite.play(true);

    character.addComponent(characterSprite);
    layer.addChild(character);
    character.setPosition(Vector2(-300.0f, 0.0f));

    move.animate(character);

    // fire
    fireSprite.setOffset(Vector2(0.0f, 20.0f));
    fireSprite.play(true);

    fireNode.addComponent(fireSprite);
    fireNode.setPosition(Vector2(-100.0f, -140.0f));
    layer.addChild(fireNode);

    // triangle
    triangleNode.addComponent(triangleSprite);
    triangleNode.setPosition(Vector2(100.0f, -140.0f));
    layer.addChild(triangleNode);

    guiLayer.addCamera(guiCamera);
    addLayer(guiLayer);

    guiLayer.addChild(menu);

    hideButton.setPosition(Vector2(-200.0f, 200.0f));
    menu.addWidget(hideButton);

    wireframeButton.setPosition(Vector2(-200.0f, 160.0f));
    menu.addWidget(wireframeButton);

    backButton.setPosition(Vector2(-200.0f, -200.0f));
    menu.addWidget(backButton);
}

bool SpritesSample::handleUI(Event::Type type, const UIEvent& event)
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.node == &backButton)
        {
            samples.setSample("");
        }
        else if (event.node == &hideButton)
        {
            character.setHidden(!character.isHidden());
        }
        else if (event.node == &wireframeButton)
        {
            layer.setWireframe(!layer.getWireframe());
        }
    }

    return true;
}

bool SpritesSample::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
{
    if (type == Event::Type::KEY_DOWN)
    {
        switch (event.key)
        {
            case input::KeyboardKey::ESCAPE:
                samples.setSample("");
                break;
            default:
                break;
        }
    }

    return true;
}
