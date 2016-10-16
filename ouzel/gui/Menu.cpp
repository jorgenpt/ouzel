// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "Menu.h"
#include "core/Engine.h"
#include "events/EventDispatcher.h"

namespace ouzel
{
    namespace gui
    {
        Menu::Menu()
        {
            eventHandler.keyboardHandler = std::bind(&Menu::handleKeyboard, this, std::placeholders::_1, std::placeholders::_2);
            eventHandler.gamepadHandler = std::bind(&Menu::handleGamepad, this, std::placeholders::_1, std::placeholders::_2);
            eventHandler.uiHandler = std::bind(&Menu::handleUI, this, std::placeholders::_1, std::placeholders::_2);
        }

        Menu::~Menu()
        {
        }

        void Menu::enter()
        {
            sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);
        }

        void Menu::leave()
        {
            sharedEngine->getEventDispatcher()->removeEventHandler(eventHandler);
        }

        bool Menu::addWidget(Widget& widget)
        {
            if (!addChild(widget))
            {
                return false;
            }

            widgets.push_back(&widget);

            if (!selectedWidget)
            {
                selectWidget(&widget);
            }

            return true;
        }

        bool Menu::removeWidget(Widget& widget)
        {
            if (!removeChild(widget))
            {
                return false;
            }

            auto i = std::find(widgets.begin(), widgets.end(), &widget);

            if (i != widgets.end())
            {
                widgets.erase(i);
            }

            if (selectedWidget == &widget)
            {
                selectWidget(nullptr);
            }

            return true;
        }

        void Menu::selectWidget(Widget* widget)
        {
            selectedWidget = widget;

            for (Widget* childWidget : widgets)
            {
                childWidget->setSelected(childWidget == selectedWidget);
            }
        }

        void Menu::selectNextWidget()
        {
            if (widgets.empty()) return;

            std::vector<Widget*>::iterator widgetIterator = widgets.end();

            if (selectedWidget)
            {
                widgetIterator = std::find(widgets.begin(), widgets.end(), selectedWidget);
            }

            if (widgetIterator != widgets.end()) widgetIterator++;
            if (widgetIterator == widgets.end()) widgetIterator = widgets.begin();

            selectWidget(*widgetIterator);
        }

        void Menu::selectPreviousWidget()
        {
            if (widgets.empty()) return;

            std::vector<Widget*>::iterator widgetIterator = widgets.end();

            if (selectedWidget)
            {
                widgetIterator = std::find(widgets.begin(), widgets.end(), selectedWidget);
            }

            if (widgetIterator == widgets.begin())
            {
                widgetIterator = --widgets.end();
            }
            else
            {
                widgetIterator--;
            }

            selectWidget(*widgetIterator);
        }

        bool Menu::handleKeyboard(Event::Type type, const KeyboardEvent& event)
        {
            if (!enabled) return true;

            if (type == Event::Type::KEY_DOWN && !widgets.empty())
            {
                if (event.key == input::KeyboardKey::UP)
                {
                    selectPreviousWidget();
                }
                else if (event.key == input::KeyboardKey::DOWN)
                {
                    selectNextWidget();
                }
                else if (event.key == input::KeyboardKey::RETURN)
                {
                    if (selectedWidget)
                    {
                        Event clickEvent;
                        clickEvent.type = Event::Type::UI_CLICK_NODE;

                        clickEvent.uiEvent.node = selectedWidget;
                        clickEvent.uiEvent.position = selectedWidget->getPosition();

                        sharedEngine->getEventDispatcher()->postEvent(clickEvent);
                    }
                }
            }

            return true;
        }

        bool Menu::handleGamepad(Event::Type, const GamepadEvent&)
        {
            if (!enabled) return true;

            return true;
        }

        bool Menu::handleUI(Event::Type type, const UIEvent& event)
        {
            if (!enabled) return true;

            if (type == Event::Type::UI_ENTER_NODE)
            {
                if (std::find(widgets.begin(), widgets.end(), event.node) != widgets.end())
                {
                    selectWidget(static_cast<Widget*>(event.node));
                }
            }

            return true;
        }
    } // namespace gui
} // namespace ouzel
