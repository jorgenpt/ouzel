// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class RTSample: public ouzel::scene::Scene
{
public:
    RTSample(Samples& pSamples);
    virtual ~RTSample();

private:
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event) const;
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event) const;

    Samples& samples;

    ouzel::gui::Button backButton;
    
    ouzel::EventHandler eventHandler;

    ouzel::scene::Layer rtLayer;
    ouzel::scene::Camera rtCamera;

    ouzel::scene::Camera camera1;
    ouzel::scene::Camera camera2;
    ouzel::scene::Layer layer;

    ouzel::scene::Sprite characterSprite;
    ouzel::scene::Node rtCharacter;

    std::auto_ptr<ouzel::scene::Sprite> rtSprite;
    ouzel::scene::Node rtNode;

    ouzel::scene::Camera guiCamera;
    ouzel::scene::Layer guiLayer;
    ouzel::gui::Menu menu;
};
