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

#include <getopt.h>

#include <pixie_crate.hpp>

#include "PlxApi.h"
#include "pixie16sys_common.h"
#include "pixie16sys_defs.h"
#include "pixie16sys_export.h"
#include "pixie16sys_globals.h"

static struct option opts[] = {
  { "help",        no_argument,            NULL,           'h' },
  { "num-modules", required_argument,      NULL,           'n' },
  { "bitfile",     required_argument,      NULL,           'B' },
  { "csys",        no_argument,            NULL,           'S' },
  { "reg-trace",   no_argument,            NULL,           'R' },
  { NULL,          0,                      NULL,            0 }
};

void
usage (int exit_code)
{
  std::cout << "pixie-modules [options]" << std::endl
            << "Options and arguments:" << std::endl
            << " -h          : help (also --help)" << std::endl
            << " -n num      : number of modules to report" << std::endl
            << " -B rev:name : bitfile (also --bitfile)" << std::endl
            << " -S          : use C sys API" << std::endl
            << " -R          : register trace" << std::endl;
  ::exit (exit_code);
}

int
main(int argc, char* argv[])
{
    xia::pixie::firmware::crate firmwares;
    size_t num_modules = 0;
    bool csys_api = false;
    bool reg_trace = false;

    try {
        while (true)
        {
            int opt = ::getopt_long(argc, argv, "hn:B:SR", opts, NULL);
            if (opt < 0)
                break;

            switch (opt)
            {
            case 'n':
            {
                std::istringstream iss(optarg);
                iss >> num_modules;
                break;
            }
            case 'B':
            {
                auto fw = xia::pixie::firmware::parse(optarg, ':');
                if (xia::pixie::firmware::check(firmwares, fw)) {
                  std::string what("duplicate bitfile option: ");
                  what += optarg;
                  throw std::runtime_error(what);
                }
                xia::pixie::firmware::add(firmwares, fw);
                break;
            }
            case 'S':
                csys_api = true;
                break;
            case 'R':
                reg_trace = true;
                break;
            case '?':
                return 2;
            case 'h':
                usage (0);
                break;
            }
        }

        if (csys_api) {
            unsigned short mod_map[num_modules];
            for (size_t m = 0; m < num_modules; ++m)
                mod_map[m] = m;
            std:: cout << "init-system: "
                       << Pixie_InitSystem(num_modules, mod_map, 0)
                       << std::endl;
            for (size_t m = 0; m < num_modules; ++m)
                std::cout << "module close:  " << m
                          << ": " << Pixie_ClosePCIDevices(m) << std::endl;
        } else {
            xia::pixie::crate::crate crate(num_modules);
            crate.initialize(reg_trace);
            std::cout << "Modules found: " << crate.modules.size()
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
