#include <cstdint>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"

#include "app/app_state.hpp"
#include "ui/overlays/title_bar.hpp"
#include "ui/overlays/command_line.hpp"

#include "app_components.hpp"

Element ImageParagraph(AppState& state) {
    return paragraph(state.asciiFrame)
            | size(WIDTH, GREATER_THAN, 1)
            | size(HEIGHT, GREATER_THAN, 1);
}

Element ImageCanvas(AppState& state) {
    return canvas([&](Canvas& c) {
        const int w { c.width() };
        const int h { c.height() };

        const auto frame = state.frame;

        for (int y {}; y < h; ++y) {
            for (int x {}; x < w; ++x) {
                const auto value = frame.at<std::uint8_t>(y, x);
                const auto index = value * (textGradient.size() - 1) / 255;
                const char ch { textGradient[index] };

                // draw pixel
                c.DrawText(x, y, std::string(1, ch));
            }
        }
    });
}

Component MakeView(AppState& state, Component container, Component commandInput) {
    return Renderer(container, [&]() {
        return dbox({
            ImageParagraph(state),
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