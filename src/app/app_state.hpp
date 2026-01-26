/* 
 * Copyright (C) 2026 Adrian Rosu
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#pragma once

#include <atomic>
#include <mutex>
#include <opencv2/core/mat.hpp>
#include <string>

const std::string_view textGradient { " .,-;<+*/ixaA8#M" };

struct AppState {
    cv::Mat frame {};   // written to by the camera thread (requires mutex)
    std::mutex frameMutex {};

    std::string asciiFrame {};  // written to by camera thread
    std::string command {};
    std::string statusMessage {};
    
    int cursorPosition {};  // controls view image position

    std::atomic<bool> isLoopRunning { true };     // controls the camera thread
    bool displayCommand { false };
    bool displayStatus { false };
};