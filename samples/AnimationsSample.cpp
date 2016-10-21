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

    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Shake(10.0f, Vector2(10.0f, 20.0f), 20.0f)));

    animators.back()->animate(drawNode);

    witch.setPosition(Vector2(200, 0.0f));
    witch.addComponent(witchSprite);
    layer.addChild(witch);

    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Scale(2.0f, Vector2(0.1f, 0.1f), false)));
    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Fade(2.0f, 0.4f)));

    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Rotate(1.0f, TAU, false)));
    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Repeat(animators[3].get(), 3)));

    vector<scene::Animator*> parallel = {
        animators[1].get(),
        animators[2].get()
    };

    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Parallel(parallel)));

    vector<scene::Animator*> sequence = {
        animators[4].get(),
        animators[5].get()
    };

    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Sequence(sequence)));

    animators.back()->animate(witch);

    ball.addComponent(ballSprite);
    layer.addChild(ball);

    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Animator(1.0f))); // delay
    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Move(2.0f, Vector2(0.0f, -240.0f), false)));
    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Ease(*animators.back().get(), scene::Ease::Type::OUT, scene::Ease::Func::BOUNCE)));

    vector<scene::Animator*> sequence2 = {
        animators[7].get(),
        animators[9].get()
    };

    animators.push_back(std::unique_ptr<scene::Animator>(new scene::Sequence(sequence2)));

    animators.back()->animate(ball);

    guiLayer.addCamera(guiCamera);
    addLayer(guiLayer);

    guiLayer.addChild(menu);

    backButton.setPosition(Vector2(-200.0f, -200.0f));
    menu.addWidget(backButton);
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
