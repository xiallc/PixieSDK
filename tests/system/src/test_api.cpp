/*----------------------------------------------------------------------
* Copyright (c) 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------*/

#include <algorithm>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include <pixie_crate.hpp>
#include <pixie_log.hpp>
#include <pixie_util.hpp>

#include "simulation.hpp"

#include "args.hxx"

/*
 * Localize the log and error
 */
typedef xia::log xia_log;
typedef xia::pixie::error::error error;

/*
 * Command processor.
 */
typedef std::vector<std::string> options;
typedef std::vector<options> commands;

struct command_def
{
    std::vector<size_t> counts;
    std::string help;
};

static const std::map<std::string, command_def> command_defs =
{
    { "boot",      { { 0 },       "boots the module(s)" } },
    { "par-read",  { { 2, 3 },    "read module/channel parameter" } },
    { "par-write", { { 3, 4 },    "write module/channel parameter" } },
    { "var-read",  { { 2, 3, 4 }, "read module/channel variable" } },
    { "var-write", { { 3, 4, 5 }, "write module/channel variable" } }
};

static void
check_number(const std::string& opt)
{
    if (!opt.empty()) {
        auto it = opt.begin();
        if (*it == '-' || std::isdigit(*it)) {
            ++it;
            while (it != opt.end() && std::isdigit(*it)) {
                ++it;
            }
            if (it == opt.end()) {
                return;
            }
        }
    }
    throw error(error::code::invalid_value, "not a number");
}

template<typename T> static T
get_value(const std::string& opt)
{
    check_number(opt);
    std::istringstream iss(opt);
    T value;
    iss >> value;
    return value;
}

template<typename V>
static void
output_value(const std::string& name, V value)
{
    xia::util::ostream_guard oguard(std::cout);
    std::cout << name << " = " << value;
    if (!std::is_same<V, double>::value) {
        std::cout << std::hex << " (0x" << value << ')';
    }
    std::cout << std::endl;
}

static bool
make_command_sets(args::PositionalList<std::string>& cmd, commands& cmds)
{
    options option;
    for (auto opt : args::get(cmd)) {
        if (option.empty()) {
            auto search = command_defs.find(opt);
            if (search == command_defs.end()) {
                std::cerr << "error: invalid command: " << opt
                          << std::endl;
                return false;
            }
        }
        option.push_back(opt);
        auto search = command_defs.find(option[0]);
        auto counts = search->second.counts;
        auto min = std::min_element(counts.begin(), counts.end());
        auto max = std::max_element(counts.begin(), counts.end());
        if (*min != *max && option.size() > *min) {
            search = command_defs.find(opt);
            if (search != command_defs.end()) {
                if (option.size() > 1) {
                    option.pop_back();
                }
                cmds.push_back(option);
                option.clear();
                option.push_back(opt);
            }
        }
        if (option.size() > *max) {
          cmds.push_back(option);
          option.clear();
        }
    }
    if (!option.empty()) {
        auto search = command_defs.find(option[0]);
        auto counts = search->second.counts;
        auto min = std::min_element(counts.begin(), counts.end());
        if (option.size() <= *min) {
            std::cerr << "error: malformed command:";
            for (auto& opt : option) {
                std::cerr << ' ' << opt;
            }
            std::cerr << std::endl;
            return false;
        }
        cmds.push_back(option);
    }
    return true;
}

static void
par_write(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    if (cmd.size() == 4) {
        auto value =
            get_value<xia::pixie::param::value_type>(cmd[3]);
        crate[mod_num].write(cmd[2], value);
    } else {
        auto channel_num = get_value<size_t>(cmd[2]);
        auto value = get_value<double>(cmd[4]);
        crate[mod_num].write(cmd[3], channel_num, value);
    }
}

static void
par_read(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    int reg_name;
    if (cmd.size() == 3) {
        reg_name = 2;
        std::cout << "# module param read: " << mod_num
                  << ": " << cmd[reg_name] << std::endl;
        if (cmd[reg_name] == "all") {
            for (auto& par : xia::pixie::param::get_module_param_map()) {
                try {
                    output_value(par.first, crate[mod_num].read(par.second));
                } catch (error& e) {
                    if (e.type != error::code::module_param_disabled &&
                        e.type != error::code::module_param_writeonly) {
                        throw;
                    }
                }
            }
        } else {
            output_value(cmd[reg_name], crate[mod_num].read(cmd[reg_name]));
        }
    } else {
        reg_name = 3;
        auto channel_num = get_value<size_t>(cmd[2]);
        std::cout << "# channel param read: " << mod_num << ':' << channel_num
                  << ": " << cmd[reg_name] << std::endl;
        if (cmd[reg_name] == "all") {
            for (auto& par : xia::pixie::param::get_channel_param_map()) {
                try {
                    output_value(par.first,
                                 crate[mod_num].read(par.second, channel_num));
                } catch (error& e) {
                    if (e.type != error::code::channel_param_disabled &&
                        e.type != error::code::channel_param_writeonly) {
                        throw;
                    }
                }
            }
        } else {
            output_value(cmd[reg_name],
                         crate[mod_num].read(cmd[reg_name], channel_num));
        }
    }
}

static void
var_write(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    int reg_name;
    size_t channel = 0;
    size_t offset = 0;
    int val;
    switch (cmd.size()) {
    case 4:
        reg_name = 2;
        val = 3;
        break;
    case 5:
        channel = get_value<size_t>(cmd[2]);
        reg_name = 3;
        val = 4;
        break;
    case 6:
    default:
        channel = get_value<size_t>(cmd[2]);
        reg_name = 3;
        val = 4;
        offset = get_value<size_t>(cmd[5]);
        break;
    }
    auto value = get_value<xia::pixie::param::value_type>(cmd[val]);
    crate[mod_num].write_var(cmd[reg_name], channel, value, offset);
}

static void
var_read(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    bool is_module = false;
    int reg_name;
    size_t channel = 0;
    size_t offset = 0;
    switch (cmd.size()) {
    case 3:
        is_module = true;
        reg_name = 2;
        break;
    case 4:
        channel = get_value<size_t>(cmd[2]);
        reg_name = 3;
        break;
    case 5:
    default:
        channel = get_value<size_t>(cmd[2]);
        reg_name = 3;
        offset = get_value<size_t>(cmd[4]);
        break;
    }
    if (cmd[reg_name] == "all") {
       if (is_module) {
           std::cout << "# module var read: " << mod_num
                     << ": " << cmd[reg_name] << std::endl;
            for (auto& var : crate[mod_num].module_var_descriptors) {
                try {
                    output_value(var.name, crate[mod_num].read_var(var.par));
                } catch (error& e) {
                    if (e.type != error::code::module_param_disabled &&
                        e.type != error::code::module_param_writeonly) {
                        throw;
                    }
                }
            }
        } else {
           std::cout << "# channel var read: " << mod_num << ':' << channel
                     << ": " << cmd[reg_name] << std::endl;
            for (auto& var : crate[mod_num].channel_var_descriptors) {
                try {
                    output_value(var.name,
                                 crate[mod_num].read_var(var.par,
                                                         channel,
                                                         offset));
                 } catch (error& e) {
                    if (e.type != error::code::channel_param_disabled &&
                        e.type != error::code::channel_param_writeonly) {
                        throw;
                    }
                }
           }
        }
    } else {
        output_value(cmd[reg_name],
                     crate[mod_num].read_var(cmd[reg_name],
                                             channel,
                                             offset));
    }
}

static bool
process_command_sets(xia::pixie::crate::crate& crate, commands& cmds)
{
    for (auto& cmd : cmds) {
        if (cmd[0] == "boot") {
            std::cout << "booting crate" << std::endl;
            crate.boot();
            std::cout << "crate:" << std::endl << crate << std::endl;
        } else if (cmd[0] == "par-write") {
            par_write(crate, cmd);
        } else if (cmd[0] == "par-read") {
            par_read(crate, cmd);
        } else if (cmd[0] == "var-write") {
            var_write(crate, cmd);
        } else if (cmd[0] == "var-read") {
            var_read(crate, cmd);
        }
    }
    return true;
}

void
load_crate_firmware(const std::string& file,
                    xia::pixie::firmware::crate& firmwares)
{
    std::ifstream input(file, std::ios::in | std::ios::binary);
    if (!input) {
        throw std::runtime_error(
            std::string("crate firmware file open: ") + file +
            ": " + std::strerror(errno)
        );
    }
    for (std::string line; std::getline(input, line); ) {
        if (!line.empty()) {
            auto fw = xia::pixie::firmware::parse(line, ',');
            if (xia::pixie::firmware::check(firmwares, fw)) {
                std::string what("duplicate firmware option: ");
                what += line;
                throw std::runtime_error(what);
            }
            xia::pixie::firmware::add(firmwares, fw);
        }
    }
}

int
main(int argc, char* argv[])
{
    args::ArgumentParser parser("Tests C++ API");

    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args::Group option_group(parser, "Options");
    args::HelpFlag
        help(option_group,
             "help",
             "Display this help menu",
             {'h', "help"});
    args::Flag
        debug_flag(option_group,
                   "debug_flag",
                   "Enable debug log level",
                   {'d', "debug"}, false);
    args::Flag
        throw_unhandled_flag(option_group,
                             "throw_unhandled_flag",
                             "Throw an unhandled exception, it will detail the exception",
                             {'t', "throw-unhandled"}, false);
    args::Flag
        reg_trace(option_group,
                  "reg_trace",
                  "Registers debugging traces in the API.", {'R', "reg-trace"});
    args::Flag
        simulate(option_group,
                 "simulate",
                 "Simulate the crate and modules",
                 {'S', "simulate"}, false);
    args::ValueFlag<size_t>
        num_modules_flag(option_group,
                         "num_modules_flag",
                         "Number of modules to report",
                         {'n', "num-modules"}, 0);
    args::ValueFlagList<std::string>
        fw_file_flag(option_group,
                     "fw_file_flag",
                     "Firmware file(s) to load. Can be repeated. "
                     "Takes the form rev:mod-rev-num:type:name"
                     "Ex. r33339:15:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin",
                     {'F', "firmware"});
    args::ValueFlag<std::string>
        module_defs(option_group,
                    "module_file_flag",
                    "Crate simulation module definition file to load. "
                    "The file contains the module to simulate.",
                    {'M', "modules"});
    args::ValueFlagList<std::string>
        crate_file_flag(option_group,
                        "crate_file_flag",
                        "Crate firmware file to load. "
                        "The file contain the list of firmware files.",
                        {'C', "crate"});
    args::ValueFlag<std::string>
        log_file_flag(option_group,
                      "log_file_flag",
                      "Log file. Use `stdout` for the console.",
                      {'l', "log"});
    args::ValueFlagList<size_t>
        slot_map_flag(option_group,
                      "slot_map_flag",
                      "A list of slots used to define the slot to index mapping.",
                      {'s', "slot_map"});

    args::Group command_group(parser, "Commands");
    args::PositionalList<std::string>
        cmd_flag(command_group,
                 "commands",
                 "Commands to be performed in order. "
                 "Commands are 'boot', 'set', 'get'.");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << help.what() << std::endl;
        std::cout << parser;
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << e.what() << std::endl;
        std::cout << parser;
        return EXIT_FAILURE;
    }

    try {
        std::string log;
        if (log_file_flag) {
            log = args::get(log_file_flag);
        } else {
            log = "test-api-log.txt";
        }

        auto log_level = xia_log::info;
        if (args::get(debug_flag)) {
            log_level = xia_log::debug;
        }
        xia::logging::start("log", log, log_level, false);

        size_t num_modules = args::get(num_modules_flag);

        xia::pixie::crate::crate crate_hw;
        xia::pixie::sim::crate crate_sim;

        xia::pixie::crate::crate* crate_selection = &crate_hw;

        if (simulate) {
            if (!module_defs) {
                throw
                    std::runtime_error("simulation requires a module defnition");
            }
            xia_log(xia_log::info) << "simulation: " << args::get(module_defs);
            xia::pixie::sim::load_module_defs(args::get(module_defs));
            crate_selection = &crate_sim;
        }

        xia::pixie::crate::crate& crate = *crate_selection;

        if (fw_file_flag) {
            for (const auto& firmware: args::get(fw_file_flag)) {
                auto fw = xia::pixie::firmware::parse(firmware, ':');
                if (xia::pixie::firmware::check(crate.firmware, fw)) {
                    std::string what("duplicate firmware: ");
                    what += firmware;
                    throw std::runtime_error(what);
                }
                xia::pixie::firmware::add(crate.firmware, fw);
            }
        }

        if (crate_file_flag) {
            for (const auto& firmware: args::get(crate_file_flag)) {
                load_crate_firmware(firmware, crate.firmware);
            }
        }

        std::cout << "detecting modules" << std::endl;
        crate.initialize(num_modules, reg_trace);
        std::cout << "modules found: " << crate.modules.size()
                  << std::endl;

        if (slot_map_flag) {
            xia::pixie::module::number_slots slot_map;
            for (const auto& slot : args::get(slot_map_flag))
                slot_map.emplace_back(std::make_pair(slot_map.size(), slot));
            crate.assign(slot_map);
        }

        crate.set_firmware();
        crate.probe();

        commands cmds;

        if (cmd_flag) {
            if (!make_command_sets(cmd_flag, cmds)) {
                return EXIT_FAILURE;
            }
        }

        if (!process_command_sets(crate, cmds)) {
            return EXIT_FAILURE;
        }
    } catch (xia::pixie::error::error& e) {
        xia_log(xia_log::error) << e;
        std::cerr << e << std::endl;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        std::cerr <<  "error: unknown error: " << e.what() << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (args::get(throw_unhandled_flag)) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        std::cerr <<  "error: unknown error: unhandled exception" << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    }

    return EXIT_SUCCESS;
}
