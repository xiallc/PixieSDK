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

#include "args.hxx"

/*
 * Localize the log and error
 */
typedef xia::pixie::log log;
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
    { "boot", { { 0 },    "boots the modules" } },
    { "get",  { { 2, 3 }, "get mod chan reg" } },
    { "set",  { { 3, 4 }, "set mod chan reg val" }
    }
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

static bool
make_command_sets(args::PositionalList<std::string>& cmd_flag, commands& cmds)
{
    options option;
    for (auto opt : args::get(cmd_flag)) {
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
        for (auto count : counts) {
            if (option.size() == (count + 1)) {
                cmds.push_back(option);
                option.clear();
                break;
            }
        }
    }
    if (!option.empty()) {
        std::cerr << "error: malformed command:";
        for (auto& opt : option) {
            std::cerr << ' ' << opt;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

static bool
process_command_sets(xia::pixie::crate::crate& crate, commands& cmds)
{
    for (auto& cmd : cmds) {
        if (cmd[0] == "boot") {
            std::cout << "booting crate" << std::endl;
            crate.boot();
            std::cout << "crate:" << std::endl
                      << crate << std::endl;
        } else if (cmd[0] == "set") {
            auto mod_num = get_value<size_t>(cmd[1]);
            if (cmd.size() == 4) {
                auto value = get_value<xia::pixie::param::value_type>(cmd[3]);
                crate[mod_num].write(cmd[2], value);
            } else {
                auto channel_num = get_value<size_t>(cmd[2]);
                auto value = get_value<xia::pixie::param::value_type>(cmd[4]);
                crate[mod_num].write(cmd[3], channel_num, value);
            }
        } else if (cmd[0] == "get") {
            xia::pixie::param::value_type value;
            auto mod_num = get_value<size_t>(cmd[1]);
            int reg_name;
            if (cmd.size() == 3) {
                reg_name = 2;
                value = crate[mod_num].read(cmd[reg_name]);
            } else {
                reg_name = 3;
                auto channel_num = get_value<size_t>(cmd[2]);
                value = crate[mod_num].read(cmd[reg_name], channel_num);
            }
            xia::pixie::ostream_guard oguard(std::cout);
            std::cout << cmd[reg_name] << " = " << value
                      << std::hex
                      << " (0x" << value << ')'
                      << std::endl;
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
    args::ValueFlag<size_t>
        num_modules_flag(option_group,
                         "num_modules_flag",
                         "Number of modules to report",
                         {'n', "num-modules"}, 0);
    args::ValueFlagList<std::string>
        fw_file_flag(option_group,
                     "fw_file_flag",
                     "Firmware file(s) to load. Can be repeated. "
                     "Takes the form rev:rev-num:type:name"
                     "Ex. r33339:15:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin",
                     {'F', "firmware"});
    args::ValueFlagList<std::string>
        crate_file_flag(option_group,
                        "crate_file_flag",
                        "Crate firmware file to load. "
                        "The contents is are firmware files.",
                        {'C', "crate"});
    args::ValueFlag<std::string>
        log_file_flag(option_group,
                      "log_file_flag",
                      "Log file. Use `stdout` for the console.",
                      {'l', "log"});

    args::Group command_group(parser, "Commands");
    args::PositionalList<std::string>
        cmd_flag(command_group,
                 "commands",
                 "Commands to be performed in order. "
                 "Commands are 'init', 'boot', 'set', 'get'.");

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

        auto log_level = log::info;
        if (args::get(debug_flag)) {
            log_level = log::debug;
        }
        xia::pixie::logging::start("log", log, log_level, false);

        size_t num_modules = args::get(num_modules_flag);

        xia::pixie::crate::crate crate;

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
        log(log::error) << e;
        std::cerr << e << std::endl;
        return e.return_code();
    } catch (std::exception& e) {
        log(log::error) << "unknown error: " << e.what();
        std::cerr <<  "error: unknown error: " << e.what() << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (args::get(throw_unhandled_flag)) {
            throw;
        }
        log(log::error) << "unknown error: unhandled exception";
        std::cerr <<  "error: unknown error: unhandled exception" << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    }

    return EXIT_SUCCESS;
}
