// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "SoundSample.h"

using namespace std;
using namespace ouzel;

SoundSample::SoundSample(Samples& pSamples):
    samples(pSamples),
    jumpButton("button.png", "button_selected.png", "button_down.png", "", "Jump", graphics::Color::BLACK, "arial.fnt"),
    ambientButton("button.png", "button_selected.png", "button_down.png", "", "Ambient", graphics::Color::BLACK, "arial.fnt"),
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", graphics::Color::BLACK, "arial.fnt")
{
    eventHandler.uiHandler = bind(&SoundSample::handleUI, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);

    audio::SoundDataPtr jumpData = sharedEngine->getAudio()->createSoundData();
    jumpData->initFromFile("jump.wav");

    jumpSound = sharedEngine->getAudio()->createSound();
    jumpSound->init(jumpData);

    audio::SoundDataPtr ambientData = sharedEngine->getAudio()->createSoundData();
    ambientData->initFromFile("ambient.wav");

    ambientSound = sharedEngine->getAudio()->createSound();
    ambientSound->init(ambientData);

    guiLayer.addCamera(camera);
    addLayer(guiLayer);

    guiLayer.addChild(menu);

    jumpButton.setPosition(Vector2(0.0f, 0.0f));
    menu.addWidget(jumpButton);


    ambientButton.setPosition(Vector2(0.0f, -40.0f));
    menu.addWidget(ambientButton);

    backButton.setPosition(Vector2(-200.0f, -200.0f));
    menu.addWidget(backButton);
}

SoundSample::~SoundSample()
{
}

bool SoundSample::handleUI(Event::Type type, const UIEvent& event) const
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.node == &backButton)
        {
            samples.back();
        }
        else if (event.node == &jumpButton)
        {
            jumpSound->play();
        }
        else if (event.node == &ambientButton)
        {
            ambientSound->play();
        }
    }

    return true;
}
