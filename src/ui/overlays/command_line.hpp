#pragma once

#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"

#include "app/app_state.hpp"

using namespace ftxui;

inline Element commandLine(AppState& state, Component commandInput) {
    return state.displayCommand 
        ? vbox({
            filler(),
                hbox({
                    commandInput->Render(),
                    filler(),
                }),
            }) 
        : filler();
}