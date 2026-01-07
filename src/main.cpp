#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <iostream>

int main() {
    cv::Mat image(cv::Mat::zeros(200, 200, CV_8UC3));
    cv::circle(image, {100, 100}, 30, {0, 0, 255}, -1);

    cv::imshow("Test", image);
    cv::waitKey(1000);
}