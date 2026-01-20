/* 
 * Copyright (C) 2026 Adrian Rosu
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <stdexcept>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/event.hpp"

#include "app/app_state.hpp"
#include "services/command_registry.hpp"

#include "ui/controller.hpp"

using namespace ftxui;

Component MakeController(
        AppState& state, 
        CommandRegistry& registry,
        Component view, 
        Component commandInput,
        Component focusSink
) {
    return CatchEvent(view, [&](Event event) {
        // open the command line
        if (event == Event::Character(':')  && !state.displayCommand) {
            state.displayCommand = true;
            state.command = ":";
            state.cursorPosition = 1;

            commandInput->TakeFocus();
            return true;
        }
        if (event == Event::Character('q')  && !state.displayCommand) {
            state.isLoopRunning = false;
            return true;
        }
        if (event == Event::Escape      && state.displayCommand) {
            state.displayCommand = false;
            state.command.clear();

            focusSink->TakeFocus();
            return true;
        }
        if (event == Event::Backspace   && state.displayCommand
                && state.command == ":") {
            state.displayCommand = false;
            state.command.clear();

            focusSink->TakeFocus();   
            return true;
        }
        // command execution
        if (event == Event::Return      && state.displayCommand) {
            try {
                registry.execute(state, state.command);
            } catch (std::runtime_error e) {
                
            }

            state.command.clear();
            state.displayCommand = false;

            focusSink->TakeFocus();
            return true;
        }
        return false;
    });
}