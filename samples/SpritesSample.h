// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class SpritesSample: public ouzel::scene::Scene
{
public:
    SpritesSample(Samples& pSamples);

private:
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event);
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event) const;

    Samples& samples;

    ouzel::scene::Layer layer;
    ouzel::scene::Camera camera;

    ouzel::scene::Move move;

    ouzel::gui::Button backButton;

    ouzel::EventHandler eventHandler;

    ouzel::scene::Sprite characterSprite;
    ouzel::scene::Node character;
    ouzel::gui::Button hideButton;
    ouzel::gui::Button wireframeButton;

    ouzel::scene::Sprite fireSprite;
    ouzel::scene::Node fireNode;

    ouzel::scene::Sprite triangleSprite;
    ouzel::scene::Node triangleNode;

    ouzel::gui::Menu menu;
    ouzel::scene::Camera guiCamera;
    ouzel::scene::Layer guiLayer;
};
