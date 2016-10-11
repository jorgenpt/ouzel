// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "RTSample.h"

using namespace std;
using namespace ouzel;

RTSample::RTSample(Samples& pSamples):
    samples(pSamples),
    characterSprite("run.json"),
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", graphics::Color::BLACK, "arial.fnt")
{
    eventHandler.uiHandler = bind(&RTSample::handleUI, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);

    addLayer(rtLayer);

    ouzel::graphics::RenderTargetPtr renderTarget = sharedEngine->getRenderer()->createRenderTarget();
    renderTarget->init(Size2(256.0f, 256.0f), false);
    renderTarget->setClearColor(graphics::Color(0, 64, 0));

    rtCamera.setRenderTarget(renderTarget);
    rtLayer.addCamera(rtCamera);

    camera1.setViewport(Rectangle(0.0f, 0.0f, 0.5f, 1.0f));
    camera2.setViewport(Rectangle(0.5f, 0.0f, 0.5f, 1.0f));

    layer.addCamera(camera1);
    layer.addCamera(camera2);
    addLayer(layer);

    characterSprite.play(true);

    rtCharacter.addComponent(characterSprite);
    rtLayer.addChild(rtCharacter);

    scene::SpriteFramePtr rtFrame = std::make_shared<scene::SpriteFrame>(renderTarget->getTexture(), Rectangle(0.0f, 0.0f, 256.0f, 256.0f), false, renderTarget->getTexture()->getSize(), Vector2(), Vector2(0.5f, 0.5f));

    const std::vector<scene::SpriteFramePtr>& spriteFrames = { rtFrame };
    rtSprite.reset(new scene::Sprite(spriteFrames));
    rtNode.addComponent(*rtSprite);
    layer.addChild(rtNode);

    guiLayer.addCamera(guiCamera);
    addLayer(guiLayer);

    guiLayer.addChild(menu);

    backButton.setPosition(Vector2(-200.0f, -200.0f));
    menu.addWidget(backButton);
}

RTSample::~RTSample()
{
}

bool RTSample::handleUI(Event::Type type, const UIEvent& event) const
{
    if (type == Event::Type::UI_CLICK_NODE && event.node == &backButton)
    {
        samples.back();
    }

    return true;
}
