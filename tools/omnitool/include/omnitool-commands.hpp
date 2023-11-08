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
#include <mutex>
#include <numeric>
#include <string>
#include <vector>

#include <pixie/error.hpp>
#include <pixie/utils/string.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/crate-view.hpp>

namespace xia {
namespace omnitool {
namespace command {
/*
 * Pixie command option types
 */
using slot_range = std::vector<int>;
using module_range = std::vector<size_t>;
using channel_range = pixie::channel::range;

/*
 * Command arguments and options
 */
using argument = std::string;
using arguments = std::vector<argument>;
using arguments_iter = arguments::iterator;
using option = std::pair<argument, argument>;
using options = std::vector<option>;
using files = std::vector<std::string>;
using paths = std::vector<std::string>;

/*
 * Forward decls
 */
struct context;
struct command;
struct completion;

/**
 * @brief Named Operations are name functions to perform custom
 *        functions.
 *
 * The command preconditions are named operations a command can
 * specify that must run before the command is run. The operations are
 * global to the session.
 */
struct named_operations {
    using operation_func = std::function<void(context& )>;
    struct operation {
        bool run_once;
        operation_func handler;
        std::once_flag done;
        operation(operation_func& handler_, bool run_once);
        operation(const operation& orig);
        operation();
        operation& operator=(const operation& orig);
        void run(context& context_);
    };
    using operations = std::map<std::string, operation>;

    /**
     * Named operations
     */
    operations ops;

    named_operations();

    /**
     * @brief Set a operation
     */
    void set(const std::string& name, operation_func func, bool run_once);

    /**
     * @brief Run the operation
     */
    void run(const std::string& name, context& context_);
};

/**
 * @brief Session options set externally
 */
struct session_options {
    std::string status;

    slot_range slots;
    slot_range excluded_slots;
    std::string firmware_host_path;
    files firmware_crate_files;
    files firmware_files;
    bool reg_trace;

    named_operations ops;

    bool verbose;
    std::ostream& out;
    paths path;
    size_t command_depth;

    session_options(std::ostream& out);
};

/**
 * @brief Command context
 */
struct context {
    pixie::crate::view::module& crate;
    session_options& opts;
    command& cmd;

    context(
        pixie::crate::view::module& crate_,
        session_options& opts_, command& cmd_) :
        crate(crate_), opts(opts_), cmd(cmd_) {}
};

/**
 * @brief Command Definition defines a command that runs a function
 *        when executed
 */
struct definition {
    using preconditions = arguments;
    using handler = void (*)(context& );
    using completion_handler = void (*)(context& , completion& );

    std::string group;
    std::string name;

    handler call;
    completion_handler completion;
    preconditions preconds;

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
    commands cmds;

    batch();

    /**
     * @brief Clear the command
     */
    void clear();

    /**
     * @brief Parse a set of arguments into commands.
     *
     * This valids some basics of the commands before executing
     * them. It checks options are valid and argument counts are
     * within range.
     */
    void parse(arguments& cmds, const paths& path);

    /**
     * @brief Execute the batch of commands
     */
    void execute(pixie::crate::view::module& crate, session_options& opts);

    /**
     * @brief Report the parsed commands
     */
    void report(std::ostream& out);
};

/**
 * @brief A completion entry
 */
struct completion_entry {
    enum struct node {
        command,
        directory,
        path,
        argument
    };
    node type;
    std::string name;
    std::string group;
    std::string help;
    std::string path;
    size_t count;

    completion_entry(
        node type, const std::string& name, const std::string& group,
        const std::string& help, const std::string& path);

    bool isdir() const;
    bool iscommand() const;
};

using completion_entries = std::vector<completion_entry>;

/**
 * @brief Completions data
 */
struct completion {
    const bool incomplete;
    arguments args;
    completion_entries entries;

    completion(const char* buf);

    inline bool has_args() const;
    inline bool no_args() const;

    inline size_t argc() const;
    inline const argument& argv(const size_t index) const;

    inline void add(completion_entry entry);

    bool partial_match(
        const std::string& name, const std::string& cmd) const;
};

inline bool completion::has_args() const {
    return !args.empty();
}

inline bool completion::no_args() const {
    return args.empty();
}

inline size_t completion::argc() const {
    return args.size();
}

inline const argument& completion::argv(const size_t index) const {
    if (index >= args.size()) {
        throw std::range_error("completion: arg index out of range");
    }
    return args[index];
}

inline void completion::add(completion_entry entry) {
    entries.push_back(entry);
}

#define omnitool_command_handler_decl(_name) \
   void _name(xia::omnitool::command::context& ); \
   void _name ## _comp(xia::omnitool::command::context& , \
                       xia::omnitool::command::completion& )
#define omnitool_command_handers(_name) _name, _name ## _comp
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
definitions::const_iterator find_command(const argument& arg);

/**
 * Find the command from the command set. Search the provided path if
 * not absolute.
 */
definitions::const_iterator find_command(
    const argument& arg, const paths& path);

/**
 * Populate the channels from the option. Defaults to all channels if
 * there is no option.
 */
void channels_option(
    channel_range& channels, const argument& opt, size_t num_channels);

/**
 * Populate the module's from the option. Defaults to all modules if
 * there is no option.
 */
void modules_option(
    module_range& modules, const argument& opt, size_t num_modules);

/**
 * Load the commands from a file.
 */
void load_commands(const std::string& name, arguments& cmds);

/**
 * List the command in a path
 */
void list_commands(const std::string& path, completion_entries& entries);

/**
 * Register commands
 */
void register_commands(const definitions& cmds);

} // namespace command
} // namespace omnitool
} // namespace xia

#endif  // ONMITOOL_COMMAND_HPP
