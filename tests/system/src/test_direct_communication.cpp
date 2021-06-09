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

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "args.hxx"
#include "easylogging++.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "system_test_utilities.hpp"

#ifdef _WINDOWS
#include <windows.h>
#endif

INITIALIZE_EASYLOGGINGPP

/**
 * @brief A small configuration class to hold information useful to the legacy API.
 */
class Configuration {
public:
    unsigned short numModules;
    unsigned short* slot_map;
    std::string ComFPGAConfigFile;
    std::string SPFPGAConfigFile;
    std::string TrigFPGAConfigFile;
    std::string DSPCodeFile;
    std::string DSPParFile;
    std::string DSPVarFile;
};

/**
 * @brief Reads the provided configuration file into the Configuration class.
 * @param config_file_name : The name of the configuration file we'll open.
 * @return A populated instance of the Configuration class.
 */
Configuration read_configuration_file(const std::string& config_file_name) {
    std::ifstream input(config_file_name, std::ios::in);

    if (input.fail()) {
        std::stringstream errmsg;
        errmsg << "Could not open " << config_file_name << "!";
        throw std::invalid_argument(errmsg.str());
    }

    Configuration cfg;
    input >> cfg.numModules;
    cfg.slot_map = new unsigned short[cfg.numModules + 1];
    for (size_t i = 0; i < cfg.numModules; i++)
        input >> cfg.slot_map[i];

    input >> cfg.ComFPGAConfigFile >> cfg.SPFPGAConfigFile >> cfg.TrigFPGAConfigFile >>
          cfg.DSPCodeFile >> cfg.DSPParFile >> cfg.DSPVarFile;

    input.close();
    return cfg;
}


/**
 * @brief Configures the EasyLogging++ logger
 * @param[in] argc : The number of arguments passed in from the command line
 * @param[in] argv : The argument array from the command line
 * @returns Nothing!
 */
void configure_logging(int argc, char** argv) {
    START_EASYLOGGINGPP(argc, argv);
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Filename,
                            "test_direct_communication.log");
    defaultConf.setGlobally(
            el::ConfigurationType::Format, "%datetime{%Y-%M-%dT%H:%m:%s.%g} - %level - %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);
}


int main(int argc, char* argv[]) {
    configure_logging(argc, argv);

    args::ArgumentParser parser("Tests direct communication with the system.");
    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args::Group commands(parser, "commands");
    args::Command boot(commands, "boot", "Just boots the system");
    args::Command dsp(commands, "dsp", "Tests related to the DSP.");
    args::Command mca(commands, "mca",
                      "Tests related to the external memory.");
    args::Command external_fifo(commands, "external_fifo",
                                "Reads external FIFO status and returns the number of 32-bit"
                                " words it contains.");
    args::Command raw(commands, "raw", "Access the raw register IO on the system");
    args::Command csr(commands, "csr", "Access the CSR");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::Positional<std::string> configuration(arguments, "cfg", "The configuration file to load.",
                                                args::Options::Required);

    args::ValueFlag<unsigned int> module_number_flag(arguments, "module_number",
                                                     "The module number to work with.",
                                                     {'m', "module"}, 0);
    args::ValueFlag<std::string> address_flag(arguments, "address",
                                              "The memory address to operate on in hex.",
                                              {'a', "address"}, "0x10073D");
    args::ValueFlag<std::string> boot_pattern_flag(arguments, "boot_pattern",
                                                   "The boot pattern used for booting.",
                                                   {'b', "boot_pattern"}, "0x7F");
    args::Flag clear(mca, "clear", "Clears the main memory", {'c', "clear"});
    args::Flag is_dry_run(arguments, "dry_run", "Control command execution.",
                          {"dry_run"});
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});
    args::ValueFlag<unsigned int> loop_flag(arguments, "num_loops",
                                            "How many times we perform write/read test.",
                                            {'l', "num_loops"}, 1);
    args::Flag status(arguments, "status", "Provides the status of the specified component",
                      {'s', "status"});
    args::Flag read(arguments, "read", "Perform a read procedure", {'r', "read"});
    args::Flag write(arguments, "write", "Perform a write procedure", {'w', "write"});
    args::Flag one_write(arguments, "one_write",
                                                   "Include this flag if you want to write the data only one time.",
                                                   {"one_write"});
    args::Flag burst_read(arguments, "burst_read", "Performs a burst read", {"burst_read"});
    args::Flag verbose(arguments, "verbose", "Control verbosity", {'v', "verbose"});

    args::Group data_arguments(parser, "data_arguments", args::Group::Validators::DontCare,
                               args::Options::Global);
    args::ValueFlag<std::string> data_flag(data_arguments, "data",
                                           "The data that we want to write to the register.",
                                           {'d', "data"}, "0x70FFE3");
    args::MapFlag<std::string, DATA_PATTERN> data_pattern_flag(data_arguments, "test_data_pattern",
                                                               "The type of test data to generate as 32-bit words."
                                                               "\nDefault: CONSTANT",
                                                               {'p', "pattern"},
                                                               {{"HI_LO",     DATA_PATTERN::HI_LO},
                                                                {"FLIP_FLOP", DATA_PATTERN::FLIP_FLOP},
                                                                {"RAMP_UP",   DATA_PATTERN::RAMP_UP},
                                                                {"RAMP_DOWN", DATA_PATTERN::RAMP_DOWN},
                                                                {"CONSTANT",  DATA_PATTERN::CONSTANT},
                                                                {"EVEN_BITS", DATA_PATTERN::EVEN_BITS},
                                                                {"ODD_BITS",  DATA_PATTERN::ODD_BITS},
                                                                {"ZERO",      DATA_PATTERN::ZERO}},
                                                               DATA_PATTERN::CONSTANT);
    args::ValueFlag<unsigned int> data_size_flag(data_arguments, "data_size",
                                                 "The number of 32-bit words to put into the buffer.",
                                                 {'s', "data_size"}, 32768);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        LOG(INFO) << help.what();
        std::cout << parser;
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        LOG(ERROR) << e.what();
        std::cout << parser;
        return EXIT_FAILURE;
    }

    if (is_dry_run)
        LOG(INFO) << "Performing a dry run, none of these commands actually execute.";

    unsigned int address;
    if (args::get(address_flag) == "0x10073D" && !csr && !external_fifo && !boot && !mca) {
        LOG(ERROR) << " You must provide us with a memory address!";
        return EXIT_FAILURE;
    } else
        address = stoul(args::get(address_flag), nullptr, 0);

    Configuration cfg;
    try {
        read_configuration_file(args::get(configuration));
    } catch (std::invalid_argument& invalidArgument) {
        LOG(ERROR) << invalidArgument.what();
        return EXIT_FAILURE;
    }

    int offline_mode = 0;
    if(is_dry_run)
        offline_mode = 1;

    LOG(INFO) << "Calling Pixie16InitSystem.";
    if (!verify_api_return_value(Pixie16InitSystem(cfg.numModules, cfg.slot_map, offline_mode),
                                 "Pixie16InitSystem"))
        return EXIT_FAILURE;

    unsigned int boot_pattern = stoul(args::get(boot_pattern_flag), nullptr, 0);
    if (boot_pattern == 0 || is_dry_run || external_fifo) {
        LOG(INFO) << "Will not boot the module!";
    } else {
        LOG(INFO) << "Calling Pixie16BootModule with boot pattern: " << std::showbase << std::hex
                  << boot_pattern << std::dec;

        if (!verify_api_return_value(
                Pixie16BootModule(cfg.ComFPGAConfigFile.c_str(), cfg.SPFPGAConfigFile.c_str(),
                                  NULL, cfg.DSPCodeFile.c_str(),
                                  cfg.DSPParFile.c_str(), cfg.DSPVarFile.c_str(), cfg.numModules,
                                  boot_pattern),
                "Pixie16BootModule", "INFO - Finished booting!"))
            return EXIT_FAILURE;
        if (boot)
            return EXIT_SUCCESS;
    }

    bool has_written_data = false;
    LOG(INFO) << "User requested that we perform our tests " << args::get(loop_flag) << " time(s).";
    for (unsigned int test_number = 0; test_number < args::get(loop_flag); test_number++) {
        LOG(INFO) << "Starting test number " << test_number;
        if (dsp) {
            LOG(INFO) << "Starting DSP test";
            auto data = prepare_data_to_write(args::get(data_pattern_flag), args::get(data_size_flag));

            if (write && !has_written_data) {
                LOG(INFO) << "Performing a write to memory address " << args::get(address_flag)
                          << " with a size of " << args::get(data_size_flag) << " on Module "
                          << args::get(module_number_flag);
                if (!is_dry_run)
                    Pixie_DSP_Memory_IO(data.data(), address, args::get(data_size_flag),
                                        static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::WRITE),
                                        args::get(module_number_flag));
                if(one_write)
                    has_written_data = true;
            }

            if (read) {
                LOG(INFO) << "Performing a read from memory address " << args::get(address_flag) << " with a size of "
                          << args::get(data_size_flag) << " on Module " << args::get(module_number_flag);
                if (!is_dry_run) {
                    std::vector<unsigned int> read_data(args::get(data_size_flag), 0);
                    if (burst_read) {
                        LOG(INFO) << "Performing the burst read from memory address " << args::get(address_flag);
                        Pixie_DSP_Memory_IO(read_data.data(), address, args::get(data_size_flag),
                                            static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::READ),
                                            args::get(module_number_flag));
                    } else {
                        LOG(INFO) << "Performing a word-by-word read from memory address " << args::get(address_flag);
                        for (unsigned int word = 0; word < args::get(data_size_flag); word++) {
                            Pixie_DSP_Memory_IO(&read_data[word], address + word, 1,
                                                static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::READ),
                                                args::get(module_number_flag));
                        }
                    }

                    auto error_count = verify_data_read(data.data(), read_data.data(), args::get(module_number_flag),
                                                        args::get(data_size_flag));
                    if (error_count == 0)
                        LOG(INFO) << "Data read was the same as data written!";
                    if (verbose) {
                        LOG(INFO) << "Outputting read data to terminal:";
                        std::cout << std::hex;
                        for (const auto& it : read_data) {
                            std::cout << it;
                            std::cout << std::dec;
                        }
                    }
                }
            }
        }

        if (mca) {
            LOG(INFO) << "Performing a test with the MCA";

            auto data = prepare_data_to_write(args::get(data_pattern_flag), 32768);

            if (write && !has_written_data) {
                if (!is_dry_run) {
                    for (unsigned int channel = 0; channel < NUMBER_OF_CHANNELS; channel++) {
                        LOG(INFO) << "Writing Channel " << channel << "'s MCA spectrum on Module "
                                  << args::get(module_number_flag) << ".";
                        if (!verify_api_return_value(
                                    Pixie_Main_Memory_IO(
                                            data.data(), data.size() * channel, data.size(),
                                            static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::WRITE),
                                            args::get(module_number_flag)),
                                    "Pixie_Main_Memory_IO", "OK")) {
                            LOG(ERROR) << " Had a problem writing the MCA spectrum for Channel "
                                       << channel << "! Aborting!";
                            return EXIT_FAILURE;
                        }
                    }
                }
                if(one_write)
                    has_written_data = true;
            }

            if (read) {
                LOG(INFO) << "Reading MCA spectrum from Module " << args::get(module_number_flag);
                if (!is_dry_run) {
                    for (unsigned int channel = 0; channel < NUMBER_OF_CHANNELS; channel++) {
                        LOG(INFO) << "Reading Channel " << channel << "'s MCA spectrum on Module "
                                  << args::get(module_number_flag) << ".";

                        std::vector<unsigned int> read_data(data.size(), 0x1a1a1a1a);

                        if (!verify_api_return_value(
                                    Pixie_Main_Memory_IO(
                                            read_data.data(), address, data.size(),
                                            static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::READ),
                                            args::get(module_number_flag)),
                                    "Pixie_Main_Memory_IO", "OK")) {
                            LOG(ERROR) << " Had a problem reading the MCA spectrum for Channel "
                                       << channel << "! Aborting!";
                            return EXIT_FAILURE;
                        }

                        auto error_count = verify_data_read(data.data(), read_data.data(),
                                                            args::get(module_number_flag), data.size());
                        if (error_count == 0)
                            LOG(INFO) << "Data read was the same as data written!";
                        if (verbose) {
                            LOG(INFO) << "Outputting read data to terminal:";
                            std::cout << std::hex;
                            for (const auto& it : read_data)
                                std::cout << it << std::dec;
                        }
                    }
                }
            }
            if (!is_dry_run && clear) {
                LOG(INFO) << "Clearing MCA memory";
                if (!verify_api_return_value(
                            Pixie_Clear_Main_Memory(0,
                                                    32768 * NUMBER_OF_CHANNELS,
                                                    args::get(module_number_flag)),
                                             "Pixie Clear Main Memory")) {
                    LOG(ERROR) << "Couldn't clear the main memory in Module " << args::get(module_number_flag);
                    return EXIT_FAILURE;
                }
            }
        }

        if (raw) {
            LOG(INFO) << "Performing a test with raw memory access.";
            if (write) {
                if (!data_flag) {
                    LOG(ERROR) << " Must define the data that you'd like to write!";
                    return EXIT_FAILURE;
                }
                unsigned int data = stoul(args::get(data_flag), nullptr, 0);

                LOG(INFO) << "Writing " << args::get(data_flag) << " to " << args::get(address_flag) << " in Module "
                          << args::get(module_number_flag);
                if (!is_dry_run)
                    Pixie_Register_IO(args::get(module_number_flag), address,
                                      static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::WRITE), &data);
            }

            if (read) {
                unsigned int data;
                LOG(INFO) << "Reading from " << args::get(address_flag) << " in Module "
                          << args::get(module_number_flag);
                if (!is_dry_run) {
                    Pixie_Register_IO(args::get(module_number_flag), address,
                                      static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::READ), &data);
                    LOG(INFO) << "Read " << std::showbase << std::hex << data << " from "
                              << args::get(address_flag) << " in Module " << std::dec
                              << args::get(module_number_flag);
                } else {
                    LOG(INFO) << "Dry run active didn't perform the read!";
                }
            }
        }

        if (external_fifo) {
            LOG(INFO) << "Performing a test with the External FIFO";
            unsigned int number_of_words_in_fifo = 0;
            Pixie_Read_ExtFIFOStatus(&number_of_words_in_fifo, args::get(module_number_flag));
            LOG(INFO) << "Number of 32-bit words in the external FIFO of Module" << args::get(module_number_flag)
                      << ": " << number_of_words_in_fifo;
            if (read && number_of_words_in_fifo > 0) {
                std::vector<unsigned int> data = {number_of_words_in_fifo, 0};
                if (!verify_api_return_value(
                            Pixie_ExtFIFO_Read(data.data(), number_of_words_in_fifo, args::get(module_number_flag)),
                            "Pixie_ExtFIFO_Read", ""))
                    return EXIT_FAILURE;
                LOG(INFO) << "Read " << number_of_words_in_fifo << " 32-bit words from the External FIFO.";
            } else {
                LOG(INFO) << "External FIFO doesn't have anything to read!";
            }
        }

        if (csr) {
            LOG(INFO) << "CSR on Module " << args::get(module_number_flag) << ".";
            if (write) {
                if (args::get(data_flag) == "0x70FFE3") {
                    LOG(ERROR) << " Must define the data that you'd like to write!";
                    return EXIT_FAILURE;
                }
                unsigned int data = stoul(args::get(data_flag), nullptr, 0);
                LOG(INFO) << "Writing " << args::get(data_flag) << " to CSR in Module "
                          << args::get(module_number_flag);
                Pixie_WrtCSR(args::get(module_number_flag), data);
            }
            if (read) {
                unsigned int data;
                LOG(INFO) << "Reading from to CSR in Module " << args::get(module_number_flag);
                Pixie_ReadCSR(args::get(module_number_flag), &data);
                LOG(INFO) << "Read " << data << " from Module " << args::get(module_number_flag) << " CSR.";
            }
        }
        LOG(INFO) << "Completed test number " << test_number;
        if (args::get(loop_flag) > 1 && test_number != args::get(loop_flag) - 1) {
            LOG(INFO) << "Will now sleep for 1 second before starting next test.";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
