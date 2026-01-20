#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"

#include "app/app_state.hpp"
#include "ui/image_canvas.hpp"
#include "ui/overlays/title_bar.hpp"
#include "ui/overlays/command_line.hpp"

#include "app_components.hpp"

Component MakeView(AppState& state, Component container, Component commandInput) {
    return Renderer(container, [&]() {
        return dbox({
            ImageCanvas(state),
            titleBar(),
            commandLine(state, commandInput),
        });
    });
}

Component MakeController(
        AppState& state, 
        ScreenInteractive& screen, 
        Component view, 
        Component commandInput,
        Component focusSink
) {
    return CatchEvent(view, [&](Event event) {
        // open the command line
        if (event == Event::Character(':')  && !state.displayCommand) {
            state.displayCommand = true;
            state.command = ":";
            state.cursorPosition = 1;

            commandInput->TakeFocus();
            return true;
        }
        if (event == Event::Character('q')  && !state.displayCommand) {
            state.isLoopRunning = false;
            screen.Exit();
            return true;
        }
        if (event == Event::Escape          && state.displayCommand) {
            state.displayCommand = false;
            state.command.clear();

            focusSink->TakeFocus();
            return true;
        }
        if (event == Event::Backspace       && state.displayCommand 
            && state.command == ":") {
            state.displayCommand = false;
            state.command.clear();

            focusSink->TakeFocus();   
            return true;
        }
        return false;
    });
}