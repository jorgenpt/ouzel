// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class GUISample: public ouzel::scene::Scene
{
public:
    GUISample(Samples& pSamples);
    virtual ~GUISample();

private:
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event);
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event) const;

    Samples& samples;

    ouzel::scene::Camera camera;
    ouzel::scene::Layer guiLayer;

    ouzel::gui::Menu menu;
    ouzel::gui::Label label;
    ouzel::gui::Label label2;
    ouzel::gui::Label label3;

    ouzel::gui::Button backButton;
    
    ouzel::EventHandler eventHandler;

    ouzel::gui::Button button;
    ouzel::gui::Button fullscreenButton;
    ouzel::gui::CheckBox checkBox;
};
