// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class AnimationsSample: public ouzel::scene::Scene
{
public:
    AnimationsSample(Samples& pSamples);

private:
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event) const;
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event) const;


private:
    Samples& samples;

    ouzel::scene::Layer layer;
    ouzel::scene::Camera camera;

    ouzel::scene::ShapeDrawable shapeDrawable;
    ouzel::scene::Sprite witchSprite;
    ouzel::scene::Node drawNode;
    ouzel::scene::Node witch;

    ouzel::scene::Node ball;
    ouzel::scene::Sprite ballSprite;

    ouzel::gui::Button backButton;

    ouzel::EventHandler eventHandler;

    ouzel::scene::Camera guiCamera;
    ouzel::scene::Layer guiLayer;

    ouzel::gui::Menu menu;

    std::vector<std::unique_ptr<ouzel::scene::Animator>> animators;
};
