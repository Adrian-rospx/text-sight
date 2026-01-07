#include <ftxui/dom/node.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <iostream>
#include <string>
#include <cstdint>

using namespace ftxui;

const std::string_view textGradient { " .,-;<+*/ixaA8#M" };

std::string imageToASCII(const cv::Mat& frame, const Screen& screen) {
    const int width { screen.dimx() };
    const int height { screen.dimy() };

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

int main(int argc, char* argv[]) {
    // attempt opening a video camera
    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera\n";
        return 1;
    }

    // open webcam window
    cv::namedWindow("Image input", cv::WINDOW_NORMAL);
    auto termScreen { ScreenInteractive::Create(Dimension::Full()) };
    
    cv::Mat frame;

    // main loop
    while (true) {
        // capture frame
        camera >> frame;

        // display
        cv::imshow("Image input", frame);
        
        const std::string asciiText { imageToASCII(frame, termScreen) };
        const auto asciiScreen { paragraph(asciiText ) };

        termScreen.Clear();
        Render(termScreen, asciiScreen);
        termScreen.Print();

        // wait 10ms; break on Esc
        if (cv::waitKey(10) == 27) {
            break;
        }
    }
}