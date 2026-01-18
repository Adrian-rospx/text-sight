#pragma once

#include <atomic>
#include <opencv2/core/mat.hpp>
#include <string>

const std::string_view textGradient { " .,-;<+*/ixaA8#M" };

struct AppState {
    cv::Mat frame {};   // written to by the camera thread

    std::string asciiFrame {};  // written to by camera thread
    std::string command {};
    
    int width { 120 };
    int height { 40 };
    int cursorPosition {};  // controls view image position

    std::atomic<bool> isLoopRunning { true };     // controls the camera thread
    bool displayCommand { false };          // displays the command overlay
};