#include "opencv2/core/mat.hpp"
#include "opencv2/imgproc.hpp"

const std::string_view textGradient { " .,-;<+*/ixaA8#M" };

std::string imageToASCII(const cv::Mat& frame, const int width, const int height) {
    if (frame.empty()) return {};
    if (width <= 0 || height <= 0) return {};

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
