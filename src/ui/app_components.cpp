#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"

#include "app_components.hpp"
#include "app/app_state.hpp"
#include "ui/overlays/title_bar.hpp"

Element CameraView(AppState& state) {
    return paragraph(state.asciiFrame)
            | size(WIDTH, GREATER_THAN, 1)
            | size(HEIGHT, GREATER_THAN, 1);
}

Element commandOverlay(AppState& state) {
    return state.displayCommand 
        ? vbox({
            filler(),
                hbox({
                    text(state.command) | border
                        | size(WIDTH, GREATER_THAN, 10),
                    filler(),
                }),
            })
        : filler();
}

Component MakeView(AppState& state, Component container) {
    return Renderer(container, [&]() {
        return dbox({
            CameraView(state),
            titleBar(),
            commandOverlay(state)
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
        if (!state.displayCommand && event == Event::Character('q')) {
            state.running = false;
            screen.Exit();
            return true;
        }
        return false;
    });
}