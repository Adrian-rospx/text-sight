#pragma once

#include <atomic>
#include <string>

struct AppState {
    std::string asciiFrame{};   // written to by camera thread
    std::string command{};
    
    int width { 120 };
    int height { 40 };
    
    std::atomic<bool> running { true };         // controls the camera thread
    std::atomic<bool> displayCommand { false }; // displays the command overlay
};