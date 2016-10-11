// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class MainMenu: public ouzel::scene::Scene
{
public:
    MainMenu(Samples& pSamples);
    virtual ~MainMenu();

    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event);

private:
    Samples& samples;

    ouzel::scene::Camera camera;
    ouzel::scene::Layer layer;

    ouzel::gui::Menu menu;

    ouzel::EventHandler eventHandler;
    ouzel::gui::Button spritesButton;
    ouzel::gui::Button GUIButton;
    ouzel::gui::Button renderTargetButton;
    ouzel::gui::Button animationsButton;
    ouzel::gui::Button inputButton;
    ouzel::gui::Button soundButton;
};
