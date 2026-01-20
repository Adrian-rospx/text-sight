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
 
#include <cstdint>
#include <mutex>

#include "opencv2/imgproc.hpp"

#include "ftxui/dom/elements.hpp"

#include "ui/image_canvas.hpp"

Element ImageParagraph(AppState& state) {
    return paragraph(state.asciiFrame)
            | size(WIDTH, GREATER_THAN, 1)
            | size(HEIGHT, GREATER_THAN, 1);
}

Element ImageCanvas(AppState& state) {
    cv::Mat frameCopy;
    {
        std::lock_guard lock(state.frameMutex);
        if (!state.frame.empty())
            frameCopy = state.frame.clone();
    }

    return canvas([frameCopy](Canvas& c) {
        if (frameCopy.empty())
            return;

        const int cw { c.width() };
        const int ch { c.height() };
        if (cw <= 0 || ch <= 0) return;

        const int targetW { cw };
        const int targetH { ch * 2 };

        cv::Mat resized;
        cv::resize(frameCopy, resized, cv::Size(targetW, targetH));

        for (int y {}; y < ch; ++y) {
            for (int x {}; x < cw; ++x) {
                const auto value = resized.at<std::uint8_t>(y * 2, x);
                const auto index = value * (textGradient.size() - 1) / 255;

                // draw pixel
                c.DrawText(x, y, std::string(1, textGradient[index]));
            }
        }
    });
}