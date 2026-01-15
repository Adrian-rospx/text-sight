#pragma once

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "app/app_state.hpp"

using namespace ftxui;

Component MakeView(AppState& state, Component container);

Component MakeController(AppState& state, ScreenInteractive& screen, Component appView);