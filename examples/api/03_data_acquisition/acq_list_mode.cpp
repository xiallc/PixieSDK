/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2026 XIA LLC, All rights reserved.
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

/** @file acq_list_mode.cpp
 * @brief Starts a list-mode data run with a 10-second duration.
 */

#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <example_utils.hpp>
#include <nolhmann/json.hpp>
#include <pixie16/pixie16.h>

using json = nlohmann::json;

// Mutex to prevent standard output text garbling from multiple threads
std::mutex console_mutex;

void read_fifo(unsigned int& num_fifo_words, std::ofstream& lm_file) {
    auto r = verify_api_return_value(::Pixie16CheckExternalFIFOStatus(&num_fifo_words, 0),
                                     "Pixie16CheckExternalFIFOStatus", false);
    // If the check fails then we need to ensure that num_fifo_words reflects that.
    if (!r) {
        num_fifo_words = 0;
    }

    if (num_fifo_words > 0) {
        // Init a data array to hold the data with a default value for error handling.
        std::vector<uint32_t> data(num_fifo_words, 0xDEADBEEF);
        r = verify_api_return_value(
            ::Pixie16ReadDataFromExternalFIFO(data.data(), num_fifo_words, 0),
            "Pixie16ReadDataFromExternalFIFO", false);
        if (r) {
            lm_file.write(reinterpret_cast<char*>(data.data()),
                          static_cast<std::streamsize>(num_fifo_words * sizeof(uint32_t)));
        }
    }
}

void lm_spool(const std::atomic_bool& run_active) {
    std::ofstream lm_file("module0-listmode.lmd", std::ios::out | std::ios::binary);
    unsigned int num_fifo_words = 0;

    while (run_active) {
        read_fifo(num_fifo_words, lm_file);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    while (num_fifo_words > 0) {
        read_fifo(num_fifo_words, lm_file);
    }

    lm_file.close();
}

void take_snapshot(std::ofstream& stats_file, const module_config& mcfg) {
    auto stats_size = ::Pixie16GetStatisticsSize();
    std::vector<unsigned int> stats(stats_size, 0);
    ::module_fifo_stats fifo_stats{};
    json snapshot;

    snapshot["timestamp"] = walltime_iso_string();

    auto r = verify_api_return_value(::PixieReadRunFifoStats(0, &fifo_stats),
                                     "PixieReadRunFifoStats", false);
    if (r) {
        snapshot["fifo_stats"] = {
            {"in", fifo_stats.in},           {"out", fifo_stats.out},
            {"dma_in", fifo_stats.dma_in},   {"overflows", fifo_stats.overflows},
            {"dropped", fifo_stats.dropped}, {"hw_overflows", fifo_stats.hw_overflows}};
    }

    r = verify_api_return_value(::Pixie16ReadStatisticsFromModule(stats.data(), 0),
                                "Pixie16ReadStatisticsFromModule", false);
    if (r) {
        snapshot["real_time"] = ::Pixie16ComputeRealTime(stats.data(), 0);

        json channels = json::object();
        for (unsigned short ch = 0; ch < mcfg.number_of_channels; ++ch) {
            json ch_data;
            ch_data["live_time"] = ::Pixie16ComputeLiveTime(stats.data(), 0, ch);
            ch_data["icr"] = ::Pixie16ComputeInputCountRate(stats.data(), 0, ch);
            ch_data["ocr"] = ::Pixie16ComputeOutputCountRate(stats.data(), 0, ch);

            unsigned int hist_length = 0;
            r = verify_api_return_value(::PixieGetHistogramLength(0, ch, &hist_length),
                                        "PixieGetHistogramLength", false);
            if (r && hist_length > 0) {
                std::vector<uint32_t> hist(hist_length, 0);
                ::Pixie16ReadHistogramFromModule(hist.data(), hist_length, 0, ch);
                ch_data["histogram"] = hist;
            }
            channels[std::to_string(ch)] = ch_data;
        }
        snapshot["channels"] = channels;
    }

    stats_file << snapshot.dump() << "\n";
    stats_file.flush();
}

void stats_monitor(const std::atomic_bool& run_active, const module_config& mcfg) {
    std::ofstream stats_file("module0-listmode-stats.jsonl");

    while (run_active) {
        // Segmented sleep to remain responsive to run_active state changes
        for (int i = 0; i < 10 && run_active; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (run_active) {
            take_snapshot(stats_file, mcfg);
        }
    }

    take_snapshot(stats_file, mcfg);
    stats_file.close();
}

int main(int argc, char** argv) {
    CliArguments args = ParseCliArguments(argc, argv);

    auto r = InitializeAndBootSystem(args);
    if (!r) {
        return EXIT_FAILURE;
    }

    // Get the module information so that we know how many channels the module has.
    module_config mcfg{};
    std::cout << logging("INFO") << "PixieGetModuleInfo start" << std::endl;
    r = verify_api_return_value(::PixieGetModuleInfo(0, &mcfg), "PixieGetModuleInfo");
    if (!r) {
        return EXIT_FAILURE;
    }

    // Configure the module so that it does not try to start a synchronous data run.
    r = verify_api_return_value(::Pixie16WriteSglModPar("SYNCH_WAIT", 0, 0),
                                "Pixie16WriteSglModPar - SYNCH_WAIT");
    if (!r) {
        return EXIT_FAILURE;
    }

    // Configure the module so that it clears the FIPPI timestamps at the start of the run.
    r = verify_api_return_value(::Pixie16WriteSglModPar("IN_SYNCH", 0, 0),
                                "Pixie16WriteSglModPar - IN_SYNCH");
    if (!r) {
        return EXIT_FAILURE;
    }

    // Start the data run
    std::cout << logging("INFO") << "Pixie16StartListModeRun" << std::endl;
    r = verify_api_return_value(::Pixie16StartListModeRun(0, 0x100, 1), "Pixie16StartListModeRun",
                                false);
    if (!r) {
        return EXIT_FAILURE;
    }

    //Initialize the thread control variable
    std::atomic_bool run_active{true};

    // thread for writing binary list-mode data to disk.
    std::thread lm_spooler_thread(lm_spool, std::ref(run_active));

    // Thread for monitoring the stats and MCA.
    std::thread stats_monitor_thread(stats_monitor, std::ref(run_active), std::ref(mcfg));

    auto runtime_s = 10.;
    auto run_start_time = std::chrono::steady_clock::now();
    double current_run_time = 0;
    double check_time = 0;
    bool run_status = ::Pixie16CheckRunStatus(0);

    // Execute the DAQ loop
    while (run_status != 0) {
        current_run_time = std::chrono::duration_cast<std::chrono::duration<double>>(
                               std::chrono::steady_clock::now() - run_start_time)
                               .count();

        if (current_run_time >= runtime_s) {
            std::lock_guard<std::mutex> lock(console_mutex);
            std::cout << logging("INFO") << "Stopping list-mode run." << std::endl;
            verify_api_return_value(::Pixie16EndRun(0), "Pixie16EndRun");
            break;
        }

        if (current_run_time - check_time > 1) {
            run_status = ::Pixie16CheckRunStatus(0);
            if (current_run_time < runtime_s) {
                std::lock_guard<std::mutex> lock(console_mutex);
                std::cout << logging("INFO")
                          << "Remaining run time: " << runtime_s - current_run_time << " s"
                          << std::endl;
            }
            check_time = current_run_time;
        }
    }

    if (current_run_time < runtime_s) {
        std::cout << logging("ERROR") << "run exited prematurely" << std::endl;
    }

    std::cout << logging("INFO") << "Run finished. Finalizing output and threads. " << std::endl;

    run_active = false;
    if (lm_spooler_thread.joinable()) {
        lm_spooler_thread.join();
    }

    if (stats_monitor_thread.joinable()) {
        stats_monitor_thread.join();
    }


    ::Pixie16ExitSystem(static_cast<unsigned short>(args.slots.size()));
    return EXIT_SUCCESS;
}