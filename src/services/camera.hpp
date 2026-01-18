#pragma once

#include <stdexcept>
#include <string>

#include "opencv2/videoio.hpp"
#include "opencv2/core/mat.hpp"

class Camera {
public:
    Camera() : capture(0) {
        if (!capture.isOpened())
            throw std::runtime_error("Error: Capture device can't be opened or is missing");
    }

    // updates and processes frame, 
    // if the update is unsuccessful it throws a runtime_error
    void update();

    // converts processed frame to an ascii string
    std::string toStringFrame(int width, int height);

private:
    cv::Mat lastFrame;
    cv::Mat processedFrame;
    cv::VideoCapture capture;
};