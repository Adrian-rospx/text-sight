#include <iostream>

#include "ui/app_components.hpp"
#include "ascii.hpp"

#include "app/app_controller.hpp"

void camera_loop(AppState& state, cv::VideoCapture& camera, ScreenInteractive& screen) {
    using namespace std::chrono_literals;

    cv::Mat frame;

    while (state.running) {
        camera >> frame;
        if (frame.empty()) continue;

        state.width = screen.dimx();
        state.height = screen.dimy();

        state.asciiFrame = imageToASCII(frame, state.width, state.height);

        screen.PostEvent(Event::Custom);

        std::this_thread::sleep_for(33ms); 
    }
}

int AppController::run() {
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