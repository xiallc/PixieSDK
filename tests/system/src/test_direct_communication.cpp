/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
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
/// @file test_direct_communication.cpp
/// @brief Used to test direct communication with memory registers on the system.
/// @author S. V. Paulauskas
/// @date February 19, 2021
#include "args.hxx"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "configuration.hpp"

#include <array>
#include <iostream>
#include <string>
#include <thread>

#include <cstdio>


#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

enum class DATA_PATTERN {
    HI_LO,
    FLIP_FLOP,
    RAMP_UP,
    RAMP_DOWN,
    CONSTANT,
    EVEN_BITS,
    ODD_BITS,
    ZERO
};

std::array<unsigned int, 65536> prepare_data_to_write(const DATA_PATTERN& data_pattern) {
    std::array<unsigned int, 65536> data{};
    for (unsigned int entry = 0; entry < 65536; entry += 2) {
        switch (data_pattern) {
            case DATA_PATTERN::HI_LO:
                data[entry] = 0xAAAA5555;
                data[entry + 1] = 0x5555AAAA;
                break;
            case DATA_PATTERN::FLIP_FLOP:
                data[entry] = 0xA0500A05;
                data[entry + 1] = 0x50A0050A;
                break;
            case DATA_PATTERN::RAMP_UP:
                data[entry] = entry;
                data[entry + 1] = entry + 1;
                break;
            case DATA_PATTERN::RAMP_DOWN:
                data[entry] = 65536 - entry;
                data[entry + 1] = 65536 - entry - 1;
                break;
            case DATA_PATTERN::CONSTANT:
                data[entry] = 0x50f750fa;
                data[entry + 1] = 0x50f750fa;
                break;
            case DATA_PATTERN::EVEN_BITS:
                data[entry] = 0xA5A5A5A5;
                data[entry + 1] = 0xA5A5A5A5;
                break;
            case DATA_PATTERN::ODD_BITS:
                data[entry] = 0x5A5A5A5A;
                data[entry + 1] = 0x5A5A5A5A;
                break;
            case DATA_PATTERN::ZERO:
                data[entry] = 0;
                data[entry + 1] = 0;
                break;
            default:
                break;
        }
    }
    return data;
}

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const std::string& okmsg = "OK") {
    if (val < 0) {
        cerr << endl << "ERROR - " << func_name << " with Error Code " << val << endl;
        return false;
    }
    if (!okmsg.empty())
        cout << okmsg << endl;
    return true;
}


int main(int argc, char* argv[]) {
    args::ArgumentParser parser("Tests direct communication with the system.");
    parser.LongSeparator("=");

//    args::Group commands(parser, "commands");
//    args::Command dsp(commands, "dsp", "Tests related to the DSP.");
//    args::Command external_memory(commands, "external_memory",
//                                  "Tests related to the external memory.");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::Positional<std::string> configuration(arguments, "cfg", "The configuration file to load.",
                                                args::Options::Required);
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});

    args::ValueFlag<std::string> boot_pattern_flag(arguments, "boot_pattern",
                                              "The boot pattern we'll use when booting up the system.",
                                              {'b', "boot_pattern"}, "0x7F");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        cout << parser;
        return EXIT_SUCCESS;
    } catch (args::ValidationError& e) {
        cerr << e.what() << endl;
        cout << parser;
        return EXIT_FAILURE;
    }

    xia::configuration::Configuration cfg;
    try {
        cfg = xia::configuration::read_configuration_file(configuration.Get());
    } catch (invalid_argument& invalidArgument) {
        cerr << invalidArgument.what() << endl;
        return EXIT_FAILURE;
    }


    static const int offline_mode = 0;
    cout << "INFO - Calling Pixie16InitSystem.......";
    if (!verify_api_return_value(Pixie16InitSystem(cfg.numModules, cfg.slot_map, offline_mode),
                                 "Pixie16InitSystem"))
        return EXIT_FAILURE;

    unsigned int boot_pattern = stoul(args::get(boot_pattern_flag), nullptr, 0);
    cout << "INFO - Calling Pixie16BootModule with boot pattern: " << showbase << hex
         << boot_pattern << dec
         << "............" << endl;

    if (!verify_api_return_value(
            Pixie16BootModule(cfg.ComFPGAConfigFile.c_str(), cfg.SPFPGAConfigFile.c_str(),
                              cfg.TrigFPGAConfigFile.c_str(), cfg.DSPCodeFile.c_str(),
                              cfg.DSPParFile.c_str(), cfg.DSPVarFile.c_str(), cfg.numModules,
                              boot_pattern),
            "Pixie16BootModule", "INFO - Finished booting!"))
        return EXIT_FAILURE;
}
