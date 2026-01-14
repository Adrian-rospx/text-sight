#pragma once

#include <iostream>

#include "opencv2/videoio.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "app/app_state.hpp"
#include "ui/app_components.hpp"

using namespace ftxui;

// defines worker thread loop that handles camera updates
void camera_loop(AppState& state, cv::VideoCapture& camera, ScreenInteractive& screen);

// main application encapsulated in a class
class AppController {
public:
    AppController(AppState& state) 
            : state(state)
            , screen(ScreenInteractive::Fullscreen()) {}

    int Run() {
        cv::VideoCapture camera(0);
        if (!camera.isOpened()) {
            std::cerr << "ERROR: Could not open camera\n";
            return 1;
        }

        auto view = MakeView(state);
        auto app  = MakeController(state, screen, view);

        std::thread worker(camera_loop, std::ref(state), std::ref(camera), std::ref(screen));

        screen.Loop(app);

        state.running = false;
        worker.join();

        return 0;
    }

private:
    AppState state{};
    ScreenInteractive screen {ScreenInteractive::Fullscreen()};
};