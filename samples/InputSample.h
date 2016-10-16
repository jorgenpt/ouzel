// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class InputSample: public ouzel::scene::Scene
{
public:
    InputSample(Samples& pSamples);

private:
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event);
    bool handleMouse(ouzel::Event::Type type, const ouzel::MouseEvent& event);
    bool handleTouch(ouzel::Event::Type type, const ouzel::TouchEvent& event);
    bool handleGamepad(ouzel::Event::Type type, const ouzel::GamepadEvent& event);
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event) const;

    Samples& samples;

    ouzel::scene::Camera camera;
    ouzel::scene::Layer layer;

    ouzel::gui::Button backButton;

    ouzel::EventHandler eventHandler;

    ouzel::gui::Button button;
    ouzel::scene::Node flame;

    ouzel::scene::ParticleSystem flameParticleSystem;

    ouzel::scene::Camera guiCamera;
    ouzel::scene::Layer guiLayer;

    ouzel::gui::Menu menu;
};
