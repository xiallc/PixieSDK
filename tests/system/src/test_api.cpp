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
#include <sstream>

#include <pixie_crate.hpp>
#include <pixie_log.hpp>

#include "args.hxx"

/*
 * Localize the log.
 */
typedef xia::pixie::log log;

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

    args::Group options(parser, "Options");
    args::HelpFlag
        help(options,
             "help",
             "Display this help menu",
             {'h', "help"});
    args::Flag
        debug_flag(options,
                   "debug_flag",
                   "Enable debug log level",
                   {'d', "debug"}, false);
    args::Flag
        throw_unhandled_flag(options,
                             "throw_unhandled_flag",
                             "Throw an unhandled exception, it will detail the exception",
                             {'t', "throw-unhandled"}, false);
    args::Flag
        reg_trace(options,
                  "reg_trace",
                  "Registers debugging traces in the API.", {'R', "reg-trace"});
    args::ValueFlag<size_t>
        num_modules_flag(options,
                         "num_modules_flag",
                         "Number of modules to report",
                         {'n', "num-modules"}, 0);
    args::ValueFlagList<std::string>
        fw_file_flag(options,
                     "fw_file_flag",
                     "Firmware file(s) to load. Can be repeated. "
                     "Takes the form rev:rev-num:type:name"
                     "Ex. r33339:15:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin",
                     {'F', "firmware"});
    args::ValueFlagList<std::string>
        crate_file_flag(options,
                        "crate_file_flag",
                        "Crate firmware file to load. "
                        "The contents is are firmware files.",
                        {'C', "crate"});
    args::ValueFlag<std::string>
        log_file_flag(options,
                      "log_file_flag",
                      "Log file. Use `stdout` for the console.",
                      {'l', "log"});

    args::Group commands(parser, "Commands");
    args::PositionalList<std::string>
        cmd_flag(commands,
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

        xia::pixie::firmware::crate firmwares;
        size_t num_modules = args::get(num_modules_flag);

        if (fw_file_flag) {
            for (const auto& firmware: args::get(fw_file_flag)) {
                auto fw = xia::pixie::firmware::parse(firmware, ':');
                if (xia::pixie::firmware::check(firmwares, fw)) {
                    std::string what("duplicate firmware: ");
                    what += firmware;
                    throw std::runtime_error(what);
                }
                xia::pixie::firmware::add(firmwares, fw);
            }
        }

        if (crate_file_flag) {
            for (const auto& firmware: args::get(crate_file_flag)) {
                load_crate_firmware(firmware, firmwares);
            }
        }

        xia::pixie::crate::crate crate;

        std::cout << "detecting modules" << std::endl;
        crate.initialize(num_modules, reg_trace);
        std::cout << "modules found: " << crate.modules.size()
                  << std::endl;

        if (cmd_flag) {
            for (auto& cmd : args::get(cmd_flag)) {
                if (cmd == "boot") {
                    std::cout << "booting crate" << std::endl;
                    crate.firmware = firmwares;
                    crate.set_firmware();
                    crate.boot();
                    std::cout << "crate:" << std::endl
                              << crate << std::endl;
                } else {
                    std::cerr << "error: invalid command: " << cmd
                              << std::endl;
                    return EXIT_FAILURE;
                }
            }
        } else {
            std::cout << "no commands; finishd" << std::endl;
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
