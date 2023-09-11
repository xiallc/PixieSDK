/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2023 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file commands.hpp
 * @brief Omnitool commands
 */

#ifndef ONMITOOL_COMMANDS_HPP
#define ONMITOOL_COMMANDS_HPP

#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <pixie/error.hpp>
#include <pixie/utils/string.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/crate.hpp>

namespace xia {
namespace omnitool {
namespace command {
/*
 * Command line options
 */
using slot_range = std::vector<int>;
using module_range = std::vector<size_t>;
using channel_range = xia::pixie::channel::range;
using files = std::vector<std::string>;
using paths = std::vector<std::string>;

/**
 * @brief Session options set externally
 */
struct session_options {
    slot_range slots;
    slot_range excluded_slots;
    std::string firmware_host_path;
    files firmware_crate_files;
    files firmware_files;
    bool reg_trace;
    bool verbose;
    std::ostream& out;

    session_options(std::ostream& out);
};

/**
 * @brief Command context
 */
struct command;
struct context {
    xia::pixie::crate::module_crate& crate;
    session_options& opts;
    command& cmd;
    context(
        xia::pixie::crate::module_crate& crate_,
        session_options& opts_,
        command& cmd_) : crate(crate_), opts(opts_), cmd(cmd_) {}
};

/**
 * @brief Command Definition defines a command that runs a function
 *        when executed
 */
struct definition {
    using argument = std::string;
    using arguments = std::vector<argument>;
    using arguments_iter = arguments::iterator;
    using option = std::pair<argument, argument>;
    using options = std::vector<option>;
    using operation = argument;
    using operations = arguments;
    using handler = void (*)(context& );

    std::string group;
    std::string name;

    handler call;
    operations boot;

    size_t min_args;
    size_t max_args;
    size_t forced;
    options opts;

    std::string help_cmd;
    std::string help;

    definition() = default;

    std::string formatted_help(const bool long_opt, const size_t max) const;
};

using definitions = std::vector<definition>;

/**
 * @brief A command is an instance of the command to executed
 */
struct command {
    using argument = definition::argument;
    using arguments = definition::arguments;
    using arguments_iter = definition::arguments_iter;
    using option = definition::option;
    using options = definition::options;

    const definition& def;
    options opts;
    arguments args;

    command(const definition& cmd);

    /**
     * @brief Parse the options and arguments from the list of arguments
     */
    void parse(arguments_iter& ai, arguments_iter& ae);

    /**
     * @brief Get the argument and return an empty string is the user
     * did not supply one.
     */
    std::string get_arg();

    /**
     * @brief Is there an argument available?
     */
    bool has_arg() const;

    /**
     * @brief Get the option if valid
     */
    std::string get_option(const argument& opt) const;

    /**
     * @brief Is the option true?
     */
    bool option_true(const std::string& opt) const;

    /**
     * @brief Run the command
     */
    void run(context& context_);
};

using commands = std::vector<command>;

/**
 * @brief A batch of commands are executed in sequence
 */
struct batch {
    using operation_func = std::function<void(context& )>;
    struct operation {
        bool run_once;
        operation_func handler;
        operation(operation_func& handler_);
        operation();
        void run(context& context_);
    };
    using operations = std::map<std::string, operation>;

    commands cmds;
    paths path;

    operations ops;

    batch();

    /**
     * @brief Set a operation
     */
    void set_operation(const std::string& name, operation_func func);

    /**
     * @brief Parse a set of arguments into commands.
     *
     * This valids some basics of the commands before executing
     * them. It checks options are valid and argument counts are
     * within range.
     */
    void parse(command::arguments& cmds);

    /**
     * @brief Execute the batch of commands
     */
    void execute(xia::pixie::crate::module_crate& crate, session_options& opts);

    /**
     * @brief Run the operation
     */
    void run_operation(const std::string& name, context& context_);

    /**
     * @brief Report the parsed commands
     */
    void report(std::ostream& out);
};

#define omnitool_command_handler_decl(_name) void _name(xia::omnitool::command::context& )
#define omnitool_command_opt_decl(_name) {"-" _name, ""}
#define omnitool_command_opt_arg_decl(_name) {"-" _name, "true"}

/**
 * Return the no command iterator.
 */
definitions::const_iterator no_command();

/**
 * Is the command valid given the command iterator?
 */
bool valid_command(definitions::const_iterator cdi);

/**
 * Find the command given the absolute command path.
 */
definitions::const_iterator find_command(const command::argument& arg);

/**
 * Find the command from the command set. Search the provided path if
 * not absolute.
 */
definitions::const_iterator find_command(const command::argument& arg, paths& path);

/**
 * Populate the channels from the option. Defaults to all channels if
 * there is no option.
 */
void channels_option(
    channel_range& channels, const command::argument& opt, size_t num_channels);

/**
 * Populate the module's from the option. Defaults to all modules if
 * there is no option.
 */
void modules_option(
    module_range& modules, const command::argument& opt, size_t num_modules);

/**
 * Load the commands from a file.
 */
void load_commands(const std::string& name, command::arguments& cmds);

/**
 * Register commands
 */
void register_commands(const definitions& cmds);

} // namespace command
} // namespace omnitool
} // namespace xia

#endif  // ONMITOOL_COMMAND_HPP
