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