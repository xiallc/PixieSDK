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

/** @file acq_baselines.cpp
 * @brief Executes a data run that acquires the ADC baseline information for each channel.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <pixie16/pixie16.h>

#include <example_utils.hpp>

/*
 * We'll define some useful conventions here to make access clearer later.
 */
using data_value = double;
using baseline_value = std::vector<data_value>;
using baseline_values = std::vector<baseline_value>;
using timestamp_value = std::vector<data_value>;
using timestamp_values = std::vector<timestamp_value>;

int main(int argc, char** argv) {
    CliArguments args = ParseCliArguments(argc, argv);

    auto r = InitializeAndBootSystem(args);
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Tell the module to acquire baselines.
     */
    std::cout << logging("INFO") << "Pixie16AcquireBaselines start" << std::endl;
    r = verify_api_return_value(::Pixie16AcquireBaselines(0), "Pixie16AcquireBaselines");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Get the module information so that we know how many channels the module has.
     */
    module_config mcfg{};
    std::cout << logging("INFO") << "PixieGetModuleInfo start" << std::endl;
    r = verify_api_return_value(::PixieGetModuleInfo(0, &mcfg), "PixieGetModuleInfo");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Acquire the baseline measurements for each channel.
     * Loop over module number to do all modules in the system.
     */
    baseline_values baselines;
    timestamp_values timestamps;
    unsigned int max_num_baselines = 0;
    for (unsigned short i = 0; i < mcfg.number_of_channels; i++) {
        ::PixieGetMaxNumBaselines(0, i, &max_num_baselines);
        baseline_value baseline(max_num_baselines);
        timestamp_value timestamp(max_num_baselines);

        std::cout << logging("INFO") << "Acquiring " << max_num_baselines
                  << " baselines for Channel " << i << std::endl;
        r = verify_api_return_value(::Pixie16ReadSglChanBaselines(baseline.data(), timestamp.data(),
                                                                  max_num_baselines, 0, i),
                                    "Pixie16ReadSglChanBaselines");
        if (!r) {
            return EXIT_FAILURE;
        }
        baselines.push_back(baseline);
        timestamps.push_back(timestamp);
    }

    std::ofstream bcsv("module0-baselines.csv");
    bcsv << "sample,time,";
    for (unsigned short i = 0; i < mcfg.number_of_channels; i++) {
        if (i != mcfg.number_of_channels - 1) {
            bcsv << "Chan" << i << ",";
        } else {
            bcsv << "Chan" << i;
        }
    }
    bcsv << std::endl;

    for (unsigned int i = 0; i < max_num_baselines; i++) {
        bcsv << i << "," << timestamps[0][i] << ",";
        for (unsigned short k = 0; k < mcfg.number_of_channels; k++) {
            if (k != mcfg.number_of_channels - 1) {
                bcsv << baselines[k][i] << ",";
            } else {
                bcsv << baselines[k][i];
            }
        }
        bcsv << std::endl;
    }
    bcsv.close();

    ::Pixie16ExitSystem(static_cast<unsigned short>(args.slots.size()));
    return EXIT_SUCCESS;
}