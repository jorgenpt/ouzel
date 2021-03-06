// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <mutex>
#include <atomic>
#define NOMINMAX
#include <d3d11.h>
#include "graphics/BlendState.h"

namespace ouzel
{
    namespace graphics
    {
        class BlendStateD3D11: public BlendState
        {
        public:
            BlendStateD3D11();
            virtual ~BlendStateD3D11();
            virtual void free() override;

            ID3D11BlendState* getBlendState() const { return blendState; }

        protected:
            virtual bool upload() override;

            ID3D11BlendState* blendState = nullptr;
        };
    } // namespace graphics
} // namespace ouzel
