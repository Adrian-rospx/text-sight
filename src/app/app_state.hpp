#pragma once

#include <string>

struct AppState {
    bool running { true };
    std::string asciiFrame{};

    int width { 120 };
    int height { 40 };

    bool displayCommand { false };
    std::string command{};
};