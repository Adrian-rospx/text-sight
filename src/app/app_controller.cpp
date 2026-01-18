#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <iostream>
#include <opencv2/core.hpp>

#include "ui/app_components.hpp"
#include "ascii.hpp"

#include "app/app_controller.hpp"

void camera_loop(AppState& state, cv::VideoCapture& camera, ScreenInteractive& screen) {
    using namespace std::chrono_literals;

    cv::Mat frame;

    while (state.isLoopRunning) {
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

    InputOption commandOption = InputOption::Default();
    commandOption.transform = [](InputState state) {
        state.element = window(text("cmd") | bold,
            std::move(state.element)
        )   | size(WIDTH, LESS_THAN, 32);
        
        return state.element;
    };
    commandOption.cursor_position = &state.cursorPosition;

    auto commandInput = Input(
            &state.command, 
            commandOption
    );
    auto focusSink = Renderer([]() {return filler();});

    auto container = Container::Vertical({
            focusSink,
            commandInput,
    });

    auto view = MakeView(state, container, commandInput);
    auto app  = MakeController(
            state, 
            screen, 
            view, 
            commandInput, 
            focusSink
    );

    std::thread worker(camera_loop, 
            std::ref(state), std::ref(camera), std::ref(screen));

    screen.Loop(app);

    state.isLoopRunning = false;
    worker.join();

    return 0;
}