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

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "app/app_state.hpp"

class CommandRegistry {
public:
    using Args = std::vector<std::string>;
    using CommandFunction = std::function<void(AppState&, const Args&)>;

    CommandRegistry() {
        init();
    }

    void registerCommand(std::string name, CommandFunction func);

    // execute the given command text
    void execute(AppState& state, const std::string& input);

private:
    void init();

    std::unordered_map<std::string, CommandFunction> m_commands{};
};