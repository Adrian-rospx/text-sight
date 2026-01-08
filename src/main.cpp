#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/elements.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <thread>

using namespace ftxui;

const std::string_view textGradient { " .,-;<+*/ixaA8#M" };

struct AppState {
    std::string ascii_frame{};
    bool running { true };
    int width { 120 };
    int height { 40 };
};

std::string imageToASCII(const cv::Mat& frame, const int width, const int height) {
    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(width, height));
    cv::cvtColor(resized, resized, cv::COLOR_BGR2GRAY);

    std::string str;
    str.reserve((width + 1) * height);

    for (int y{}; y < resized.rows; ++y) {
        for (int x{}; x < resized.cols; ++x) {
            const std::uint8_t pixel { resized.at<std::uint8_t>(y, x) };

            const int index = pixel * (textGradient.size() - 1) / 255;
            str += textGradient[index];
        }
        str += '\n';
    }

    return str;
}

void camera_loop(AppState& state, cv::VideoCapture& camera, ScreenInteractive& screen) {
    using namespace std::chrono_literals;

    cv::Mat frame;

    while (state.running) {
        camera >> frame;
        if (frame.empty()) continue;

        state.width = screen.dimx();
        state.height = screen.dimy();

        state.ascii_frame = imageToASCII(frame, state.width, state.height);

        screen.PostEvent(Event::Custom);

        std::this_thread::sleep_for(33ms); 
    }
}

Component MakeView(AppState& state) {
    return Renderer([&]() {
        return paragraph(state.ascii_frame);
    });
}

Component MakeController(AppState& state, ScreenInteractive& screen,Component view) {
    return CatchEvent(view, [&](Event event) {
        if (event == Event::Character('q')) {
            state.running = false;
            screen.Exit();
            return true;
        }
        return false;
    });
}

int main(int argc, char* argv[]) {
    AppState state {};

    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera\n";
        return 1;
    }

    // start terminal screen
    auto screen { ScreenInteractive::Fullscreen() };

    auto view = MakeView(state);
    auto app  = MakeController(state, screen, view);

    std::thread worker(camera_loop, std::ref(state), std::ref(camera), std::ref(screen));

    screen.Loop(app);

    state.running = false;
    worker.join();
}