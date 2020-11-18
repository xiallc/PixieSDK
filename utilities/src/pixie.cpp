/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2020, XIA LLC
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
/******************************************************************************
*
* File Name:
*
*		Pixie16Boot.c
*
* Description:
*
*		This file contains the code necessary to boot a crate of Pixie modules.
******************************************************************************/
#include "functions.hpp"
#include "pixie16app_export.h"

#include <chrono>
#include <iostream>
#include <vector>

#include <csignal>
#include <cstdio>
#include <cstring>
#include <ctime>

#ifdef _WIN64
#include <Windows.h>
#else
#include <unistd.h>
#endif


using namespace std;

int main(int argc, char** argv) {
    xia::Configuration cfg;
    if (xia::cmdOptionExists(argv, argv + argc, "-c"))
        try {
            cfg = xia::read_configuration_file(xia::getCmdOption(argv, argv + argc, "-c"));
        } catch (invalid_argument& invalidArgument) {
            cerr << invalidArgument.what() << endl;
            return 1;
        }
    else {
        cerr << "ERROR - Need to provide the configuration file for reading." << endl;
        return 1;
    }

    cout << "INFO - Initializing Pixie Modules.......";
    int retval = Pixie16InitSystem(cfg.numModules, cfg.slot_map, 0);
    if (retval < 0) {
        cerr << endl << "ERROR - Pixie16InitSystem failed, retval = %d" << retval << endl;
        return retval;
    } else
        cout << "OK" << endl;

    cout << "INFO - Booting Pixie modules........" << endl;
    retval = Pixie16BootModule(cfg.ComFPGAConfigFile.c_str(), cfg.SPFPGAConfigFile.c_str(),
                               cfg.TrigFPGAConfigFile.c_str(), cfg.DSPCodeFile.c_str(), cfg.DSPParFile.c_str(),
                               cfg.DSPVarFile.c_str(), cfg.numModules, 0x7F);
    if (retval < 0) {
        cerr << endl << "ERROR - Failed to boot modules with Error Code " << retval << endl;
        return retval;
    } else
        cout << "OK" << endl;
}
