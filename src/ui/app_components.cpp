#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"

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

Component MakeView(AppState& state, Component container, Component commandInput) {
    return Renderer(container, [&]() {
        return dbox({
            CameraView(state),
            titleBar(),
            commandOverlay(state)
        });
    });
}

Component MakeController(AppState& state, ScreenInteractive& screen, 
        Component appView, Component commandInput) {
    return CatchEvent(appView, [&](Event event) {
        // open the command line
        if (event == Event::Character(':')  && !state.displayCommand) {
            state.displayCommand = true;
            commandInput->TakeFocus();
            return true;
        }
        if (event == Event::Escape          && state.displayCommand) {
            state.displayCommand = false;
            return true;
        }
        if (event == Event::Character('q')  && !state.displayCommand) {
            state.running = false;
            screen.Exit();
            return true;
        }
        return false;
    });
}