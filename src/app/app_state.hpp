#pragma once

#include <atomic>
#include <string>

struct AppState {
    std::string asciiFrame{};   // written to by camera thread
    std::string command{};
    
    int width { 120 };
    int height { 40 };
    int cursorPosition {};

    std::atomic<bool> isLoopRunning { true };     // controls the camera thread
    bool displayCommand { false };          // displays the command overlay
};