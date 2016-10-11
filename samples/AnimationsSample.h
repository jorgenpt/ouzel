// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class AnimationsSample: public ouzel::scene::Scene
{
public:
    AnimationsSample(Samples& pSamples);
    virtual ~AnimationsSample();

    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event) const;

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
};
