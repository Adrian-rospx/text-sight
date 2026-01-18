#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <string>

#include "services/camera.hpp"

const std::string_view textGradient { " .,-;<+*/ixaA8#M" };

void Camera::update() {
    capture >> lastFrame;
    if (lastFrame.empty()) 
        throw std::runtime_error("Error: Could not read frame from camera");
    
    // perform additional processing
    processedFrame = lastFrame.clone();
}

std::string Camera::toStringFrame(int width, int height) {
    if (processedFrame.empty()) return {};
    if (width <= 0 || height <= 0) return {};

    cv::resize(processedFrame, processedFrame, cv::Size(width, height));
    cv::cvtColor(processedFrame, processedFrame, cv::COLOR_BGR2GRAY);

    std::string str;
    str.reserve((width + 1) * height);

    for (int y{}; y < processedFrame.rows; ++y) {
        for (int x{}; x < processedFrame.cols; ++x) {
            const std::uint8_t pixel { processedFrame.at<std::uint8_t>(y, x) };

            const int index = pixel * (textGradient.size() - 1) / 255;
            str += textGradient[index];
        }
        str += '\n';
    }

    return str;
}