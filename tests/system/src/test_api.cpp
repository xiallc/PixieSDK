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

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::ValueFlag<size_t> num_modules_flag(arguments, "num_modules_flag",
                                             "Number of modules to report",
                                             {'n', "num-modules"}, 0);
    args::ValueFlagList<std::string> fw_file_flag(arguments, "fw_file_flag",
                                                  "Firmware file(s) to load. Can be repeated. "
                                                  "Takes the form rev:rev-num:type:name"
                                                  "Ex. r33339:15:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin",
                                                  {'F', "firmware"});
    args::ValueFlagList<std::string> crate_file_flag(arguments, "crate_file_flag",
                                                     "Crate firmware file to load. "
                                                     "The contents is are firmware files.",
                                                     {'C', "crate"});
    args::ValueFlag<std::string> log_file_flag(arguments, "log_file_flag",
                                               "Log file. Use `stdout` for the console.",
                                               {'l', "log"});
    args::Flag reg_trace(arguments, "reg_trace",
                         "Registers debugging traces in the API.", {'R', "reg-trace"});

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

        xia::pixie::logging::start("log", log, xia::pixie::log::debug, false);

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
        crate.initialize(num_modules, reg_trace);
        std::cout << "Modules found: " << crate.modules.size()
                  << std::endl;
        crate.firmware = firmwares;
        crate.set_firmware();
        crate.boot();
        std::cout << "Crate:" << std::endl << crate << std::endl;
    } catch (std::runtime_error& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
