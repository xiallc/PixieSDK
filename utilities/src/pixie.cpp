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
/// @author H.Tan and S.V.Paulauskas
/// @date November 14, 2020
#include "args.hxx"
#include "configuration.hpp"
#include "pixie16app_export.h"
#include "easylogging++.h"

#include <chrono>
#include <iostream>
#include <vector>
#include <thread>

#include <cstring>

#if defined(_WIN64) || defined(_WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace std::chrono;

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const bool& print_success = true) {
    if (val < 0) {
        LOG(FATAL) << func_name << " failed with Error Code " << val;
        return false;
    }
    if (print_success)
        LOG(INFO) << func_name << " finished successfully.";
    return true;
}

bool save_dsp_pars(const std::string& filename) {
    LOG(INFO) << "Saving DSP Parameters to " << filename << ".";
    if (!verify_api_return_value(Pixie16SaveDSPParametersToFile(filename.c_str()),
                                 "Pixie16SaveDSPParametersToFile"))
        return false;
    return true;
}

bool execute_adjust_offsets(const unsigned int& numModules, const std::string& setfile) {
    for (unsigned int k = 0; k < numModules; k++) {
        LOG(INFO) << "Adjusting baseline offset for Module " << k << ".";
        if (!verify_api_return_value(Pixie16AdjustOffsets(k),
                                     "Pixie16AdjustOffsets for Module" + to_string(k)))
            return false;
    }

    if (!save_dsp_pars(setfile))
        return false;

    return true;
}

bool execute_list_mode_run(const xia::configuration::Configuration& cfg,
                           const double& runtime_in_seconds) {
    LOG(INFO) << "Starting list mode data run for " << runtime_in_seconds << " s.";

    if (!execute_adjust_offsets(cfg.numModules, cfg.DSPParFile))
        return false;

    LOG(INFO) << "Calling Pixie16WriteSglModPar to write SYNCH_WAIT = 1 in Module 0.";
    if (!verify_api_return_value(Pixie16WriteSglModPar("SYNCH_WAIT", 1, 0),
                                 "Pixie16WriteSglModPar - SYNC_WAIT"))
        return false;

    LOG(INFO) << "Calling Pixie16WriteSglModPar to write IN_SYNCH  = 0 in Module 0.";
    if (!verify_api_return_value(Pixie16WriteSglModPar("IN_SYNCH", 0, 0),
                                 "Pixie16WriteSglModPar - IN_SYNC"))
        return false;

    LOG(INFO) << "Calling Pixie16StartListModeRun.";
    if (!verify_api_return_value(Pixie16StartListModeRun(cfg.numModules, 0x100, NEW_RUN),
                                 "Pixie16StartListModeRun"))
        return false;

    LOG(INFO) << "Waiting 1 second for DSP to boot.";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    LOG(INFO) << "DSP boot wait is over!";

    unsigned int* lmdata;
    if ((lmdata = (unsigned int*) malloc(sizeof(unsigned int) * 131072)) == nullptr) {
        LOG(FATAL) << "Failed to allocate memory block (lmdata) for list mode data!";
        return false;
    }

    unsigned int mod_numwordsread = 0;

    vector<string> output_file_names;
    output_file_names.reserve(cfg.numModules);
    for (auto i = 0; i < cfg.numModules; i++)
        output_file_names.push_back("module" + to_string(i) + ".lmd");

    LOG(INFO) << "Collecting data for " << runtime_in_seconds << " s.";
    steady_clock::time_point run_start_time = steady_clock::now();
    while (duration_cast<duration<double>>(steady_clock::now() - run_start_time).count() <
           runtime_in_seconds) {
        for (int k = 0; k < cfg.numModules; k++) {
            if (!verify_api_return_value(
                    Pixie16SaveExternalFIFODataToFile(output_file_names[k].c_str(),
                                                      &mod_numwordsread, k, 0),
                    "Pixie16SaveExternalFIFODataToFile for Module " + to_string(k), false)) {
                free(lmdata);
                break;
            }
        }

        // Check the run status of the Director module (module #0) to see if the run has been stopped.
        // This is possible in a multi-chassis system where modules in one chassis can stop the run
        // in all chassis.
        if (Pixie16CheckRunStatus(0) == 0) {
            LOG(INFO) << "Run was stopped but number of events are not reached yet";
            break;
        }
    }

    // Stop run in the Director module (module #0) - a SYNC interrupt should be generated
    // to stop run in all modules simultaneously
    LOG(INFO) << "Stopping List Mode Run.";
    if (!verify_api_return_value(Pixie16EndRun(0), "Pixie16EndRun"))
        return false;

    // Make sure all modules indeed finish their run successfully.
    for (int k = 0; k < cfg.numModules; k++) {
        size_t finalize_attempt_number = 0;
        while (finalize_attempt_number < 10) {
            if (Pixie16CheckRunStatus(k) != 0) {
                if (!verify_api_return_value(
                        Pixie16SaveExternalFIFODataToFile(output_file_names[k].c_str(),
                                                          &mod_numwordsread, k, 1),
                        "Pixie16SaveExternalFIFODataToFile for Module " + to_string(k), false)) {
                    free(lmdata);
                    return false;
                }
            } else
                break;

            finalize_attempt_number++;
        }
        if (finalize_attempt_number == 10)
            LOG(FATAL) << "End run in module " << k << " failed";
    }

    LOG(INFO) << "Finished collecting data in "
              << duration_cast<duration<double>>(steady_clock::now() - run_start_time).count()
              << " s";

    // All modules have their run stopped successfully.Now read out the possible last words from the external FIFO
    for (int k = 0; k < cfg.numModules; k++) {
        if (!verify_api_return_value(
                Pixie16SaveExternalFIFODataToFile(output_file_names[k].c_str(), &mod_numwordsread,
                                                  k, 1),
                "Pixie16SaveExternalFIFODataToFile for Module " + to_string(k), false)) {
            free(lmdata);
            return false;
        }
    }
    free(lmdata);
    return true;
}

bool execute_parameter_read(args::ValueFlag<string>& parameter,
                            args::ValueFlag<unsigned int>& crate,
                            args::ValueFlag<unsigned int>& module,
                            args::ValueFlag<unsigned int>& channel) {
    if (channel) {
        double result;
        LOG(INFO) << "Pixie16ReadSglChanPar reading " << parameter.Get() << " from Crate "
                  << crate.Get() << " Module " << module.Get()
                  << " Channel " << channel.Get() << ".";
        if (!verify_api_return_value(
                Pixie16ReadSglChanPar(parameter.Get().c_str(), &result, module.Get(),
                                      channel.Get()),
                "Pixie16ReadSglChanPar", false))
            return false;
        LOG(INFO) << result;
    } else {
        unsigned int result;
        LOG(INFO) << "Pixie16ReadSglModPar reading " << parameter.Get() << " from Crate "
                  << crate.Get() << " Module " << module.Get() << ".";
        if (!verify_api_return_value(
                Pixie16ReadSglModPar(parameter.Get().c_str(), &result, module.Get()),
                "Pixie16ReadSglModPar", false))
            return false;
        LOG(INFO) << result;
    }
    return true;
}

bool execute_parameter_write(args::ValueFlag<string>& parameter, args::ValueFlag<double>& value,
                             args::ValueFlag<unsigned int>& crate,
                             args::ValueFlag<unsigned int>& module,
                             args::ValueFlag<unsigned int>& channel, const std::string& setfile) {
    if (channel) {
        LOG(INFO) << "Pixie16WriteSglChanPar setting " << parameter.Get() << " to " << value.Get()
                  << " for Crate " << crate.Get() << " Module " << module.Get()
                  << " Channel " << channel.Get() << ".";
        if (!verify_api_return_value(
                Pixie16WriteSglChanPar(parameter.Get().c_str(), value.Get(), module.Get(),
                                       channel.Get()),
                "Pixie16WriteSglChanPar"))
            return false;
    } else {
        LOG(INFO) << "Pixie16WriteSglModPar setting " << parameter.Get() << " to " << value.Get()
                  << " for  Crate " << crate.Get() << " Module " << module.Get() << ".";
        if (!verify_api_return_value(
                Pixie16WriteSglModPar(parameter.Get().c_str(), value, module.Get()),
                "Pixie16ReadSglModPar"))
            return false;
    }

    if (!save_dsp_pars(setfile))
        return false;
    return true;
}

bool execute_trace_capture(args::ValueFlag<unsigned int>& module) {
    if (!module)
        return false;

    LOG(INFO) << "Pixie16AcquireADCTrace acquiring traces for Module" << module.Get() << ".";
    if (!verify_api_return_value(Pixie16AcquireADCTrace(module.Get()), "Pixie16AcquireADCTrace"))
        return false;

    unsigned short trace[NUMBER_OF_CHANNELS][MAX_ADC_TRACE_LEN];
    for (unsigned int i = 0; i < NUMBER_OF_CHANNELS; i++) {
        if (!verify_api_return_value(
                Pixie16ReadSglChanADCTrace(trace[i], MAX_ADC_TRACE_LEN, module.Get(), i),
                "Pixie16ReadSglChanADCTrace", false))
            return false;
    }

    ofstream ofstream1("traces-module" + to_string(module.Get()) + ".csv");
    ofstream1 << "bin,";
    for (unsigned int i = 0; i < NUMBER_OF_CHANNELS; i++)
        ofstream1 << "Chan" << i << ",";
    ofstream1 << endl;
    for (unsigned int i = 0; i < MAX_ADC_TRACE_LEN; i++) {
        ofstream1 << i << ",";
        for (unsigned int k = 0; k < NUMBER_OF_CHANNELS; k++) {
            if (k != NUMBER_OF_CHANNELS - 1)
                ofstream1 << trace[k][i] << ",";
            else
                ofstream1 << trace[k][i];
        }
        ofstream1 << endl;
    }

    return true;
}

bool execute_close_module_connection(const int& numModules) {
    for (int i = 0; i < numModules; i++) {
        LOG(INFO) << "Closing out connection to Module " << i << ".";
        verify_api_return_value(Pixie16ExitSystem(i),
                                "Pixie16ExitSystem for Module" + to_string(i));
    }
    return true;
}

double calculate_duration_in_seconds(const std::chrono::system_clock::time_point& start,
                                     const std::chrono::system_clock::time_point& end) {
    return std::chrono::duration<double>(end - start).count();
}

void configure_logging(int argc, char** argv) {
    START_EASYLOGGINGPP(argc, argv);
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(
            el::ConfigurationType::Format, "%datetime{%Y-%M-%dT%H:%m:%s.%g} - %level - %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);
}

int main(int argc, char** argv) {
    auto start = std::chrono::system_clock::now();
    configure_logging(argc, argv);
    args::ArgumentParser parser(
            "Sample code that interfaces with a Pixie system through the User API.");
    parser.LongSeparator("=");

    args::Group commands(parser, "commands");
    args::Command boot(commands, "boot", "Boots the crate of modules.");
    args::Command export_settings(commands, "export-settings",
                                  "Boots the system and dumps the settings to the file defined in the config.");
    args::Command histogram(commands, "histogram", "Save histograms from the module.");
    args::Command list_mode(commands, "list-mode", "Starts a list mode data run");
    args::Command read(commands, "read", "Read a parameter from the module.");
    args::Command write(commands, "write", "Write a parameter to the module.");
    args::Command trace(commands, "trace", "Captures traces from the modules.");
    args::Command adjust_offsets(commands, "adjust_offsets",
                                 "Adjusts the DC offsets for all modules in the config file.");
    //args::Command mca(commands, "mca", "Starts an MCA data run.");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);

    args::Positional<std::string> configuration(arguments, "cfg", "The configuration file to load.",
                                                args::Options::Required);
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});
    args::Flag is_fast_boot(boot, "fast-boot", "Performs a partial boot of the system.",
                            {'f', "fast-boot"});
    args::Flag is_offline(arguments, "Offline Mode",
                          "Tells the API to use Offline mode when running.",
                          {'o', "offline"});
    args::ValueFlag<std::string> boot_pattern_flag(arguments, "boot_pattern",
                                                   "The boot pattern used for booting.",
                                                   {'b', "boot_pattern"}, "0x7F");
    args::ValueFlag<double> run_time(list_mode, "time",
                                     "The amount of time that a list mode run will take in seconds.",
                                     {'t', "run-time"}, 10.);
    args::ValueFlag<string> parameter(read, "parameter",
                                      "The parameter we want to read from the system.",
                                      {'n', "name"});
    args::ValueFlag<unsigned int> crate(read, "crate", "The crate", {"crate"}, 0);
    args::ValueFlag<unsigned int> module(read, "module", "The module", {"mod"});
    args::ValueFlag<unsigned int> channel(read, "channel", "The channel", {"chan"});
    args::ValueFlag<double> parameter_value(write, "parameter_value",
                                            "The value of the parameter we want to write.",
                                            {'v', "value"});
    adjust_offsets.Add(configuration);
    trace.Add(configuration);
    trace.Add(module);
    write.Add(configuration);
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
        LOG(FATAL) << e.what();
        cout << parser;
        return EXIT_FAILURE;
    }

    xia::configuration::Configuration cfg;
    try {
        cfg = xia::configuration::read_configuration_file(configuration.Get());
    } catch (invalid_argument& invalidArgument) {
        LOG(FATAL) << invalidArgument.what();
        return EXIT_FAILURE;
    }

    LOG(INFO) << "Finished reading config in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s.";

    int offline_mode = 0;
    if (is_offline)
        offline_mode = 1;

    start = std::chrono::system_clock::now();
    LOG(INFO) << "Calling Pixie16InitSystem.";
    if (!verify_api_return_value(Pixie16InitSystem(cfg.numModules, cfg.slot_map, offline_mode),
                                 "Pixie16InitSystem", false))
        return EXIT_FAILURE;
    LOG(INFO) << "Finished Pixie16InitSystem in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s.";

    start = std::chrono::system_clock::now();
    unsigned int boot_pattern = stoul(args::get(boot_pattern_flag), nullptr, 0);
    if (is_fast_boot)
        boot_pattern = 0x70;

    if (boot_pattern == 0) {
        LOG(INFO) << "Will not boot the module!";
    } else {
        LOG(INFO) << "Calling Pixie16BootModule with boot pattern: " << showbase << hex
                  << boot_pattern << dec;

        if (!verify_api_return_value(
                Pixie16BootModule(cfg.ComFPGAConfigFile.c_str(), cfg.SPFPGAConfigFile.c_str(),
                                  cfg.TrigFPGAConfigFile.c_str(), cfg.DSPCodeFile.c_str(),
                                  cfg.DSPParFile.c_str(), cfg.DSPVarFile.c_str(), cfg.numModules,
                                  boot_pattern),
                "Pixie16BootModule", "INFO - Finished booting!"))
            return EXIT_FAILURE;
        LOG(INFO) << "Finished Pixie16BootModule in "
                  << calculate_duration_in_seconds(start, std::chrono::system_clock::now())
                  << " s.";
        if (boot) {
            execute_close_module_connection(cfg.numModules);
            return EXIT_SUCCESS;
        }
    }

    if (read) {
        if (!execute_parameter_read(parameter, crate, module, channel))
            return EXIT_FAILURE;
        execute_close_module_connection(cfg.numModules);
        return EXIT_SUCCESS;
    }

    if (write) {
        if (!execute_parameter_write(parameter, parameter_value, crate, module, channel,
                                     cfg.DSPParFile))
            return EXIT_FAILURE;
        execute_close_module_connection(cfg.numModules);
        return EXIT_SUCCESS;
    }

    if (adjust_offsets) {
        if (!execute_adjust_offsets(cfg.numModules, cfg.DSPParFile))
            return EXIT_FAILURE;
        execute_close_module_connection(cfg.numModules);
        return EXIT_SUCCESS;
    }

    if (trace) {
        if (!execute_trace_capture(module))
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
        LOG(INFO) << "Starting to write histograms from the module.";
        for (int i = 0; i < cfg.numModules; i++)
            Pixie16SaveHistogramToFile(("module" + to_string(i) + ".his").c_str(), i);
        LOG(INFO) << "Finished writing histograms from the module.";
    }

    execute_close_module_connection(cfg.numModules);
}
