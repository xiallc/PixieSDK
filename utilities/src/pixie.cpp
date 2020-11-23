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
using namespace std::chrono;

int main(int argc, char** argv) {
    xia::Configuration cfg;
    if (xia::cmdOptionExists(argv, argv + argc, "-c"))
        try {
            cfg = xia::read_configuration_file(xia::getCmdOption(argv, argv + argc, "-c"));
        } catch (invalid_argument& invalidArgument) {
            cerr << invalidArgument.what() << endl;
            return EXIT_FAILURE;
        }
    else {
        cerr << "ERROR - Need to provide the configuration file for reading." << endl;
        return EXIT_FAILURE;
    }

    cout << "INFO - Initializing Pixie Modules.......";
    int retval = Pixie16InitSystem(cfg.numModules, cfg.slot_map, 0);
    if (retval < 0) {
        cerr << endl << "ERROR - Pixie16InitSystem failed, retval = %d" << retval << endl;
        return EXIT_FAILURE;
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

    if (xia::cmdOptionExists(argv, argv + argc, "--list-mode-run")) {
        // Adjust DC-Offsets
        for (int k = 0; k < cfg.numModules; k++) {
            retval = Pixie16AdjustOffsets(k);
            if (retval < 0) {
                cerr << "ERROR - Pixie16AdjustOffsets in module" << k << "failed with Error Code " << retval << endl;
                return EXIT_FAILURE;
            }
        }

        cout << "INFO - Setting SYNCH_WAIT in Module 0.......";
        retval = Pixie16WriteSglModPar("SYNCH_WAIT", 1, 0);
        if (retval < 0) {
            cerr << endl << "ERROR - Setting SYNC_WAIT failed with Exit Code " << retval << endl;
            return EXIT_FAILURE;
        } else
            cout << "OK" << endl;

        cout << "INFO - Setting IN_SYNCH in Module 0.......";
        retval = Pixie16WriteSglModPar("IN_SYNCH", 0, 0);
        if (retval < 0) {
            cerr << endl << "ERROR - Setting IN_SYNC failed with Exit Code " << retval << endl;
            return EXIT_FAILURE;
        } else
            cout << "OK" << endl;

        cout << "INFO - Starting List Mode Run.......";
        retval = Pixie16StartListModeRun(cfg.numModules, 0x100, NEW_RUN);
        if (retval < 0) {
            cerr << endl << "ERROR - Pixie16StartListModeRun failed with Error Code " << retval << endl;
            return EXIT_FAILURE;
        } else
            cout << "OK" << endl;

        cout << "INFO - Waiting for DSP to boot....";
        sleep(1);
        cout << "OK" << endl;


        unsigned int* lmdata;
        if ((lmdata = (unsigned int*) malloc(sizeof(unsigned int) * 131072)) == nullptr) {
            cerr << "ERROR - Failed to allocate memory block (lmdata) for list mode data!" << endl;
            return EXIT_FAILURE;
        }

        unsigned int mod_numwordsread = 0;

        vector<string> output_file_names;
        for (auto i = 0; i < cfg.numModules; i++)
            output_file_names.push_back("module" + to_string(i) + ".lmd");

        size_t requested_run_length_in_seconds = 10;
        if (xia::cmdOptionExists(argv, argv + argc, "-t"))
            requested_run_length_in_seconds = stod(xia::getCmdOption(argv, argv + argc, "-t"));

        cout << "INFO - Collecting data for " << requested_run_length_in_seconds << " s." << endl;
        steady_clock::time_point run_start_time = steady_clock::now();
        while (duration_cast<duration<double>>(steady_clock::now() - run_start_time).count() <
               requested_run_length_in_seconds) {
            for (int k = 0; k < cfg.numModules; k++) {
                retval = Pixie16SaveExternalFIFODataToFile(output_file_names[k].c_str(), &mod_numwordsread, k, 0);
                if (retval < 0) {
                    cerr << "*ERROR* Pixie16SaveExternalFIFODataToFile failed for Module " << k << "with Error Code "
                         << retval;
                    free(lmdata);
                    break;
                }
            }

            // Check the run status of the Director module (module #0) to see if the run has been stopped.
            // This is possible in a multi-chassis system where modules in one chassis can stop the run
            // in all chassis.
            if (Pixie16CheckRunStatus(0) == 0) {
                cout << "Run was stopped but number of events are not reached yet" << endl;
                break;
            }
        }

        // Stop run in the Director module (module #0) - a SYNC interrupt should be generated
        // to stop run in all modules simultaneously
        cout << "INFO - Stopping List Mode Run.......";
        retval = Pixie16EndRun(0);
        if (retval < 0) {
            cerr << endl << "ERROR - Stopping List Mode Run failed with Error Code " << retval << endl;
            return 3;
        } else
            cout << "OK" << endl;

        // Make sure all modules indeed finish their run successfully.
        for (int k = 0; k < cfg.numModules; k++) {
            size_t finalize_attempt_number = 0;
            while (finalize_attempt_number < 10) {
                if (Pixie16CheckRunStatus(k) != 0) {
                    retval = Pixie16SaveExternalFIFODataToFile(output_file_names[k].c_str(), &mod_numwordsread, k, 1);
                    if (retval < 0) {
                        cerr << "ERROR - Pixie16SaveExternalFIFODataToFile failed in module " << k << "with Exit Code "
                             << retval << endl;
                        free(lmdata);
                        return EXIT_FAILURE;
                    }
                } else
                    break;

                finalize_attempt_number++;
            }
            if (finalize_attempt_number == 10)
                cout << "end run in module " << k << " failed" << endl << flush;
        }
        cout << "INFO - Finished collecting data in "
             << duration_cast<duration<double>>(steady_clock::now() - run_start_time).count() << " s" << endl;

        // All modules have their run stopped successfully. Now read out the possible last words from the external FIFO
        for (int k = 0; k < cfg.numModules; k++) {
            retval = Pixie16SaveExternalFIFODataToFile(output_file_names[k].c_str(), &mod_numwordsread, k, 1);
            if (retval < 0) {
                cerr << "ERROR - Pixie16SaveExternalFIFODataToFile failed in module " << k << "with Exit Code "
                     << retval << endl;
                free(lmdata);
                return EXIT_FAILURE;
            }
        }

        free(lmdata);
    }

    if (xia::cmdOptionExists(argv, argv + argc, "--save-dsp-pars")) {
        auto output_file_name = xia::getCmdOption(argv, argv + argc, "--save-dsp-pars");
        if (!output_file_name) {
            cerr << "ERROR - You must provide a file name with --save-dsp-pars!" << endl;
            return EXIT_FAILURE;
        }

        cout << "INFO - Saving DSP Parameters to " << output_file_name << "....";
        retval = Pixie16SaveDSPParametersToFile(output_file_name);
        if (retval < 0) {
            cerr << endl << "ERROR - Saving DSP parameters to file failed with Exit Code " << retval << endl;
        } else
            cout << "OK" << endl;
    }

    if (xia::cmdOptionExists(argv, argv + argc, "--histograms")) {
        cout << "INFO - Starting to write histograms from the module...";
        for (int i = 0; i < cfg.numModules; i++)
            Pixie16SaveHistogramToFile(("module" + to_string(i) + ".his").c_str(), i);
        cout << "OK" << endl;
    }

    cout << "INFO - Closing out the connection to Modules....";
    for (int i = 0; i < cfg.numModules; i++) {
        retval = Pixie16ExitSystem(i);
        if (retval != 0) {
            cerr << endl << "ERROR - Closing Module " << i << " failed with Exit Code " << retval << endl;
        }
    }
    cout << "OK" << endl;
}
