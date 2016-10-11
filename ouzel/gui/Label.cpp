// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "core/CompileConfig.h"
#include "Label.h"
#include "core/Engine.h"
#include "graphics/Renderer.h"
#include "scene/Layer.h"
#include "scene/Camera.h"
#include "BMFont.h"
#include "core/Cache.h"
#include "scene/TextDrawable.h"

namespace ouzel
{
    namespace gui
    {
        Label::Label(const std::string& fontFile, const std::string& pText, const Vector2& textAnchor):
            textDrawable(fontFile, pText, textAnchor)
        {
            text = pText;

            addComponent(textDrawable);

            pickable = true;
        }

        Label::~Label()
        {
        }

        void Label::setText(const std::string& newText)
        {
            text = newText;
            textDrawable.setText(text);
        }
    } // namespace gui
} // namespace ouzel
