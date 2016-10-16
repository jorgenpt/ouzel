// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "AnimationsSample.h"
#include "MainMenu.h"

using namespace std;
using namespace ouzel;

AnimationsSample::AnimationsSample(Samples& pSamples):
    samples(pSamples),
    witchSprite("witch.png"),
    ballSprite("ball.png"),
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", graphics::Color::BLACK, "arial.fnt")
{
    eventHandler.uiHandler = bind(&AnimationsSample::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&AnimationsSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);

    layer.addCamera(camera);
    addLayer(layer);

    shapeDrawable.rectangle(Rectangle(100.0f, 100.0f), graphics::Color(0, 128, 128, 255), true);
    shapeDrawable.rectangle(Rectangle(100.0f, 100.0f), graphics::Color::WHITE, false);
    shapeDrawable.line(Vector2(0.0f, 0.0f), Vector2(50.0f, 50.0f), graphics::Color::CYAN);
    shapeDrawable.point(Vector2(75.0f, 75.0f), graphics::Color::RED);

    shapeDrawable.circle(Vector2(75.0f, 75.0f), 20.0f, graphics::Color::BLUE);
    shapeDrawable.circle(Vector2(25.0f, 75.0f), 20.0f, graphics::Color::BLUE, true);

    drawNode.addComponent(shapeDrawable);
    drawNode.setPosition(Vector2(-300, 0.0f));
    layer.addChild(drawNode);

    drawNode.animate(make_shared<scene::Shake>(10.0f, Vector2(10.0f, 20.0f), 20.0f));

    witch.setPosition(Vector2(200, 0.0f));
    witch.addComponent(witchSprite);
    layer.addChild(witch);

    vector<scene::AnimatorPtr> parallel = {
        make_shared<scene::Scale>(2.0f, Vector2(0.1f, 0.1f), false),
        make_shared<scene::Fade>(2.0f, 0.4f)
    };

    vector<scene::AnimatorPtr> sequence = {
        make_shared<scene::Repeat>(make_shared<scene::Rotate>(1.0f, TAU, false), 3),
        make_shared<scene::Parallel>(parallel)
    };

    witch.animate(make_shared<scene::Sequence>(sequence));

    ball.addComponent(ballSprite);
    layer.addChild(ball);

    vector<scene::AnimatorPtr> sequence2 = {
        make_shared<scene::Animator>(1.0f), // delay
        make_shared<scene::Ease>(make_shared<scene::Move>(2.0f, Vector2(0.0f, -240.0f), false), scene::Ease::Type::OUT, scene::Ease::Func::BOUNCE)
    };

    ball.animate(make_shared<scene::Sequence>(sequence2));

    guiLayer.addCamera(guiCamera);
    addLayer(guiLayer);

    guiLayer.addChild(menu);

    backButton.setPosition(Vector2(-200.0f, -200.0f));
    menu.addWidget(backButton);
}

AnimationsSample::~AnimationsSample()
{
}

bool AnimationsSample::handleUI(Event::Type type, const UIEvent& event) const
{
    if (type == Event::Type::UI_CLICK_NODE && event.node == &backButton)
    {
        samples.setSample("");
    }

    return true;
}

bool AnimationsSample::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
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
