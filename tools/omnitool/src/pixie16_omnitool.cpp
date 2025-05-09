/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/**
 * @brief A General Purpose Tool for Pixie16
 *
 * Named after an all-purpose tool in the Mass Effect video game.
 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>

#include <pixie/error.hpp>
#include <pixie/log.hpp>
#include <pixie/utils/io.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/crate-view.hpp>
#include <pixie/pixie16/sim.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-crate.hpp>
#include <omnitool-module.hpp>
#include <omnitool-qa.hpp>

#include <args/args.hxx>

using firmware_sets = std::map<std::string, xia::pixie::firmware::firmware_set>;

static void firmware_line_parse(
    firmware_sets& fw_sets, std::string& line, const char delimiter) {
    xia::pixie::firmware::release_type release;
    auto fw = xia::pixie::firmware::parse(release, line, delimiter);
    auto fw_tag = fw.tag;
    if (fw_sets.find(fw_tag) == std::end(fw_sets)) {
        fw_sets.emplace(fw_tag, xia::pixie::firmware::firmware_set(release, "undefined"));
    }
    fw_sets[fw_tag].add(fw);
}

static void load_crate_firmwares(firmware_sets& fw_sets, const std::string& file) {
    std::ifstream input(file, std::ios::in | std::ios::binary);
    if (!input) {
        throw std::runtime_error(std::string("crate firmware file open: ") + file + ": " +
                                 std::strerror(errno));
    }
    for (std::string line; std::getline(input, line);) {
        if (!line.empty()) {
            firmware_line_parse(fw_sets, line, ',');
        }
    }
}

static void firmwares_add(xia::pixie::firmware::system& fws, firmware_sets& fw_sets) {
    for (auto& fw_set : fw_sets) {
        xia::pixie::firmware::add(fws, fw_set.second);
    }
}

static void firmware_load(xia::omnitool::command::context& context) {
    auto& crate = context.crate;
    auto& opts = context.opts;
    /*
     * Load command line first, then crate files and then fill in any
     * gaps with the host files.
     */
    firmware_sets fw_sets;
    if (!opts.firmware_files.empty()) {
        for (auto& fwfile : opts.firmware_files) {
            firmware_line_parse(fw_sets, fwfile,  ',');
        }
    }
    if (!opts.firmware_crate_files.empty()) {
        for (auto& fwfile : opts.firmware_crate_files) {
            load_crate_firmwares(fw_sets, fwfile);
        }
    }
    if (!fw_sets.empty()) {
        firmwares_add(crate->firmware, fw_sets);
    }
    if (!opts.firmware_host_path.empty()) {
        load_firmwares(crate->firmware, opts.firmware_host_path);
    } else {
        xia::pixie::firmware::load_system_firmwares(crate->firmware);
    }
}

static void status(xia::omnitool::command::context& context) {
    auto& crate = context.crate;
    auto& opts = context.opts;
    std::ostringstream oss;
    oss << "s:" << crate->num_slots
        << " p:" << crate->num_present
        << " +" << crate->num_online
        << " -" << crate->num_offline;
    opts.status = oss.str();
}

static void initialize(xia::omnitool::command::context& context) {
    auto& crate = context.crate;
    auto& opts = context.opts;
    xia::util::time::timepoint tp;
    if (opts.verbose) {
        opts.out << "crate: initialize" << std::endl;
        tp.start();
    }
    crate->initialize(opts.reg_trace);
    if (opts.verbose) {
        tp.end();
        opts.out << "modules: detected=" << crate.get_num_modules()
                 << " time=" << tp << std::endl;
    }
    if (!opts.slots.empty() || !opts.excluded_slots.empty()) {
        auto slots = crate.get_modules();
        if (!opts.slots.empty()) {
            slots = opts.slots;
        }
        xia::pixie::module::number_slots slot_map;
        for (auto s : slots) {
            auto exclude = std::find(opts.excluded_slots.begin(), opts.excluded_slots.end(), s);
            if (exclude == opts.excluded_slots.end()) {
                slot_map.emplace_back(std::make_pair(int(slot_map.size()), s));
            }
        }
        crate.assign(slot_map);
    }
}

static void probe(xia::omnitool::command::context& context) {
    auto& crate = context.crate;
    auto& opts = context.opts;
    crate->probe();
    if (opts.verbose) {
        opts.out << "modules: slots=" << crate->num_slots
                 << " present=" << crate->num_present
                 << " online=" << crate->num_online
                 << " offline=" << crate->num_offline << std::endl
                 << "module: assignment map: " << crate.format_module_map()
                 << std::endl;
    }
}

static void help_output(std::ostream& out) {
    auto ci = xia::omnitool::command::find_command("/util/help");
    if (xia::omnitool::command::valid_command(ci)) {
        const xia::omnitool::command::definition& cmd = *ci;
        out << "  COMMANDS:" << std::endl;
        out << "      " << cmd.name << " - " << cmd.help << std::endl
            << "        eg '-- help -l'" << std::endl
            << std::endl;
    } else {
        out << "  No help found; please report to XIA" << std::endl;
    }
}

using args_parser = args::ArgumentParser;
using args_group = args::Group;
using args_help_flag = args::HelpFlag;
using args_flag = args::Flag;
using args_int_flag = args::ValueFlag<int>;
using args_size_flag = args::ValueFlag<size_t>;
using args_string_flag = args::ValueFlag<std::string>;
using args_strings_flag = args::ValueFlagList<std::string>;
using args_positional_list = args::PositionalList<xia::omnitool::command::argument>;

int main(int argc, char* argv[]) {
    xia::omnitool::crate::register_commands();
    xia::omnitool::module::register_commands();
    xia::omnitool::qa::register_commands();

    args_parser parser("Pixie-16 OmniTool ");

    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args_group option_group(parser, "Options");
    args_help_flag help(option_group, "help", "Display this help menu", {'h', "help"});
    args_flag quiet_flag(
        option_group, "quiet_flag", "Quiet output", {'q', "quiet"},
        false);
    args_flag throw_unhandled_flag(
        option_group, "throw_unhandled_flag",
        "Throw an unhandled exception, it will detail the exception",
        {'t', "throw-unhandled"}, false);
    args_flag reg_trace(
        option_group, "reg_trace", "Registers debugging traces in the API.",
        {'R', "reg-trace"});
    args_flag simulate(
        option_group, "simulate", "Simulate the crate and modules",
        {'S', "simulate"}, false);
    args_string_flag module_defs(
        option_group, "module_file_flag",
        "Crate simulation module definition file to load. "
        "The file contains the module to simulate.",
        {'M', "modules"});
    args_string_flag fw_host_path_flag(
        option_group, "fw_host_path_flag",
        "Path to the host installed firmware.",
        {'H', "hostpath"});
    args_strings_flag fw_file_flag(
        option_group, "fw_file_flag",
        "Firmware file(s) to load. Can be repeated. "
        "Takes the form version,revision,adc-msps,adc-bits,device,mask,file "
        "Ex. version=r33339,revision=15,adc-msps=250,adc-bits=14,device=sys,"
        "mask=0x0000000F,file=syspixie16_revfgeneral_adc250mhz_r33339.bin",
        {'F', "firmware"});
    args_strings_flag crate_file_flag(
        option_group, "crate_file_flag",
        "Crate firmware file to load. "
        "The file contain the list of firmware files.",
        {'C', "crate"});
    args_string_flag log_file_flag(
        option_group, "log_file_flag",
        "Operational log file. Use `stdout` for the console.", {'l', "log"});
    args_string_flag log_level_flag(
        option_group, "log_level_flag", "Set the logging level (off, error, warning, info, debug)",
        {'L', "log-level"});
    args_string_flag out_file_flag(
        option_group, "out_file_flag",
        "Output file. Default is the console.", {'o', "output"});
    args_strings_flag slot_map_flag(
        option_group, "slot_map_flag",
        "A list of slots used to define the slot to index mapping. "
        " Use this option to limit slots available to use.",
        {'s', "slot_map"});
    args_strings_flag slot_exclude_map_flag(
        option_group, "slot_exclude_map_flag",
        "A list of slots to exclude from being used. "
        " Use this option to limit slots available to use by excluding some.",
        {'X', "slot_exclude_map"});
    args_string_flag cmd_file_flag(
        option_group, "cmd_file_flag",
        "File of commands to execute.", {'c', "cmd"});
    args_string_flag cmd_path(
        option_group, "cmd_path",
        "Command search path (':' separated) prepended to the default path, default: '/util'.",
        {'p', "path"});
    args_flag no_execute(
        option_group, "no_execute", "Parse the command but do not execute.",
        {'n', "no-execute"});
    args_flag logical_map_flag(
        option_group, "logical_map_flag", "Use a logical mapping instead of slot mapping.",
        {'m', "logical-map"}, false);

    args_group command_group(parser, "Commands");
    args_positional_list args_cmds(
        command_group, "commands",
        "Commands to be performed in order. Prefix with '--'. "
        "The command 'help' lists the available commands.");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << help.what() << std::endl;
        std::cout << parser;
        help_output(std::cout);
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << e.what() << std::endl;
        std::cout << parser;
        help_output(std::cout);
        return EXIT_FAILURE;
    }

    try {
        xia::util::time::timepoint run;
        run.start();

        std::string log;
        if (log_file_flag) {
            log = args::get(log_file_flag);
        } else {
            log = "pixie16-omnitool.log";
        }

        auto log_level = xia::log::info;
        if (log_level_flag) {
            std::string level = args::get(log_level_flag);
            if (level == "off") {
                log_level = xia::log::off;
            } else if  (level == "error") {
                log_level = xia::log::error;
            } else if  (level == "warning") {
                log_level = xia::log::warning;
            } else if  (level == "info") {
                log_level = xia::log::info;
            } else if  (level == "debug") {
                log_level = xia::log::debug;
            } else {
                throw std::runtime_error("invalid log level: " + level);
            }
        }
        xia::logging::start("log", log, false);
        xia::logging::set_level(log_level);

        std::ostream* out = &std::cout;
        std::ofstream output;
        if (out_file_flag) {
            std::string outfile = args::get(out_file_flag);
            output.open(outfile);
            if (!output) {
                throw std::runtime_error("cannot open: " + outfile);
            }
            out = &output;
        }

        xia::omnitool::command::session_options session_opts(*out);
        session_opts.verbose = !args::get(quiet_flag);
        session_opts.reg_trace = args::get(reg_trace);

        /*
         * The physical crates. Let the options select one.
         */
        xia::pixie::crate::crate crate_hw;
        xia::pixie::sim::crate crate_sim;
        xia::pixie::crate::crate* crate_selection = &crate_hw;

        if (simulate) {
            if (!module_defs) {
                throw std::runtime_error("simulation requires a module definition");
            }
            xia_log(xia::log::info) << "simulation: " << args::get(module_defs);
            xia::pixie::sim::load_module_defs(args::get(module_defs));
            crate_selection = &crate_sim;
        }

        /*
         * The logical module crate
         */
        xia::pixie::crate::view::module mod_crate(*crate_selection);
        xia::omnitool::command::omnitool_crate crate(mod_crate, logical_map_flag);

        if (fw_file_flag) {
            for (const auto& fw : args::get(fw_file_flag)) {
                session_opts.firmware_files.push_back(fw);
            }
        }

        if (crate_file_flag) {
            for (const auto& fw : args::get(crate_file_flag)) {
                session_opts.firmware_crate_files.push_back(fw);
            }
        }

        if (slot_map_flag) {
            for (const auto& slot : args::get(slot_map_flag)) {
                xia::omnitool::command::slot_range ss =
                    xia::util::io::get_values<size_t>(slot, xia::pixie::hw::max_slots);
                session_opts.slots.insert(
                    std::end(session_opts.slots), std::begin(ss), std::end(ss));
            }
        }

        if (slot_exclude_map_flag) {
            for (const auto& slot : args::get(slot_exclude_map_flag)) {
                xia::omnitool::command::slot_range ss =
                    xia::util::io::get_values<size_t>(slot, xia::pixie::hw::max_slots);
                session_opts.excluded_slots.insert(
                    std::end(session_opts.excluded_slots), std::begin(ss), std::end(ss));
            }
        }

        if (fw_host_path_flag) {
            session_opts.firmware_host_path = args::get(fw_host_path_flag);
        }

        xia::omnitool::command::arguments cmds;
        for (auto& cmd : args_cmds) {
            cmds.push_back(cmd);
        }
        if (cmd_file_flag) {
            cmds.push_back("script");
            cmds.push_back(args::get(cmd_file_flag));
        }

        /*
         * Default path is /util and users can prepend a path, for example:
         *   /crate:/module
         */
        session_opts.path = {"/util"};
        if (cmd_path) {
            xia::util::string::strings user_path;
            xia::util::string::split(user_path, args::get(cmd_path), ':');
            session_opts.path.insert(
                session_opts.path.begin(), user_path.begin(), user_path.end());
        }

        session_opts.ops.set("status", [](xia::omnitool::command::context& context) {
            status(context);
        }, false);
        session_opts.ops.set("init", [](xia::omnitool::command::context& context) {
            firmware_load(context);
            initialize(context);
        }, true);
        session_opts.ops.set("probe", [](xia::omnitool::command::context& context) {
            probe(context);
        }, true);

        /*
         * Execute the command or just list them for debugging.
         */
        bool execute = !args::get(no_execute);

        /*
         * By default, run the shell if the user passes no options.
         */
        if (cmds.empty()) {
            cmds.push_back("shell");
        }

        /*
         * Execute the commands as a batch.
         */
        xia::omnitool::command::batch batch;
        batch.parse(cmds, session_opts.path);
        if (execute) {
            batch.execute(crate, session_opts);
        } else {
            batch.report(*out);
        }

        run.end();
        if (session_opts.verbose) {
            *out << "run time=" << run << std::endl;
        }
    } catch (xia::pixie::error::error& e) {
        xia_log(xia::log::error) << e;
        std::cerr << e << std::endl;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << e.what();
        std::cerr << "error: " << e.what() << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (args::get(throw_unhandled_flag)) {
            throw;
        }
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        std::cerr << "error: unknown error: unhandled exception" << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    }

    return EXIT_SUCCESS;
}
