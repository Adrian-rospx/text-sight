#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    // attempt opening a video camera
    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera\n";
        return 1;
    }

    // open webcam window
    cv::namedWindow("Image input", cv::WINDOW_NORMAL);

    cv::Mat frame;

    // main loop
    while (true) {
        // capture frame
        camera >> frame;

        // display
        cv::imshow("Image input", frame);

        // wait 10ms; break on Esc
        if (cv::waitKey(10) == 27) {
            break;
        }
    }
}