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

#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "app/app_state.hpp"

#include "services/command_registry.hpp"

static std::vector<std::string> tokenize(const std::string& str) {
    std::istringstream iss(str);

    return {
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>{}
    };
}

void CommandRegistry::registerCommand(std::string name, CommandFunction func) {
    m_commands[std::move(name)] = std::move(func);
}

void CommandRegistry::execute(AppState& state, const std::string& input) {
    // obtain command tokens
    auto tokens = tokenize(input);
    if (tokens.empty()) return;

    // find command in registry
    const auto it = m_commands.find(tokens[0]);
    if (it == m_commands.end())
        throw std::runtime_error("Error: Unknown command");

    // execute
    it->second(state, tokens);
}

void CommandRegistry::init() {
    registerCommand(":q", [](AppState& state, auto&) {
        state.isLoopRunning = false;
    });
    registerCommand(":quit", [](AppState& state, auto&) {
        state.isLoopRunning = false;
    });
}