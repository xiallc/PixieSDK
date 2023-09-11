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

#include <pixie/utils/io.hpp>

#include <omnitool-commands.hpp>

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

session_options::session_options(std::ostream& out_)
    : reg_trace(false), verbose(false), out(out_) {}

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

std::string command::get_option(const command::argument& opt) const {
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


batch::operation::operation() : run_once(false) {}

batch::operation::operation(operation_func& handler_) : run_once(false), handler(handler_) {}

void batch::operation::run(omnitool::command::context& context) {
    if (!run_once) {
        run_once = true;
        handler(context);
    }
}

batch::batch() {
    set_operation("none", [](context& ){});
}

void batch::set_operation(const std::string& name, operation_func func) {
    auto check = ops.find(name);
    if (check != ops.end()) {
        throw std::runtime_error("operation aleady registered: " + name);
    }
    ops[name] = operation({func});
}

void batch::parse(command::arguments& args) {
    auto ai = args.begin();
    while (ai != args.end()) {
        auto the_cmd = find_command(*ai++, path);
        if (the_cmd == no_command()) {
            throw std::runtime_error("invalid command: " + *ai);
        }
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
    for (auto& cmd : cmds) {
        omnitool::command::context context(crate, opts, cmd);
        for (auto& op : cmd.def.boot) {
            run_operation(op, context);
        }
        cmd.run(context);
    }
}

void batch::run_operation(const std::string& name, omnitool::command::context& context) {
    auto opi = ops.find(name);
    if (opi == ops.end()) {
        throw std::runtime_error("invalid operation: " + name);
    }
    auto& op = std::get<1>(*opi);
    op.run(context);
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

definitions::const_iterator no_command() {
    return command_set.end();
}


bool valid_command(definitions::const_iterator cdi) {
    return cdi != no_command();
}

definitions::const_iterator find_command(const command::argument& arg) {
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

definitions::const_iterator find_command(const command::argument& arg, paths& path) {
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
    channel_range& channels, const command::argument& opt, size_t num_channels) {
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
    module_range& modules, const command::argument& opt, size_t num_modules) {
    if (opt.empty()) {
        modules.resize(num_modules);
        std::iota(modules.begin(), modules.end(), 0);
    } else {
        modules = util::io::get_values<size_t>(opt, num_modules);
    }
}

void load_commands(const std::string& name, command::arguments& cmds) {
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

omnitool_command_handler_decl(help);
static const definition help_cmd = {
    "Utilities", "/util/help", help,
    {"none"},
    0, 1, 1,
    {omnitool_command_opt_decl("l")},
    "[-l] [command]",
    "Command specific help. Add '-l' to list all commands"
};

omnitool_command_handler_decl(wait);
static const definition wait_cmd = {
    "Utilities", "/util/wait", wait,
    {"none"},
    1, 1, 0,
    {},
    "msecs",
    "wait a number of msecs; add 's' for seconds and 'm' for minutes"
};

static const definitions util_commands = {
    help_cmd,
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
