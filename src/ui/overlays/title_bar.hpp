#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

using namespace ftxui;

inline Element titleBar() {
    return vbox({
        hbox({
            filler(),
            text("Text sight") 
                | underlined,
            filler()
        }),
        filler()
    });
}