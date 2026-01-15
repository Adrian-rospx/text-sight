#pragma once

#include "opencv2/videoio.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "app/app_state.hpp"

using namespace ftxui;

// defines worker thread loop that handles camera updates
void camera_loop(AppState& state, cv::VideoCapture& camera, ScreenInteractive& screen);

// main application encapsulated in a class
class AppController {
public:
    AppController(AppState& state) 
            : state(state)
            , screen(ScreenInteractive::Fullscreen()) {}

    int run();

private:
    AppState& state;
    ScreenInteractive screen {ScreenInteractive::Fullscreen()};
};