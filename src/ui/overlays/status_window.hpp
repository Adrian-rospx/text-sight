#pragma once

#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component_base.hpp"

#include "app/app_state.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/deprecated.hpp>

using namespace ftxui;

inline Component StatusWindow(AppState& state) {
    return Maybe(Renderer([&]() {
        return vbox({
            filler(),
            hbox({
                filler(),
                window(text("status") | center,
                    vbox({
                        paragraph(state.statusMessage),
                        separatorEmpty(),
                        text("Press \'Esc\' to close") | dim 
                    }) | clear_under
                ),
                filler()
            }),
            filler()
        });
    }), &state.displayStatus);
}