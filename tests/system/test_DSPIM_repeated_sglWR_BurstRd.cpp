/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2020 XIA LLC
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
#include "args.hxx"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "configuration.hpp"

#include <iostream>
#include <cstdio>
#include <string>

#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

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
    char ErrMSG[256];
    int retval = 0;
    unsigned int errorcounts, rd_wr_errors;
    unsigned int rd_data[65536], wr_data[65536];
    unsigned int count;
    unsigned short modnum, numtries;
    static const int offline_mode = 0;

    args::ArgumentParser parser("System test code ");
    parser.LongSeparator("=");
    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::Positional<std::string> configuration(arguments, "cfg", "The configuration file to load.",
                                                args::Options::Required);
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});

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


    cout << "INFO - Calling Pixie16InitSystem.......";
    if (!verify_api_return_value(Pixie16InitSystem(cfg.numModules, cfg.slot_map, offline_mode),
                                 "Pixie16InitSystem"))
        return EXIT_FAILURE;


    unsigned short boot_pattern = 0x7F;

    cout << "INFO - Calling Pixie16BootModule with boot pattern: " << showbase << hex
         << boot_pattern << dec
         << "............" << endl;
    if (!verify_api_return_value(
            Pixie16BootModule(cfg.ComFPGAConfigFile.c_str(), cfg.SPFPGAConfigFile.c_str(),
                              cfg.TrigFPGAConfigFile.c_str(), cfg.DSPCodeFile.c_str(),
                              cfg.DSPParFile.c_str(), cfg.DSPVarFile.c_str(), cfg.numModules,
                              boot_pattern),
            "Pixie16BootModule", ""))
        return EXIT_FAILURE;

    errorcounts = 0;
    for (unsigned int loop_num = 0; loop_num < 100; loop_num++) {
        for (numtries = 0; numtries < 4; numtries++) {
            for (count = 0; count < 65536; count += 2) {
                if (numtries == 0)
                    wr_data[count] = 0xAAAA5555;
                else if (numtries == 1)
                    wr_data[count] = 0xA0500A05;
                else if (numtries == 2)
                    wr_data[count] = count;
                else
                    wr_data[count] = 65536 - count;
            }
            for (count = 1; count < 65536; count += 2) {
                if (numtries == 0)
                    wr_data[count] = 0x5555AAAA;
                else if (numtries == 1)
                    wr_data[count] = 0x50A0050A;
                else if (numtries == 2)
                    wr_data[count] = count;
                else
                    wr_data[count] = 65536 - count;
            }

            for (modnum = 0; modnum < cfg.numModules; modnum++) {
                retval = Pixie_DSP_Memory_IO(wr_data, 0x50000, 65535, MOD_WRITE, modnum);
                if (retval < 0) {
                    sprintf(ErrMSG,
                            "*ERROR* Pixie16_DSP_Memory_IO WRITE failed in module %d, retval = %d",
                            modnum, retval);
                    Pixie_Print_MSG(ErrMSG);
                }

                retval = Pixie_DSP_Memory_IO(rd_data, 0x50000, 65535, MOD_READ, modnum);
                if (retval < 0) {
                    sprintf(ErrMSG,
                            "*ERROR* Pixie16_DSP_Memory_IO READ failed in module %d, retval = %d",
                            modnum, retval);
                    Pixie_Print_MSG(ErrMSG);
                    return (-1);
                }

                rd_wr_errors = 0;

                for (count = 0; count < 65535; count++) {
                    if (wr_data[count] != rd_data[count]) {
                        sprintf(ErrMSG,
                                "RD_WR data mismatch in module %d, rd_data=0x%x, wr_data=0x%x, count=%d",
                                modnum, rd_data[count], wr_data[count], count);
                        Pixie_Print_MSG(ErrMSG);
                        rd_wr_errors++;
//						return 2;
                    }
                }
                if (rd_wr_errors > 0) {
                    sprintf(ErrMSG, "RD_WR data mismatch in module %d, rd_wr_errors=%d", modnum,
                            rd_wr_errors);
                    Pixie_Print_MSG(ErrMSG);
                    printf("RD_WR data mismatch in module %d, rd_wr_errors=%d\n", modnum,
                           rd_wr_errors);

                }
                errorcounts += rd_wr_errors;
            }
        }
        printf("Number of DSP write & read tests = %d, error counts = %d\n", loop_num, errorcounts);
        usleep(100000);
    }
}
