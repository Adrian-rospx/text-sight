#include <mutex>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/elements.hpp"

#include "services/camera.hpp"
#include "ui/app_components.hpp"

#include "app/app_controller.hpp"

void camera_loop(AppState& state, Camera& camera, ScreenInteractive& screen) {
    using namespace std::chrono_literals;

    while (state.isLoopRunning) {
        camera.update();

        state.width = screen.dimx();
        state.height = screen.dimy();

        // state.asciiFrame = camera.getStringFrame(state.width, state.height);
        {
            std::lock_guard lock(state.frameMutex);
            state.frame = camera.getFrame(state.width, state.height);
        }

        screen.PostEvent(Event::Custom);

        std::this_thread::sleep_for(33ms); 
    }
}

int AppController::run() {
    Camera camera;

    // command input component
    InputOption commandOption = InputOption::Default();
    commandOption.transform = [](InputState state) {
        state.element = window(text("cmd"),
            std::move(state.element)
        )   | size(WIDTH, LESS_THAN, 32);
        
        return state.element;
    };
    commandOption.cursor_position = &state.cursorPosition;
    auto commandInput = Input(
            &state.command, 
            commandOption
    );

    // empty component
    auto focusSink = Renderer([]() {return filler();});

    // container for input components
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