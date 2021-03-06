// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "Window.h"
#include "Engine.h"
#include "events/EventDispatcher.h"

namespace ouzel
{
    Window::Window(const Size2& aSize, bool aResizable, bool aFullscreen, const std::string& aTitle):
        size(aSize),
        resizable(aResizable),
        fullscreen(aFullscreen),
        title(aTitle)
    {
    }

    Window::~Window()
    {
    }

    bool Window::init()
    {
        return true;
    }

    void Window::close()
    {
    }

    void Window::setSize(const Size2& newSize)
    {
        if (size != newSize)
        {
            size = newSize;

            Event event;
            event.type = Event::Type::WINDOW_SIZE_CHANGE;

            event.windowEvent.window = this;
            event.windowEvent.size = size;
            event.windowEvent.title = title;
            event.windowEvent.fullscreen = fullscreen;

            sharedEngine->getEventDispatcher()->postEvent(event);
        }
    }

    void Window::setFullscreen(bool newFullscreen)
    {
        if (fullscreen != newFullscreen)
        {
            fullscreen = newFullscreen;

            Event event;
            event.type = Event::Type::WINDOW_FULLSCREEN_CHANGE;

            event.windowEvent.window = this;
            event.windowEvent.size = size;
            event.windowEvent.title = title;
            event.windowEvent.fullscreen = fullscreen;

            sharedEngine->getEventDispatcher()->postEvent(event);
        }
    }

    void Window::setTitle(const std::string& newTitle)
    {
        if (title != newTitle)
        {
            title = newTitle;

            Event event;
            event.type = Event::Type::WINDOW_TITLE_CHANGE;

            event.windowEvent.window = this;
            event.windowEvent.size = size;
            event.windowEvent.title = title;
            event.windowEvent.fullscreen = fullscreen;

            sharedEngine->getEventDispatcher()->postEvent(event);
        }
    }
}
