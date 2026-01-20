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

#pragma once

#include "ftxui/component/screen_interactive.hpp"

#include "app/app_state.hpp"
#include "services/camera.hpp"

using namespace ftxui;

// defines worker thread loop that handles camera updates
void camera_loop(AppState& state, Camera& camera, ScreenInteractive& screen);

// main application encapsulated in a class
class AppController {
public:
    AppController(AppState& state) 
            : state(state)
            , screen(ScreenInteractive::Fullscreen()) {}

    // attempt to start the application
    int run();

private:
    AppState& state;
    ScreenInteractive screen {ScreenInteractive::Fullscreen()};
};