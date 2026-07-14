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

/** @file acq_mca.cpp
 * @brief Starts a 10 second MCA data run
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

void take_snapshot(std::ofstream& stats_file, const module_config& mcfg) {
    auto stats_size = ::Pixie16GetStatisticsSize();
    std::vector<unsigned int> stats(stats_size, 0);
    json snapshot;

    snapshot["timestamp"] = walltime_iso_string();

    auto r = verify_api_return_value(::Pixie16ReadStatisticsFromModule(stats.data(), 0),
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

    // Write to file and flush immediately to ensure data isn't lost if the app crashes
    stats_file << snapshot.dump() << "\n";
    stats_file.flush();
}

void mca_monitor(const std::atomic_bool& run_active, const module_config& mcfg) {
    std::ofstream stats_file("module0-mca.jsonl");

    while (run_active) {
        // Segmented sleep to remain responsive to run_active state changes
        for (int i = 0; i < 10 && run_active; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (run_active) {
            take_snapshot(stats_file, mcfg);
        }
    }

    // Final snapshot requested after run ends
    take_snapshot(stats_file, mcfg);
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

    /*
     * Tell the module that we want an MCA data run with a 10 second length.
     * All MCA data runs are timed using HOST_RT_PRESET, which is an IEEE-754 floating point number.
     * You can set an "indefinite" run length by making this very large (Ex. 604800 seconds = 1 week).
     */
    constexpr auto runtime_s = 10.;
    const auto runtime_ieee = Decimal2IEEEFloating(runtime_s);
    std::cout << logging("INFO") << "Set HOST_RT_PRESET to " << runtime_s << " s -> "
              << runtime_ieee << std::endl;
    r = verify_api_return_value(::Pixie16WriteSglModPar("HOST_RT_PRESET", runtime_ieee, 0),
                                "Pixie16WriteSglModPar - HOST_RT_PRESET");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Start the data run
     */
    std::cout << logging("INFO") << "Pixie16StartHistogramRun" << std::endl;
    r = verify_api_return_value(::Pixie16StartHistogramRun(0, 1), "Pixie16StartHistogramRun",
                                false);
    if (!r) {
        return EXIT_FAILURE;
    }

    //Initialize the thread control variables
    std::atomic_bool run_active{true};

    // Spawn the background worker thread
    std::thread mca_monitor_thread(mca_monitor, std::ref(run_active), std::ref(mcfg));

    const auto run_start_time = std::chrono::steady_clock::now();
    double current_run_time = 0;
    double check_time = 0;
    bool run_status = ::Pixie16CheckRunStatus(0);
    auto forced_end = false;

    // Execute the DAQ loop
    while (run_status != 0 && !forced_end) {
        current_run_time = std::chrono::duration_cast<std::chrono::duration<double>>(
                               std::chrono::steady_clock::now() - run_start_time)
                               .count();

        /*
         * During the data run we'll check that the run is still active and do any sort of
         * data management that we may want. For example, you can read out the hardware
         * statistics or MCA histogram here.
         */
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
        if (current_run_time > runtime_s + 5) {
            std::lock_guard<std::mutex> lock(console_mutex);
            std::cout << logging("INFO") << "Forcing end of run in Module 0" << std::endl;
            r = verify_api_return_value(::Pixie16EndRun(0), "Pixie16EndRun");
            if (!r) {
                run_active = false;
                if (mca_monitor_thread.joinable()) {
                    mca_monitor_thread.join();
                }
                return EXIT_FAILURE;
            }
            forced_end = true;
        }
    }

    run_active = false;
    if (mca_monitor_thread.joinable()) {
        mca_monitor_thread.join();
    }

    if (current_run_time < runtime_s) {
        std::cout << logging("ERROR") << "run exited prematurely" << std::endl;
    }

    if (forced_end) {
        std::cout << logging("ERROR") << "forced to end run" << std::endl;
    }

    ::Pixie16ExitSystem(static_cast<unsigned short>(args.slots.size()));
    return EXIT_SUCCESS;
}