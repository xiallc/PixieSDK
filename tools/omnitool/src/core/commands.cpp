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

#include <cstring>
#include <fstream>
#include <mutex>

#include <string.h>

#include <pixie/utils/io.hpp>
#include <pixie/utils/path.hpp>

#include <omnitool-commands.hpp>

#include "crossline.h"

namespace xia {
namespace omnitool {
namespace command {
/*
 * Commands are registered at start up
 */
static definitions command_set;

/*
 * Command group labels.
 */
/*
 * Command group labels.
 */
using group = std::vector<std::string>;
static group command_groups;

named_operations::operation::operation(
    operation_func& handler_, bool run_once_)
    : run_once(run_once_), handler(handler_) {}

named_operations::operation::operation(const named_operations::operation& orig)
    : run_once(orig.run_once), handler(orig.handler) {}

named_operations::operation::operation() : run_once(false) {}

named_operations::operation& named_operations::operation::operator=(
    const named_operations::operation& orig) {
    /*
     * The once flag cannot be moved or copied so ignore it. This lets
     * the asignment to the map work without error.
     */
    run_once = orig.run_once;
    handler = orig.handler;
    return *this;
}

void named_operations::operation::run(context& context_) {
    /*
     * Provide a context for the call once to run the handler
     * function.
     */
    if (run_once) {
        std::call_once(done, [&self = *this, &context_]() {
            self.handler(context_);
        });
    } else {
        handler(context_);
    }
}

named_operations::named_operations() {
    set("none", [](context& ){}, true);
}

void named_operations::set(
    const std::string& name, operation_func func, bool run_once) {
    auto check = ops.find(name);
    if (check != ops.end()) {
        throw std::runtime_error("operation aleady registered: " + name);
    }
    ops[name] = operation({func, run_once});
}

void named_operations::run(const std::string& name, context& context_) {
    auto opi = ops.find(name);
    if (opi == ops.end()) {
        throw std::runtime_error("invalid operation: " + name);
    }
    auto& op = std::get<1>(*opi);
    op.run(context_);
}

session_options::session_options(std::ostream& out_)
    : reg_trace(false), verbose(false), out(out_), command_depth(0) {}

std::string definition::formatted_help(const bool long_opt, const size_t max) const {
    std::ostringstream oss;
    if (long_opt) {
        oss << ' '<< std::left << name << ": "
            << std::endl
            << "  " << help << std::endl;
        if (min_args == max_args) {
            if (min_args == 0) {
                oss << "   - no arguments";
            } else {
                oss << "   - requires " << min_args << " argument";
                if (min_args > 1) {
                    oss << 's';
                }
            }
        } else if (min_args < max_args) {
            oss << "   - requires " << min_args << " to " << max_args << " arguments ";
        } else {
            throw std::runtime_error("invalid command args range: " + name);
        }
        oss << std::endl
            << "     # " << name << ' ' << help_cmd << std::endl;
    } else {
        oss << ' ' << std::left << std::setw(max + 1) << name
            << " - " << help
            << std::endl;
    }
    return oss.str();
}

command::command(const definition& cmd) : def(cmd) {}

void command::parse(arguments_iter& ai, arguments_iter& ae) {
    while (ai != ae) {
        auto& arg = *ai;
        if (arg[0] != '-') {
            break;
        }
        ++ai;
        auto is_arg_option = [&arg](auto& o) {
            return util::string::starts_with(arg, std::get<0>(o));
        };
        auto doi = std::find_if(def.opts.begin(), def.opts.end(), is_arg_option);
        if (doi == def.opts.end()) {
            throw std::runtime_error(
                "option (" + arg + ") argument not found: " + def.name);
        }
        auto& opt_switch = std::get<0>(*doi);
        auto opt_arg = !std::get<1>(*doi).empty();
        auto is_option = [&opt_switch](auto& o) {
            return opt_switch == std::get<0>(o);
        };
        auto oi = std::find_if(opts.begin(), opts.end(), is_option);
        if (oi != opts.end()) {
            throw std::runtime_error(
                "command option used more than once: " +
                opt_switch + " (" + def.name + ')');
        }
        if (opt_arg) {
            if (ai == ae || (*ai)[0] == '-') {
                throw std::runtime_error(
                    "option (" + opt_switch + ") argument not found: " + def.name);
            }
            opts.push_back({opt_switch, *ai++});
        } else {
            opts.push_back({arg, "true"});
        }
    }
    while (ai != ae) {
        auto& arg = *ai++;
        if (arg[0] == '-') {
            throw std::runtime_error("command option after argument: " + def.name);
        } else {
            args.push_back(arg);
        }
    }
    if (args.size() < def.min_args) {
        throw std::runtime_error("not enough arguments: " + def.name);
    }
    if (args.size() > def.max_args) {
        throw std::runtime_error("too many arguments: " + def.name);
    }
    while (args.size() < def.max_args) {
        args.push_back({});
    }
}

 std::string command::get_arg() {
    if (args.empty()) {
        throw std::runtime_error("no arguments to get: " + def.name);
    }
    std::string arg = args.front();
    args.erase(args.begin());
    return arg;
}

bool command::has_arg() const {
    return !args.empty();
}

std::string command::get_option(const argument& opt) const {
    auto is_option = [&opt](auto& o) {
        return opt == std::get<0>(o);
    };
    auto oi = std::find_if(opts.begin(), opts.end(), is_option);
    if (oi == opts.end()) {
        auto odef = std::find_if(def.opts.begin(), def.opts.end(), is_option);
        if (odef == def.opts.end()) {
            throw std::runtime_error("invalid option to get: " + opt + " (" + def.name + ')');
        }
    }
    std::string opt_arg;
    if (oi != opts.end()) {
        opt_arg = std::get<1>(*oi);
    }
    return opt_arg;
}

bool command::option_true(const std::string& opt) const {
    return opt == "true";
}

void command::run(omnitool::command::context& context) {
    def.call(context);
}

batch::batch() {
}

void batch::clear() {
    cmds.clear();
}

void batch::parse(arguments& args, const paths& path) {
    auto ai = args.begin();
    while (ai != args.end()) {
        auto the_cmd = find_command(*ai, path);
        if (the_cmd == no_command()) {
            throw std::runtime_error("invalid command: " + *ai);
        }
        ++ai;
        command cmd(*the_cmd);
        size_t forced = cmd.def.forced;
        auto ae = ai;
        while (ae != args.end()) {
            if (forced == 0) {
                auto next_cmd = find_command(*ae, path);
                if (next_cmd != no_command()) {
                    break;
                }
            } else {
                --forced;
            }
            ++ae;
        }
        cmd.parse(ai, ae);
        cmds.push_back(cmd);
    }
}

void batch::execute(
    xia::pixie::crate::module_crate& crate, session_options& opts) {
    ++opts.command_depth;
    for (auto& cmd : cmds) {
        omnitool::command::context context(crate, opts, cmd);
        for (auto& op : cmd.def.preconds) {
            opts.ops.run(op, context);
        }
        cmd.run(context);
    }
    --opts.command_depth;
}

void batch::report(std::ostream& out) {
    out << "Total: " << cmds.size() << std::endl;
    size_t count = 0;
    for (auto& cmd : cmds) {
        out << std::setw(4) << ++count
            << ' ' << cmd.def.name
            << " (" << cmd.def.group << ')' << std::endl
            << "      args: total: " << cmd.args.size()
            << " (max=" << cmd.def.max_args
            << ", min=" << cmd.def.min_args
            << ", forced=" << cmd.def.forced << ')' << std::endl;
        size_t arg_count = 0;
        for (auto& arg : cmd.args) {
            out << "      " << std::setw(2) << ++arg_count << ": ";
            if (arg.empty()) {
                out << "  <empty>";
            } else {
                out << arg;
            }
            out << std::endl;
        }
        out << "      options: total: " << cmd.opts.size() << std::endl;
        size_t opt_count = 0;
        for (auto opt : cmd.opts) {
            out << "      " << std::setw(2) << ++opt_count
                << ": " << std::get<0>(opt) << ' ' << std::get<1>(opt) << std::endl;
        }
    }
}

command_entry::command_entry(
    node type_, const std::string& name_, const std::string& group_,
    const std::string& help_, const std::string& path_)
    : type(type_), name(name_), group(group_), help(help_), path(path_), count(1) {
}

bool command_entry::isdir() const {
    return type == node::directory;
}

bool command_entry::iscommand() const {
    return type == node::command;
}

static bool is_last_char_a_space(const char* buf) {
    auto len = ::strnlen(buf, 256);
    if (len > 0) {
        return buf[len - 1] == ' ';
    }
    return false;
}

command_completion::command_completion(const char* buf)
    : incomplete(!is_last_char_a_space(buf)) {
    util::string::split(args, buf);
}

bool command_completion::partial_match(
    const std::string& name, const std::string& cmd) const {
    return name.size() != cmd.size() && util::string::starts_with(name, cmd);
};

definitions::const_iterator no_command() {
    return command_set.end();
}


bool valid_command(definitions::const_iterator cdi) {
    return cdi != no_command();
}

definitions::const_iterator find_command(const argument& arg) {
    for (definitions::const_iterator cdi = command_set.begin();
         cdi != command_set.end();
         ++cdi) {
        const auto& def = *cdi;
        if (def.name == arg) {
            return cdi;
        }
    }
    return no_command();
}

definitions::const_iterator find_command(
    const argument& arg, const paths& path) {
    auto ci = find_command(arg);
    if (valid_command(ci)) {
        return ci;
    }
    for (auto& dir : path) {
        auto dci = find_command(dir + '/' + arg);
        if (valid_command(dci)) {
            return dci;
        }
    }
    return no_command();
}

void channels_option(
    channel_range& channels, const argument& opt, size_t num_channels) {
    if (opt.empty()) {
        channels.resize(num_channels);
        xia::pixie::channel::range_set(channels);
    } else {
        auto chans = util::io::get_values<size_t>(opt, num_channels);
        for (auto c : chans) {
            channels.push_back(c);
        }
    }
}

void modules_option(
    module_range& modules, const argument& opt, size_t num_modules) {
    if (opt.empty()) {
        modules.resize(num_modules);
        std::iota(modules.begin(), modules.end(), 0);
    } else {
        modules = util::io::get_values<size_t>(opt, num_modules);
    }
}

void load_commands(const std::string& name, arguments& cmds) {
    std::ifstream in(name);
    if (!in) {
        throw std::runtime_error(
            std::string("command file open: ") + name + ": " +
            std::strerror(errno));
    }
    std::string line;
    while (std::getline(in, line)) {
        util::string::replace(line, '\r', ' ');
        util::string::replace(line, '\t', ' ');
        util::string::strings sc;
        util::string::split(sc, line);
        if (!sc.empty()) {
            cmds.insert(std::end(cmds), std::begin(sc), std::end(sc));
        }
    }
}

void list_commands(const std::string& path, command_entries& entries) {
    /*
     * Spliting is not fast but simple. If this is a performance issue
     * we can look at faster ways to do this.
     */
    util::string::strings path_parts;
    util::string::split(path_parts, path, '/');
    bool path_found = false;
    for (auto& cmd : command_set) {
        util::string::strings cmd_parts;
        util::string::split(cmd_parts, cmd.name, '/');
        if (path_parts.size() < cmd_parts.size()) {
            bool match = true;
            for (size_t p = 0; p < path_parts.size(); ++p) {
                if (path_parts[p] != cmd_parts[p]) {
                    match = false;
                    break;
                }
            }
            if (!match) {
                continue;
            }
            path_found = true;
            auto& name = cmd_parts[path_parts.size()];
            bool found = false;
            for (auto& entry : entries) {
                if (entry.name == name) {
                    ++entry.count;
                    found = true;
                    break;
                }
            }
            if (!found) {
                auto type = command_entry::node::command;
                std::string help;
                if (cmd_parts.size() > path_parts.size() + 1) {
                    type = command_entry::node::directory;
                } else {
                    help = cmd.help + " - " +
                        util::path::basename(cmd.name) + ' ' +
                        cmd.help_cmd;
                }
                entries.push_back({type, name, cmd.group, help, path});
            }
        }
    }
    if (!path_found) {
        throw std::runtime_error(
            std::string("path not found: ") + path);
     }
    std::sort(entries.begin(), entries.end(), [](auto& a, auto& b) {
        return a.name < b.name;
    });
}

omnitool_command_handler_decl(help);
static const definition help_cmd = {
    "Utilities", "/util/help",
    omnitool_command_handers(help),
    {"none"},
    0, 1, 1,
    {omnitool_command_opt_decl("l")},
    "[-l] [command]",
    "Command specific help. Add '-l' to list all commands"
};

omnitool_command_handler_decl(shell);
static const definition shell_cmd = {
    "Utilities", "/util/shell",
    omnitool_command_handers(shell),
    {},
    0, 0, 0,
    {},
    "",
    "start an interactive shell session"
};

omnitool_command_handler_decl(wait);
static const definition wait_cmd = {
    "Utilities", "/util/wait",
    omnitool_command_handers(wait),
    {"none"},
    1, 1, 0,
    {},
    "msecs",
    "wait a number of msecs; add 's' for seconds and 'm' for minutes"
};

static const definitions util_commands = {
    help_cmd,
    shell_cmd,
    wait_cmd
};

void help(xia::omnitool::command::context& context) {
    auto long_opt = context.cmd.get_option("-l");
    context.opts.out << "Command help:" << std::endl
                     << " 1. Command options and argument are required in the order help reports."
                     << std::endl
                     << " 2. Command options are '-' and flag, eg '-x', or option switch and argument, eg '-d 0'."
                     << std::endl
                     << " 3. 'modules(s)' and 'channel(s)' can be a number or series:"
                     << std::endl
                     << "     eg '0' or '3,4,6' or '3,4-5,10,20-22'" << std::endl;
    auto help_opt = context.cmd.get_arg();
    auto mi = std::max_element(
        command_set.begin(), command_set.end(),
        [](auto& a, auto& b) {
            return a.name.size() < b.name.size();
        });
    size_t max = (*mi).name.size();
    if (!help_opt.empty()) {
        auto is_command = [&help_opt](auto& cmd) {
            if (help_opt == cmd.name) { return true; }
            auto n = cmd.name.rfind('/');
            if (n != std::string::npos) {
                return cmd.name.compare(n + 1, cmd.name.size() - n, help_opt) == 0;
            }
            return false;
        };
        auto ci = std::find_if(
            command_set.begin(), command_set.end(), is_command);
        if (ci == command_set.end()) {
            context.opts.out << "error: command not found: " << help_opt << std::endl;
        } else {
            context.opts.out << (*ci).formatted_help(true, max);
        }
    } else {
        for (auto& group : command_groups) {
            context.opts.out << group << ':' << std::endl;
            for (auto& cmd : command_set) {
                if (cmd.group == group) {
                    context.opts.out << cmd.formatted_help(long_opt == "true", max);
                }
            }
        }
    }
}

bool help_comp(context& context, command_completion& completions) {
    (void) context;
    (void) completions;
    return false;
}

struct shell_session {
    using arguments = util::string::strings;

    static const size_t buffer_size = 256;

    char buffer[buffer_size];

    context& context_;

    arguments args;
    batch cmd_set;
    bool running;
    std::string path;
    std::string prompt;

    shell_session(context& context_);
    ~shell_session();

    void clear();
    void make_prompt();

    void execute();

    void get_completions(command_completion& completions);

    bool builtins();
    void builtin_completions(command_completion& completions);

    bool builtin_ls();
    void builtin_ls_completion(command_completion& completions);
    bool builtin_cd();
    void builtin_cd_completion(command_completion& completions);
};

shell_session::shell_session(context& context__)
    : context_(context__), running(true), path("/") {
    clear();
    xia_log(xia::log::info) << "shell: start";
}

shell_session::~shell_session() {
    xia_log(xia::log::info) << "shell: end";
}

void shell_session::clear() {
    std::memset(&buffer[0], 0, buffer_size);
}

void shell_session::make_prompt() {
    context_.opts.ops.run("status", context_);
    prompt = "[" + context_.opts.status + "] " + path + " # ";
}

void shell_session::execute() {
    xia_log(xia::log::info) << "shell: cmd: " << buffer;
    util::string::split(args, buffer);
    if (running && !args.empty()) {
        using error = pixie::error::error;
        try {
            if (!builtins()) {
                auto& cmd = args[0];
                if (!cmd.empty() && cmd[0] != '/') {
                    cmd = path + '/' + cmd;
                }
                cmd_set.clear();
                cmd_set.parse(args, context_.opts.path);
                cmd_set.execute(context_.crate, context_.opts);
            }
        } catch (error& e) {
            xia_log(xia::log::error) << e;
            std::cout << e << std::endl;
        } catch (std::exception& e) {
            xia_log(xia::log::error) << e.what();
            std::cout << "error: " << e.what() << std::endl;
        } catch (...) {
            throw;
        }
    }
}

void shell_session::get_completions(command_completion& completions) {
    builtin_completions(completions);
    command_entries entries;
    list_commands(path, entries);
    if (completions.no_args()) {
        for (auto& search_path : context_.opts.path) {
            list_commands(search_path, entries);
        }
    }
    for (auto& entry : entries) {
        if (completions.no_args() ||
            (!entry.isdir() &&
             completions.partial_match(entry.name, completions.argv(0)))) {
            completions.add(entry);
        }
    }
}

bool shell_session::builtins() {
    bool r = false;
    if (!args.empty()) {
        if (args[0] == "builtins") {
            std::cout << "shell builtins:" << std::endl
                      << " ls   : list commands in the command directory" << std::endl
                      << " cd   : change command directory" << std::endl
                      << " pwd  : present working comment directory" << std::endl
                      << " exit : exit the shell" << std::endl;
            r = true;
        } else if (args[0] == "ls") {
            r = builtin_ls();;
        } else if (args[0] == "cd") {
            r = builtin_cd();;
        } else if (args[0] == "pwd") {
            std::cout << path << std::endl;
            r = true;
        } else if (args[0] == "exit") {
            running = false;
            r = true;
        }
    }
    return r;
}

void shell_session::builtin_completions(command_completion& completions) {
    builtin_ls_completion(completions);
    builtin_cd_completion(completions);
    if (completions.argc() == 1 && completions.incomplete) {
        auto& cmd = completions.argv(0);
        if (completions.partial_match("builtins", cmd)) {
            completions.add(
                {command_entry::node::command, "builtins", "Shell",
                 "builtins help - builtins", "builtins"});
        } else if (completions.partial_match("pwd", cmd)) {
            completions.add(
                {command_entry::node::command, "pwd", "Shell",
                 "current command directory - pwd", "pwd"});
        } else if (completions.partial_match("exit", cmd)) {
            completions.add(
                {command_entry::node::command, "exit", "Shell",
                 "exit the current shell session - exit", "exit"});
        }
    }
}

bool shell_session::builtin_ls() {
    paths search_paths;
    bool list_long = false;
    if (args.size() > 1) {
        for (size_t a = 1; a < args.size(); ++a) {
            if (args[a][0] == '-') {
                if (args[a] == "-l") {
                    list_long = true;
                } else {
                    throw std::runtime_error(
                        std::string("invalid option: ") + args[a]);
                }
            } else {
                if (args[a][0] == '/') {
                    search_paths.push_back(args[a]);
                } else {
                    const char* sep = "/";
                    if (path == "/") {
                        sep = "";
                    }
                    search_paths.push_back(path + sep + args[a]);
                }
            }
        }
    }
    if (search_paths.empty()) {
        search_paths.push_back(path);
    }
    command_entries entries;
    for (auto& search_path : search_paths) {
        list_commands(search_path, entries);
    }
    util::io::ostream_guard oguard(std::cout);
    std::cout << "total: " << entries.size() << std::endl;
    if (list_long) {
        for (auto& entry : entries) {
            char t = entry.type == command_entry::node::directory ? 'd' : 'c';
            std::cout << std::right << std::setw(4) << entry.count
                      << ' ' << t << ' '
                      << std::left << entry.name << std::endl;
        }
    } else if (!entries.empty()) {
        auto mi = std::max_element(
            entries.begin(), entries.end(), [](auto& a, auto& b) {
                return a.name.size() < b.name.size();
            });
        const size_t max = std::max(size_t(10), (*mi).name.size()) + 2;
        size_t at = 0;
        std::cout << std::left;
        for (auto& entry : entries) {
            std::cout << std::setw(max) << entry.name;
            at += max;
            if (at > 80) {
                std::cout << std::endl;
                at = 0;
            }
        }
        if (at > 0) {
            std::cout << std::endl;
        }
    }
    return true;
}

void shell_session::builtin_ls_completion(command_completion& completions) {
    if (completions.argc() == 1 && completions.incomplete) {
        auto& cmd = completions.argv(0);
        if (cmd == "ls" || completions.partial_match("ls", cmd)) {
            completions.add(
                {command_entry::node::command, "ls", "Shell",
                 "list command directory - ls [-l] [path..]", "ls"});
        }
    } else {
    }
}

bool shell_session::builtin_cd() {
    if (args.size() == 1) {
        path = "/";
    } else if (args.size() == 2) {
        std::string new_path = path;
        util::string::strings path_parts;
        util::string::split(path_parts, args[1], '/');
        if (path_parts.size() == 0) {
            new_path = "/";
        } else {
            for (auto& pp : path_parts) {
                if (pp == ".") {
                    /* nothing to do */
                } else if (pp == "..") {
                    if (new_path == "/") {
                        throw std::runtime_error("path not found: ..");
                    }
                    new_path = util::path::dirname(new_path);
                    if (new_path.empty()) {
                        new_path = "/";
                    }
                } else {
                    command_entries entries;
                    list_commands(new_path, entries);
                    bool found = false;
                    for (auto& entry : entries) {
                        if (entry.isdir()) {
                            if (entry.name == pp) {
                                const char* sep = "/";
                                if (new_path == "/") {
                                    sep = "";
                                }
                                new_path = new_path + sep + pp;
                                found = true;
                                break;
                            }
                        }
                    }
                    if (!found) {
                        throw std::runtime_error(
                            std::string("path not found: ") + pp);
                    }
                }
            }
        }
        path = new_path;
    } else {
        throw std::runtime_error("too many arguments");
    }
    return true;
}

void shell_session::builtin_cd_completion(command_completion& completions) {
    if (completions.argc() == 1 && completions.incomplete) {
        auto& cmd = completions.argv(0);
        if (cmd == "cd" || completions.partial_match("cd", cmd)) {
            completions.add(
                {command_entry::node::command, "cd", "Shell",
                 "change directory - cd [path]", "cd"});
        }
    } else {
        std::string dir_path = path;
        std::string dir;
        if (completions.argc() == 2) {
            auto& arg = completions.argv(1);
            if (arg.find('/') != std::string::npos) {
                dir_path = util::path::dirname(arg);
                if (dir_path[0] != '/') {
                    dir_path = path + '/' + dir_path;
                }
            }
            dir = util::path::basename(arg);
        }
        command_entries entries;
        list_commands(dir_path, entries);
        for (auto& entry : entries) {
            if (entry.isdir() &&
                (dir.empty() ||
                 completions.partial_match(entry.name, dir))) {
                completions.add(entry);
            }
        }
    }
}

static void shell_completion(
    void* user, char const* buf, crossline_completions_t* completion) {
    shell_session* ssession = static_cast<shell_session*>(user);
    try {
        command_completion completions(buf);
        ssession->get_completions(completions);
        for (auto& entry : completions.entries) {
            crossline_color_e colour = CROSSLINE_FGCOLOR_WHITE;
            std::string name = entry.name;
            if (entry.isdir()) {
                colour = CROSSLINE_FGCOLOR_WHITE;
            } else if (entry.group == "Shell") {
                colour = CROSSLINE_FGCOLOR_YELLOW;
            } else if (entry.group == "Crate") {
                colour = CROSSLINE_FGCOLOR_MAGENTA;
            } else if (entry.group == "Module") {
                colour = CROSSLINE_FGCOLOR_GREEN;
            } else if (entry.group == "Utilities") {
                colour = CROSSLINE_FGCOLOR_BLUE;
            } else if (entry.group == "QA") {
                colour = CROSSLINE_FGCOLOR_CYAN;
            }
            colour = crossline_color_e(CROSSLINE_FGCOLOR_BRIGHT | colour);
            if (entry.isdir()) {
                ::crossline_completion_add_incomplete_color(
                    completion, name.c_str(), colour, entry.help.c_str(), colour, '/');
            } else {
                ::crossline_completion_add_color(
                    completion, name.c_str(), colour, entry.help.c_str(), colour);
            }
        }
    } catch (...) {};
}

void shell(context& context_) {
    if (context_.opts.command_depth > 1) {
        throw std::runtime_error("shell: already running");
    }
    shell_session ssession(context_);
    ::crossline_user_completion_register(
        shell_completion, static_cast<void*>(&ssession));
    std::cout << "Pixie16 Shell (enter 'builtins' for shell help, tab to complete)"
              << std::endl;
    while (ssession.running) {
        ssession.make_prompt();
        auto line = ::crossline_readline(
            ssession.prompt.c_str(), &ssession.buffer[0], ssession.buffer_size);
        if (line == nullptr) {
            break;
        }
        ssession.execute();
    };
    ::crossline_user_completion_register(nullptr, nullptr);
}

bool shell_comp(context& context, command_completion& completions) {
    (void) context;
    (void) completions;
    return false;
}

void wait(xia::omnitool::command::context& context) {
    auto period_opt = context.cmd.get_arg();
    size_t multipler = 1;
    switch (period_opt.back()) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      break;
    case 's':
      multipler = 1000;
      break;
    case 'm':
      multipler = 60 * 1000;
      break;
    default:
      throw std::runtime_error("wait: invalid time units: " + period_opt.back());
      break;
    }
    auto msecs = util::io::get_value<size_t>(period_opt);
    msecs *= multipler;
    if (context.opts.verbose) {
        context.opts.out << "waiting " << msecs << " msecs" << std::endl;
    }
    xia::pixie::hw::wait(msecs * 1000);
}

bool wait_comp(context& context, command_completion& completions) {
    (void) context;
    (void) completions;
    return false;
}

static void commands_registration(const definitions& cmds) {
    for (auto& cmd : cmds) {
        auto fi = std::find(command_groups.begin(), command_groups.end(), cmd.group);
        if (fi == command_groups.end()) {
            command_groups.push_back(cmd.group);
        }
        command_set.push_back(cmd);
    }
    std::sort(command_groups.begin(), command_groups.end());
    std::sort(
        command_set.begin(), command_set.end(), [](auto& a, auto& b) {
        return a.name < b.name;
    });
}

static std::once_flag utils_registered;

void register_commands(const definitions& cmds) {
    std::call_once(utils_registered, []() {
        commands_registration(util_commands);
    });
    commands_registration(cmds);
}
}
}
}
