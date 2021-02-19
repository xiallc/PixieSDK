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

int clear_external_memory(const int& number_of_modules) {
    cout << "INFO - Starting to clear the External Memory...";
    for (int modnum = 0; modnum < number_of_modules; modnum++) {
        if (verify_api_return_value(Pixie_Clear_Main_Memory(0, 32768 * 16, modnum),
                                    "Pixie Clear Main Memory")) {
            return -1;
        }
    }
    return 0;
}

int verify_data_read(const unsigned int* expected, const unsigned int* returned,
                     const int& module_number, const unsigned int& size) {
    char ErrMSG[256];
    unsigned int error_count = 0;
    for (unsigned int idx = 0; idx < size; idx++) {
        if (expected[idx] != returned[idx]) {
            sprintf(ErrMSG,
                    "RD_WR data mismatch in module %d, rd_data=0x%x, wr_data=0x%x, count=%d",
                    module_number, returned[idx], expected[idx], idx);
            Pixie_Print_MSG(ErrMSG);
            error_count++;
        }
    }
    return error_count;
}

int test_external_memory_reads_and_writes(const int& number_of_modules,
                                          const std::size_t& number_of_loops,
                                          const std::string& write_type) {
    char ErrMSG[256];
    unsigned int rd_wr_errors = 0;
    unsigned int rd_data[65536];
    unsigned int errorcounts = 0;

    vector<std::array<unsigned int, 65536> > test_data;

    if (write_type == "single") {
        for (int segment = 0; segment < 8; segment++) {
            auto data = prepare_data_to_write(DATA_PATTERN::RAMP_UP);
            for (int modnum = 0; modnum < number_of_modules; modnum++) {
                if (!verify_api_return_value(
                        Pixie_Main_Memory_IO(data.begin(), 65536 * segment, 65536, MOD_WRITE,
                                             modnum), "Pixie_Main_Memory_IO", "")) {
                    return (-1);
                }
            }
        }
        test_data = {prepare_data_to_write(DATA_PATTERN::RAMP_UP)};
    } else if (write_type == "clear") {
        clear_external_memory(number_of_modules);
        test_data = {prepare_data_to_write(DATA_PATTERN::ZERO)};
    } else {
        test_data = {prepare_data_to_write(DATA_PATTERN::HI_LO),
                     prepare_data_to_write(DATA_PATTERN::FLIP_FLOP),
                     prepare_data_to_write(DATA_PATTERN::RAMP_UP),
                     prepare_data_to_write(DATA_PATTERN::RAMP_DOWN)};
    }


    for (std::size_t loop_num = 0; loop_num < number_of_loops; loop_num++) {
        for (auto data: test_data) {
            for (int modnum = 0; modnum < number_of_modules; modnum++) {
                for (int emwrites = 0; emwrites < 8; emwrites++) {
                    if (write_type == "repeated") {
                        if (!verify_api_return_value(
                                Pixie_Main_Memory_IO(data.begin(), 65536 * emwrites, 65536,
                                                     MOD_WRITE, modnum),
                                "Pixie_Main_Memory_IO", ""))
                            return -1;
                    }

                    for (unsigned int idx = 0; idx < 65535; idx++) {
                        if (write_type != "clear")
                            rd_data[idx] = 0;
                        else
                            rd_data[idx] = 0xA5A5A5A5;
                    }

                    if (!verify_api_return_value(
                            Pixie_Main_Memory_IO(rd_data, 65536 * emwrites, 65536, MOD_READ,
                                                 modnum),
                            "Pixie_Main_Memory_IO", ""))
                        return -1;

                    rd_wr_errors += verify_data_read(data.begin(), rd_data, modnum, 65536);
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
        } // for (auto data:test-data
        printf("INFO - Number of External Memory write & read tests = %lu, error counts = %d\n", loop_num,
               errorcounts);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }// for (std::size_t loop_num
    return 0;
}


int test_dsp_reads_and_writes(const int& number_of_modules, const std::size_t& number_of_loops,
                              const std::string& write_type, const string& read_type) {
    char ErrMSG[256];
    unsigned int rd_wr_errors = 0;
    unsigned int rd_data[65536];
    unsigned int errorcounts = 0;
    vector<DATA_PATTERN> test_data_patterns = {DATA_PATTERN::HI_LO, DATA_PATTERN::FLIP_FLOP,
                                               DATA_PATTERN::RAMP_UP, DATA_PATTERN::RAMP_DOWN};

    for (std::size_t loop_num = 0; loop_num < number_of_loops; loop_num++) {
        for (auto data_pattern: test_data_patterns) {
            auto wr_data = prepare_data_to_write(data_pattern);

            for (int modnum = 0; modnum < number_of_modules; modnum++) {
                if (!verify_api_return_value(
                        Pixie_DSP_Memory_IO(wr_data.begin(), 0x50000, 65535, MOD_WRITE, modnum),
                        "Pixie_DSP_Memory_IO", ""))
                    return -1;

                if (read_type == "burst")
                    if (!verify_api_return_value(
                            Pixie_DSP_Memory_IO(rd_data, 0x50000, 65535, MOD_READ, modnum),
                            "Pixie_DSP_Memory_IO", ""))
                        return -1;

                for (unsigned int count = 0; count < 65535; count++) {
                    if (read_type == "burst") {
                        if (wr_data[count] != rd_data[count]) {
                            sprintf(ErrMSG,
                                    "RD_WR data mismatch in module %d, rd_data=0x%x, wr_data=0x%x, count=%d",
                                    modnum, rd_data[count], wr_data[count], count);
                            Pixie_Print_MSG(ErrMSG);
                            rd_wr_errors++;
                        }
                    } else {
                        if (!verify_api_return_value(
                                Pixie_DSP_Memory_IO(&rd_data[count], 0x50000 + count, 1, MOD_READ,
                                                    modnum),
                                "Pixie_DSP_Memory_IO", ""))
                            return (-1);
                        if (wr_data[count] != rd_data[count]) {
                            sprintf(ErrMSG,
                                    "RD_WR data mismatch in module %d, rd_data=0x%x, wr_data=0x%x, count=%d",
                                    modnum, rd_data[count], wr_data[count], count);
                            Pixie_Print_MSG(ErrMSG);
                            rd_wr_errors++;
                        }
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
        printf("INFO - Number of DSP write & read tests = %lu, error counts = %d\n", loop_num,
               errorcounts);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}

int main(int argc, char* argv[]) {

    static const int offline_mode = 0;

    args::ArgumentParser parser("System test code ");
    parser.LongSeparator("=");

    args::Group commands(parser, "commands");
    args::Command dsp(commands, "dsp", "Tests related to the DSP.");
    args::Command external_memory(commands, "external_memory",
                                  "Tests related to the external memory.");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::Positional<std::string> configuration(arguments, "cfg", "The configuration file to load.",
                                                args::Options::Required);
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});

    args::ValueFlag<std::size_t> num_loops(arguments, "num_loops",
                                           "The number of loops that we'll use in our tests",
                                           {'l', "loops"}, 10);
    args::ValueFlag<string> read_type(arguments, "read_type",
                                      "The type of read we'll do from the DSP - burst (default) or single.",
                                      {'r', "read_type"}, "burst");
    args::ValueFlag<string> write_type(arguments, "write_type",
                                       "The type of write we'll do - single (default), repeated, clear.",
                                       {'w', "write_type"}, "single");

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
            "Pixie16BootModule", "INFO - Finished booting!"))
        return EXIT_FAILURE;

    cout << "INFO - Performing a " << args::get(write_type) << " Write with "
         << args::get(read_type) << " Reads on " << cfg.numModules << " using "
         << args::get(num_loops) << " and the ";
    if (dsp) {
        cout << "DSP" << endl;
        if (args::get(write_type) == "clear") {
            cout << "ERROR - Clear is not a valid write type for the DSP." << endl;
            return -1;
        }
        test_dsp_reads_and_writes(cfg.numModules, args::get(num_loops),
                                  args::get(write_type), args::get(read_type));
    }
    if (external_memory) {
        cout << "External Memory" << endl;
        test_external_memory_reads_and_writes(cfg.numModules, args::get(num_loops),
                                              args::get(write_type));
    }
}
