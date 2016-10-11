// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <cstdint>
#include <memory>

namespace ouzel
{
    class Window;
    typedef std::shared_ptr<Window> WindowPtr;

    class EventDispatcher;
    typedef std::shared_ptr<EventDispatcher> EventDispatcherPtr;

    class Cache;
    typedef std::shared_ptr<Cache> CachePtr;

    class Localization;
    typedef std::shared_ptr<Localization> LocalizationPtr;

    class Language;
    typedef std::shared_ptr<Language> LanguagePtr;

    namespace audio
    {
        class Audio;
        typedef std::shared_ptr<Audio> AudioPtr;

        class Sound;
        typedef std::shared_ptr<Sound> SoundPtr;

        class SoundData;
        typedef std::shared_ptr<SoundData> SoundDataPtr;
    }

    namespace graphics
    {
        class Renderer;
        typedef std::shared_ptr<Renderer> RendererPtr;

        class Resource;
        typedef std::shared_ptr<Resource> ResourcePtr;

        class BlendState;
        typedef std::shared_ptr<BlendState> BlendStatePtr;

        class IndexBuffer;
        typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;

        class MeshBuffer;
        typedef std::shared_ptr<MeshBuffer> MeshBufferPtr;

        class RenderTarget;
        typedef std::shared_ptr<RenderTarget> RenderTargetPtr;

        class Shader;
        typedef std::shared_ptr<Shader> ShaderPtr;

        class Texture;
        typedef std::shared_ptr<Texture> TexturePtr;

        class VertexBuffer;
        typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
    }

    class FileSystem;
    typedef std::shared_ptr<FileSystem> FileSystemPtr;

    namespace input
    {
        class Input;
        typedef std::shared_ptr<Input> InputPtr;

        class Gamepad;
        typedef std::shared_ptr<Gamepad> GamepadPtr;
    }

    namespace scene
    {
        class SceneManager;
        typedef std::shared_ptr<SceneManager> SceneManagerPtr;

        class SpriteFrame;
        typedef std::shared_ptr<SpriteFrame> SpriteFramePtr;

        struct ParticleDefinition;
        typedef std::shared_ptr<ParticleDefinition> ParticleDefinitionPtr;

        class Animator;
        typedef std::shared_ptr<Animator> AnimatorPtr;
    } // namespace scene
} // namespace ouzel
