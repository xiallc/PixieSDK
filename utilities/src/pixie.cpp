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
/// @file pixie.cpp
/// @brief This file contains the code necessary to boot a crate of Pixie modules.
/// @author H. Tan and S. V. Paulauskas
/// @date November 14, 2020
#include "args.hxx"
#include "functions.hpp"
#include "pixie16app_export.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

#include <cstring>

#ifdef _WIN64
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;
using namespace std::chrono;

void display_help() {
    cout << "Pixie sample user interface" << endl
         << endl
         << "Usage:" << endl
         << "\tpixie -h | --help" << endl
         << "\tpixie -c <path to config>" << endl
         << "\tpixie (--list-mode-run | -l) [-t <run time in seconds>]" << endl
         << "\tpixie --save-dsp-pars <name>" << endl
         << "\tpixie --histograms" << endl
         << "\tpixie -c <path to config> -r <PARAMETER NAME> --crate <x> --mod <y> [--chan <z>]" << endl
         << "\tpixie -c <path to config> -w <PARAMETER NAME> -v <val> --crate <x> --mod <y> [--chan <z>]" << endl
         << "Options:" << endl
         << "\t-h --help                 Display this message" << endl
         << "\t-c <config file>          Configuration file to use." << endl
         << "\t--list-mode-run           Starts a list mode run " << endl
         << "\t-t <run time in seconds>  Defines how long a list mode run will execute. [Default: 10]" << endl
         << "\t--save-dsp-pars <name>    Saves the DSP parameters to a file. " << endl
         << "\t--histograms              Reads histograms from the modules and saves them to a file." << endl
         << "\t-f --fast-boot            Skips uploading firmware to the modules." << endl
         << "\t--mod <y>                 Module number to read from" << endl
         << "\t--chan <z>                Channel number to read from" << endl
         << "\t--crate <x>               Crate number to read from" << endl
         << "\t-v <val>                  Value we will write to the specified Crate/Module[/Channel]. " << endl;
}

bool verify_api_return_value(const int& val, const std::string& func_name, const std::string& okmsg = "OK") {
    if (val < 0) {
        cerr << endl << "ERROR - " << func_name << " with Error Code " << val << endl;
        return false;
    }
    if (!okmsg.empty())
        cout << okmsg << endl;
    return true;
}

bool verify_cli_argument(const std::string* par) {
    if (!par) {
        cerr << "ERROR - Could not parse command line argument!" << endl;
        display_help();
        return false;
    }
    return true;
}

bool save_dsp_pars(const std::string& filename) {
    cout << "INFO - Saving DSP Parameters to " << filename << "....";
    if (!verify_api_return_value(Pixie16SaveDSPParametersToFile(filename.c_str()), "Pixie16SaveDSPParametersToFile"))
        return false;
    return true;
}

bool execute_list_mode_run(const xia::Configuration& cfg, const double& runtime_in_seconds) {
    for (int k = 0; k < cfg.numModules; k++) {
        if (!verify_api_return_value(Pixie16AdjustOffsets(k), "Pixie16AdjustOffsets for Module" + to_string(k)))
            return false;
    }

    cout << "INFO - Calling Pixie16WriteSglModPar to write SYNCH_WAIT = 1 in Module 0.......";
    if (!verify_api_return_value(Pixie16WriteSglModPar("SYNCH_WAIT", 1, 0), "Pixie16WriteSglModPar - SYNC_WAIT"))
        return false;

    cout << "INFO - Calling Pixie16WriteSglModPar to write IN_SYNCH  = 0 in Module 0.......";
    if (!verify_api_return_value(Pixie16WriteSglModPar("IN_SYNCH", 0, 0), "Pixie16WriteSglModPar - IN_SYNC"))
        return false;

    cout << "INFO - Calling Pixie16StartListModeRun.......";
    if (!verify_api_return_value(Pixie16StartListModeRun(cfg.numModules, 0x100, NEW_RUN), "Pixie16StartListModeRun"))
        return false;

    cout << "INFO - Waiting for DSP to boot....";
    sleep(1);
    cout << "OK" << endl;

    unsigned int* lmdata;
    if ((lmdata = (unsigned int*) malloc(sizeof(unsigned int) * 131072)) == nullptr) {
        cerr << "ERROR - Failed to allocate memory block (lmdata) for list mode data!" << endl;
        return false;
    }

    unsigned int mod_numwordsread = 0;

    vector<string> output_file_names;
    output_file_names.reserve(cfg.numModules);
    for (auto i = 0; i < cfg.numModules; i++)
        output_file_names.push_back("module" + to_string(i) + ".lmd");

    cout << "INFO - Collecting data for " << runtime_in_seconds << " s." << endl;
    steady_clock::time_point run_start_time = steady_clock::now();
    while (duration_cast<duration<double>>(steady_clock::now() - run_start_time).count() < runtime_in_seconds) {
        for (int k = 0; k < cfg.numModules; k++) {
            if (!verify_api_return_value(
                        Pixie16SaveExternalFIFODataToFile(output_file_names[k].c_str(), &mod_numwordsread, k, 0),
                        "Pixie16SaveExternalFIFODataToFile for Module " + to_string(k), "")) {
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
    if (!verify_api_return_value(Pixie16EndRun(0), "Pixie16EndRun"))
        return false;

    // Make sure all modules indeed finish their run successfully.
    for (int k = 0; k < cfg.numModules; k++) {
        size_t finalize_attempt_number = 0;
        while (finalize_attempt_number < 10) {
            if (Pixie16CheckRunStatus(k) != 0) {
                if (!verify_api_return_value(
                            Pixie16SaveExternalFIFODataToFile(output_file_names[k].c_str(), &mod_numwordsread, k, 1),
                            "Pixie16SaveExternalFIFODataToFile for Module " + to_string(k), "")) {
                    free(lmdata);
                    return false;
                }
            } else
                break;

            finalize_attempt_number++;
        }
        if (finalize_attempt_number == 10)
            cerr << "ERROR - End run in module " << k << " failed" << endl;
    }

    cout << "INFO - Finished collecting data in "
         << duration_cast<duration<double>>(steady_clock::now() - run_start_time).count() << " s" << endl;

    // All modules have their run stopped successfully. Now read out the possible last words from the external FIFO
    for (int k = 0; k < cfg.numModules; k++) {
        if (!verify_api_return_value(
                    Pixie16SaveExternalFIFODataToFile(output_file_names[k].c_str(), &mod_numwordsread, k, 1),
                    "Pixie16SaveExternalFIFODataToFile for Module " + to_string(k), "")) {
            free(lmdata);
            return false;
        }
    }
    free(lmdata);
    return true;
}

bool execute_parameter_read(const int& argc, char** argv) {
    if (!xia::cmdOptionExists(argc, argv, "--crate") || !xia::cmdOptionExists(argc, argv, "--mod")) {
        cerr << "ERROR - You must specify a crate and module to read from!" << endl;
        display_help();
        return false;
    }

    bool is_channel_parameter = xia::cmdOptionExists(argc, argv, "--chan");

    int crate, mod, chan;
    try {
        crate = stoi(xia::getCmdOption(argc, argv, "--crate"));
        mod = stoi(xia::getCmdOption(argc, argv, "--mod"));
        if (is_channel_parameter)
            chan = stoi(xia::getCmdOption(argc, argv, "--chan"));
    } catch (invalid_argument& invalid_argument) {
        cerr << "ERROR - Invalid option for crate, module, or channel!" << endl;
        display_help();
        return false;
    }

    string parameter_name = xia::getCmdOption(argc, argv, "-r");

    if (!verify_cli_argument(&parameter_name)) {
        cout << "ERROR - Invalid argument parsed!" << endl;
        display_help();
        return false;
    }

    if (is_channel_parameter) {
        double result;
        cout << "INFO - Pixie16ReadSglChanPar reading " << parameter_name << " from Crate " << crate << " Module "
             << mod << " Channel " << chan << "........ ";
        if (!verify_api_return_value(Pixie16ReadSglChanPar(parameter_name.c_str(), &result, mod, chan),
                                     "Pixie16ReadSglChanPar", ""))
            return false;
        cout << result << endl;
    } else {
        unsigned int result;
        cout << "INFO - Pixie16ReadSglModPar reading " << parameter_name << " from Crate " << crate << " Module " << mod
             << "........ ";
        if (!verify_api_return_value(Pixie16ReadSglModPar(parameter_name.c_str(), &result, mod), "Pixie16ReadSglModPar",
                                     ""))
            return false;
        cout << result << endl;
    }
    return true;
}

bool execute_parameter_write(const int& argc, char** argv, const std::string& setfile) {
    if (!xia::cmdOptionExists(argc, argv, "--crate") || !xia::cmdOptionExists(argc, argv, "--mod")) {
        cerr << "ERROR - You must specify a crate and module to read from!" << endl;
        display_help();
        return false;
    }

    bool is_channel_parameter = xia::cmdOptionExists(argc, argv, "--chan");

    int crate, mod, chan;
    try {
        crate = stoi(xia::getCmdOption(argc, argv, "--crate"));
        mod = stoi(xia::getCmdOption(argc, argv, "--mod"));
        if (is_channel_parameter)
            chan = stoi(xia::getCmdOption(argc, argv, "--chan"));
    } catch (invalid_argument& invalid_argument) {
        cerr << "ERROR - Invalid option for crate, module, or channel!" << endl;
        display_help();
        return false;
    }

    string parameter_name = xia::getCmdOption(argc, argv, "-w");

    if (!verify_cli_argument(&parameter_name)) {
        cout << "ERROR - Invalid argument parsed!" << endl;
        display_help();
        return false;
    }

    double value;
    try {
        value = stod(xia::getCmdOption(argc, argv, "-v"));
    } catch (invalid_argument& invalid_argument) {
        cerr << "ERROR - Invalid option for value!" << endl;
        display_help();
        return false;
    }

    if (is_channel_parameter) {
        cout << "INFO - Pixie16WriteSglChanPar setting " << parameter_name << " to " << value << " for Crate " << crate
             << " Module " << mod << " Channel " << chan << "........ ";
        if (!verify_api_return_value(Pixie16WriteSglChanPar(parameter_name.c_str(), value, mod, chan),
                                     "Pixie16WriteSglChanPar"))
            return false;
    } else {
        cout << "INFO - Pixie16WriteSglModPar setting " << parameter_name << " to " << value << " for  Crate " << crate
             << " Module " << mod << "........ ";
        if (!verify_api_return_value(Pixie16WriteSglModPar(parameter_name.c_str(), value, mod), "Pixie16ReadSglModPar"))
            return false;
    }

    if (!save_dsp_pars(setfile))
        return false;
    return true;
}

bool execute_close_module_connection(const int& numModules) {
    for (int i = 0; i < numModules; i++) {
        cout << "INFO - Closing out connection to Module " << i << "......";
        verify_api_return_value(Pixie16ExitSystem(i), "Pixie16ExitSystem for Module" + to_string(i));
    }
    return true;
}

int main(int argc, char** argv) {
    args::ArgumentParser parser("Sample code that interfaces with a Pixie system through the User API.");
    parser.LongSeparator("=");

    args::Group commands(parser, "commands");

    args::Command boot(commands, "boot", "Boots the crate of modules.");
    args::Command export_settings(commands, "export-settings",
                                  "Boots the system and dumps the settings to the file defined in the config.");
    args::Command fast_boot(commands, "fast-boot", "Boots the crate of modules.");
    args::Command histogram(commands, "histogram", "Save histograms from the module.");
    args::Command list_mode(commands, "list-mode", "Starts a list mode data run");
    //args::Command mca(commands, "mca", "Starts an MCA data run.");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne, args::Options::Global);
    args::Positional<std::string> configuration(arguments, "cfg", "The configuration file to load.",
                                                args::Options::Required);
    args::HelpFlag h(arguments, "help", "Displays this message", {'h', "help"});
    args::Flag is_offline(arguments, "Offline Mode", "Tells the API to use Offline mode when running.",
                          {'o', "offline"});

    args::ValueFlag<double> run_time(list_mode, "time", "The amount of time that a list mode run will take in seconds.",
                                     {'t', "run-time"}, 10.);

    args::Command read(commands, "read", "Read a parameter from the module.");
    args::Command write(commands, "write", "Write a parameter to the module.");
    args::ValueFlag<string> parameter(read, "parameter", "The parameter we want to read from the system.",
                                      {'n', "name"});
    args::ValueFlag<double> crate(read, "crate", "The crate to inspect.", {"crate"}, 0);
    args::ValueFlag<double> module(read, "module", "The module to inspect.", {"mod"});
    args::ValueFlag<double> channel(read, "channel", "The channel to inspect.", {"chan"});

    write.Add(parameter);
    write.Add(crate);
    write.Add(module);
    write.Add(channel);

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

    xia::Configuration cfg;
    try {
        cfg = xia::read_configuration_file(configuration.Get());
    } catch (invalid_argument& invalidArgument) {
        cerr << invalidArgument.what() << endl;
        return EXIT_FAILURE;
    }

    int offline_mode = 0;
    if (is_offline)
        offline_mode = 1;

    cout << "INFO - Calling Pixie16InitSystem.......";
    if (!verify_api_return_value(Pixie16InitSystem(cfg.numModules, cfg.slot_map, offline_mode), "Pixie16InitSystem"))
        return EXIT_FAILURE;

    unsigned short boot_pattern = 0x7F;
    if (fast_boot)
        boot_pattern = 0x70;

    cout << "INFO - Calling Pixie16BootModule with boot pattern: " << showbase << hex << boot_pattern << dec
         << "............" << endl;
    if (!verify_api_return_value(Pixie16BootModule(cfg.ComFPGAConfigFile.c_str(), cfg.SPFPGAConfigFile.c_str(),
                                                   cfg.TrigFPGAConfigFile.c_str(), cfg.DSPCodeFile.c_str(),
                                                   cfg.DSPParFile.c_str(), cfg.DSPVarFile.c_str(), cfg.numModules,
                                                   boot_pattern),
                                 "Pixie16BootModule", ""))
        return EXIT_FAILURE;

    if (read) {
        if (!execute_parameter_read(argc, argv))
            return EXIT_FAILURE;
        execute_close_module_connection(cfg.numModules);
        return EXIT_SUCCESS;
    }

    if (write) {
        if (!execute_parameter_write(argc, argv, cfg.DSPParFile))
            return EXIT_FAILURE;
        execute_close_module_connection(cfg.numModules);
        return EXIT_SUCCESS;
    }

    if (list_mode) {
        if (!execute_list_mode_run(cfg, run_time.Get()))
            return EXIT_FAILURE;
        execute_close_module_connection(cfg.numModules);
        return EXIT_SUCCESS;
    }

    if (export_settings) {
        if (!save_dsp_pars(cfg.DSPParFile))
            return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }

    if (histogram) {
        cout << "INFO - Starting to write histograms from the module...";
        for (int i = 0; i < cfg.numModules; i++)
            Pixie16SaveHistogramToFile(("module" + to_string(i) + ".his").c_str(), i);
        cout << "OK" << endl;
    }

    execute_close_module_connection(cfg.numModules);
}
