#include <cstdint>
#include <mutex>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

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
    cv::Mat frameCopy;
    {
        std::lock_guard lock(state.frameMutex);
        if (!state.frame.empty())
            frameCopy = state.frame.clone();
    }

    return canvas([frameCopy, &state](Canvas& c) {
        if (frameCopy.empty())
            return;

        const int cw { c.width() };
        const int ch { c.height() };
        if (cw <= 0 || ch <= 0) return;

        const int targetW { cw };
        const int targetH { ch * 2 };

        cv::Mat resized;
        cv::resize(frameCopy, resized, cv::Size(targetW, targetH));

        for (int y {}; y < ch; ++y) {
            for (int x {}; x < cw; ++x) {
                const auto value = resized.at<std::uint8_t>(y * 2, x);
                const auto index = value * (textGradient.size() - 1) / 255;

                // draw pixel
                c.DrawText(x, y, std::string(1, textGradient[index]));
            }
        }
    });
}

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