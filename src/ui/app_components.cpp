#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"

#include "app_components.hpp"
#include "app/app_state.hpp"
#include "ui/overlays/title_bar.hpp"

Component CommandInput(AppState& state) {
    auto input = Input(state.command, "Enter a command");

    return Renderer(input, [&]() {
        return vbox({
            text("Command") | bold,
            input->Render()
        })
        | border
        | bgcolor(Color::Black)
        | color(Color::White);
    });
}

Element Overlay(Component command) {
    return vbox({
        filler(),
        hbox({
            filler(),
            command->Render() | size(WIDTH, LESS_THAN, 60),
            filler()
        }),
        text("")    // small text margin    
    });
}

Element CameraView(AppState& state) {
    return paragraph(state.asciiFrame)
            | size(WIDTH, GREATER_THAN, 1)
            | size(HEIGHT, GREATER_THAN, 1);
};

Component MakeView(AppState& state) {
    auto command = CommandInput(state);

    return Renderer(command, [&]() {
        return dbox({
            CameraView(state),
            titleBar(),
            state.displayCommand ? Overlay(command) : filler(),
        });
    });
}

Component MakeController(AppState& state, ScreenInteractive& screen, Component appView) {
    return CatchEvent(appView, [&](Event event) {
        if (event == Event::Character(':')) {
            state.displayCommand = true;
            return true;
        }
        if (event == Event::Escape) {
            state.displayCommand = false;
            return true;
        }
        if (event == Event::Character('q')) {
            state.running = false;
            screen.Exit();
            return true;
        }
        return false;
    });
}