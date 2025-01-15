/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2025 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file support-1859.cpp
 * @brief Recreating the list-mode run behavior observed in the customer's log file
 */
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <args/args.hxx>
#include <nolhmann/json.hpp>
#include "pixie16/pixie16.h"

std::string fifo_stats_to_json(module_fifo_stats& stats) {
    nlohmann::json data = {
        {"timestamp", std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::system_clock::now().time_since_epoch())
                          .count()},
        {"in", stats.in},
        {"out", stats.out},
        {"dma_in", stats.dma_in},
        {"overflows", stats.overflows},
        {"dropped", stats.dropped},
        {"hw_overflows", stats.hw_overflows},
    };
    return data.dump();
}

using args_parser = args::ArgumentParser;
using args_group = args::Group;
using args_flag = args::Flag;
using args_int_flag = args::ValueFlag<int>;
using args_string_flag = args::ValueFlag<std::string>;

int main(int argc, char* argv[]) {

    args_parser parser("List-Mode Test ");

    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args_group option_group(parser, "Options");
    args_int_flag num_mods_flag(
        option_group, "num_mods_flag",
        "Number of modules", {'n', "num-mods"});
    args_int_flag run_length_flag(
        option_group, "run_length_flag",
        "Length of list-mode run, in seconds", {'t', "time"});
    args_string_flag log_file_flag(
        option_group, "log_file_flag",
        "Log file", {'l', "log"});
    args_string_flag settings_file_flag(
        option_group, "settings_file_flag",
        "Settings file", {'s', "settings"});

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

    std::string log_name = "./support-1859_output.log";
    if (log_file_flag) {
        log_name = args::get(log_file_flag);
    }

    std::string input_settings;
    if (settings_file_flag) {
        input_settings = args::get(settings_file_flag);
    }

    int num_mods = 6;
    if (num_mods_flag) {
        num_mods = args::get(num_mods_flag);
    }
    int run_length = 300;
    if (run_length_flag) {
        run_length = args::get(run_length_flag);
    }
    int retval;
    retval = Pixie16InitSystem(num_mods, NULL, 0);
    std::cout << "Pixie16InitSystem: " << retval << std::endl;

    retval = PixieBootCrate(input_settings.c_str(), PIXIE_BOOT_RESET_LOAD);
    std::cout << "PixieBootCrate: " << retval << std::endl;

    retval = Pixie16StartListModeRun(num_mods, 0x100, 1);
    std::cout << "Pixie16StartListModeRun: " << retval << std::endl;

    std::ofstream fifo_stat_stream("support-1859-stats.json");

    unsigned int num_fifo_words = 0;
    ::module_fifo_stats fifo_stats{};
    auto run_start_time = std::chrono::steady_clock::now();
    double current_run_time = 0;
    double check_time = 0;
    bool run_status = ::Pixie16CheckRunStatus(0);
    bool stop = false;

    while (run_status != 0) {
        current_run_time = std::chrono::duration_cast<std::chrono::duration<double>>(
                               std::chrono::steady_clock::now() - run_start_time)
                               .count();

        if (current_run_time >= run_length) {
            retval = ::Pixie16EndRun(0);
            std::cout << "Pixie16EndRun: " << retval << std::endl;
            break;
        }

        if (current_run_time - check_time > 5) {
            if (current_run_time < run_length) {
                std::cout << "Remaining run time: "
                          << std::round(run_length - current_run_time) << " s" << std::endl;
            }
            check_time = current_run_time;
        }

        if (current_run_time >= (run_length - 20)) {
            stop = true;
        }

        for (unsigned short mod_num = 0; mod_num < static_cast<unsigned short>(num_mods); mod_num++) {
            if (stop) {
                const auto start = std::chrono::high_resolution_clock::now();
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                const auto end = std::chrono::high_resolution_clock::now();
                const std::chrono::duration<double, std::milli> elapsed = end - start;
                std::cout << "Waited " << elapsed.count() << "ms" << std::endl;
            }
            ::Pixie16CheckExternalFIFOStatus(&num_fifo_words, mod_num);

            std::cout << "Module " << mod_num << " FIFO has " << num_fifo_words
                      << " words." << std::endl;

            if (num_fifo_words > 0) {
                std::vector<uint32_t> data(num_fifo_words);
                ::Pixie16ReadDataFromExternalFIFO(data.data(), num_fifo_words, mod_num);
                std::cout << "Pixie16ReadDataFromExternalFIFO: " << mod_num << ": " << num_fifo_words
                          << std::endl;
            }

            ::PixieReadRunFifoStats(mod_num, &fifo_stats);
            fifo_stat_stream << fifo_stats_to_json(fifo_stats) << std::endl;
            num_fifo_words = 0;
        }
        run_status = ::Pixie16CheckRunStatus(0);
    }
    retval = ::Pixie16EndRun(0);
    std::cout << "Pixie16EndRun: " << retval << std::endl;
    fifo_stat_stream.close();

    retval = Pixie16ExitSystem(static_cast<unsigned short>(num_mods));
    std::cout << "Pixie16ExitSystem: " << retval << std::endl;

    std::stringstream mv_log_cmd;
    mv_log_cmd << "mv Pixie16Msg.log " << log_name;
    return system(mv_log_cmd.str().c_str());
}
