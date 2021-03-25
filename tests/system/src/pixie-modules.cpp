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
#include <iostream>
#include <sstream>

#include <pixie_crate.hpp>

#include "args.hxx"
#include "PlxApi.h"
#include "pixie16sys_common.h"
#include "pixie16sys_defs.h"
#include "pixie16sys_export.h"
#include "pixie16sys_globals.h"

int
main(int argc, char* argv[])
{
    args::ArgumentParser parser("Tests module initialization");
    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::ValueFlag<size_t> num_modules_flag(arguments, "num_modules_flag",
                                             "Number of modules to report", {'n', "num-modules"}, 1);
    args::ValueFlag<std::string> bit_file_flag(arguments, "bit_file_flag",
                                               "Bit file to load in the form. Ex. F:15:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin",
                                               {'B', "bitfile"});
    args::Flag csys_api(arguments, "csys_api", "Use the Legacy C API", {'S', "csys_api"});
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

    xia::pixie::firmware::crate firmwares;
    size_t num_modules = args::get(num_modules_flag);

    try {
        if(bit_file_flag) {
            auto fw = xia::pixie::firmware::parse(args::get(bit_file_flag), ':');
            if (xia::pixie::firmware::check(firmwares, fw)) {
                std::string what("duplicate bitfile option: ");
                what += args::get(bit_file_flag);
                throw std::runtime_error(what);
            }
            xia::pixie::firmware::add(firmwares, fw);
        }

        if (csys_api) {
            std::vector<unsigned short> mod_map;
            for (size_t m = 0; m < num_modules; ++m)
                mod_map.emplace_back((unsigned short)m);
            std:: cout << "init-system: "
                       << Pixie_InitSystem((unsigned short)num_modules, mod_map.data(), 0)
                       << std::endl;
            for (size_t m = 0; m < num_modules; ++m)
                std::cout << "module close:  " << m
                          << ": " << Pixie_ClosePCIDevices((unsigned short)m) << std::endl;
        } else {
            xia::pixie::crate::crate crate(num_modules);
            crate.initialize(reg_trace);
            std::cout << "Total Modules found: " << crate.modules.size()
                      << std::endl;
            crate.set(firmwares);
            std::cout << "Crate:" << std::endl
                      << crate << std::endl;
            crate.boot();
        }
    } catch (std::runtime_error& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
