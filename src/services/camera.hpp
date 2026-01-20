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
    std::string getStringFrame(int width, int height);
    
    // returns the resized frame
    cv::Mat getFrame();

private:
    cv::Mat lastFrame;
    cv::Mat processedFrame;
    cv::VideoCapture capture;
};