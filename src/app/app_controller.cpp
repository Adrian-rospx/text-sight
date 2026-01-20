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

#include <mutex>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/elements.hpp"

#include "services/camera.hpp"
#include "ui/controller.hpp"
#include "ui/view.hpp"

#include "app/app_controller.hpp"

void camera_loop(AppState& state, Camera& camera, ScreenInteractive& screen) {
    using namespace std::chrono_literals;

    while (state.isLoopRunning) {
        camera.update();
        {
            std::lock_guard lock(state.frameMutex);
            state.frame = camera.getFrame();
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