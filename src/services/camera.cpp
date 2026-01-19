#include <stdexcept>
#include <string>

#include "opencv2/core/mat.hpp"
#include "opencv2/imgproc.hpp"

#include "services/camera.hpp"
#include "app/app_state.hpp"    // contains the textgradient

void Camera::update() {
    capture >> lastFrame;
    if (lastFrame.empty()) 
        throw std::runtime_error("Error: Could not read frame from camera");
    
    // perform additional processing
    processedFrame = lastFrame.clone();
}

std::string Camera::getStringFrame(int width, int height) {
    if (processedFrame.empty()) 
        throw std::runtime_error("Error: empty processed frame can't be transformed");
    if (width <= 0 || height <= 0) 
        throw std::runtime_error("Error: width and height must be larger than 0");

    cv::Mat frame;
    cv::cvtColor(processedFrame, frame, cv::COLOR_BGR2GRAY);
    cv::resize(frame, frame, cv::Size(width, height));

    std::string str;
    str.reserve((width + 1) * height);

    for (int y{}; y < frame.rows; ++y) {
        for (int x{}; x < frame.cols; ++x) {
            const std::uint8_t pixel { frame.at<std::uint8_t>(y, x) };

            const int index = pixel * (textGradient.size() - 1) / 255;
            str += textGradient[index];
        }
        str += '\n';
    }

    return str;
}

cv::Mat Camera::getFrame() {
    if (processedFrame.empty()) 
        throw std::runtime_error("Error: empty processed frame can't be transformed");

    cv::Mat frame;
    cv::cvtColor(processedFrame, frame, cv::COLOR_BGR2GRAY);

    return frame;
}