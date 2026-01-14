#pragma once

#include <string>

#include "opencv2/core/mat.hpp"

// Converts image matrix into an ASCII string frame
std::string imageToASCII(const cv::Mat& frame, const int width, const int height);
