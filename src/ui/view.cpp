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

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"

#include "app/app_state.hpp"
#include "ui/image_canvas.hpp"
#include "ui/overlays/command_line.hpp"
#include "ui/overlays/title_bar.hpp"

#include "ui/view.hpp"

using namespace ftxui;

Component MakeView(AppState& state, Component container, Component commandInput) {
    return Renderer(container, [&]() {
        return dbox({
            ImageCanvas(state),
            titleBar(),
            commandLine(state, commandInput),
        });
    });
}