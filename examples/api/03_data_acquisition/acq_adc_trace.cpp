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

/** @file acq_adc_trace.cpp
 * @brief Reads an ADC trace for all channels in the module.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <pixie16/pixie16.h>

#include <example_utils.hpp>

/*
 * We'll define some useful conventions here to make access clearer later.
 */
using trace_value = std::vector<unsigned short>;
using trace_values = std::vector<trace_value>;

int main(int argc, char** argv) {
    CliArguments args = ParseCliArguments(argc, argv);

    auto r = InitializeAndBootSystem(args);
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Tell the module to acquire ADC traces for all channels.
     */
    std::cout << logging("INFO") << "Pixie16AcquireADCTrace start" << std::endl;
    r = verify_api_return_value(::Pixie16AcquireADCTrace(0), "Pixie16AcquireADCTrace");
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

    std::ofstream tcsv("module0-traces.csv");
    tcsv << "bin,";

    unsigned int max_trace_length = 0;
    trace_values traces;
    for (unsigned short i = 0; i < mcfg.number_of_channels; i++) {
        std::cout << logging("INFO") << "Acquiring trace for Channel " << i << std::endl;

        /*
         * Trace length varies based on hardware revision, get it to size arrays properly.
         */
        unsigned int trace_length = 0;
        r = verify_api_return_value(::PixieGetTraceLength(0, i, &trace_length),
                                    "Pixie16GetTraceLength");
        if (!r) {
            return EXIT_FAILURE;
        }

        // Save the max trace length we record so that we can use it later for output.
        max_trace_length = std::max(trace_length, max_trace_length);

        /*
         * Preallocate the array so the API can fill it.
         */
        trace_value trace(trace_length, 0);

        /*
         * Get the requested ADC trace from the hardware.
         */
        r = verify_api_return_value(::Pixie16ReadSglChanADCTrace(trace.data(), trace_length, 0, i),
                                    "Pixie16AcquireADCTrace");
        if (!r) {
            return EXIT_FAILURE;
        }
        traces.push_back(trace);

        if (i != mcfg.number_of_channels - 1) {
            tcsv << "Chan" << i << ',';
        } else {
            tcsv << "Chan" << i;
        }
    }
    tcsv << std::endl;

    for (unsigned int idx = 0; idx < max_trace_length; idx++) {
        tcsv << idx << ",";
        for (auto& trace : traces) {
            std::string val = " ";
            if (idx < trace.size()) {
                val = std::to_string(trace[idx]);
            }
            if (&trace != &traces.back()) {
                tcsv << val << ",";
            } else {
                tcsv << val;
            }
        }
        tcsv << std::endl;
    }
    tcsv.close();

    ::Pixie16ExitSystem(static_cast<unsigned short>(args.slots.size()));
    return EXIT_SUCCESS;
}