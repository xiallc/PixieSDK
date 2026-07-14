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

/** @file init.cpp
 * @brief Initializes the provided list of slots, prints the module information and exits.
 */

#include <iostream>
#include <string>
#include <vector>

#include <pixie16/pixie16.h>

#include <example_utils.hpp>

int main(int argc, char** argv) {
    CliArguments args = ParseCliArguments(argc, argv);

    if (args.slots.empty()) {
        std::cerr << "Error: At least one slot must be provided." << std::endl;
        return EXIT_FAILURE;
    }

    unsigned short num_modules = static_cast<unsigned short>(args.slots.size());

    std::cout << logging("INFO") << "Pixie16InitSystem start" << std::endl;
    auto r = verify_api_return_value(::Pixie16InitSystem(num_modules, args.slots.data(), 0),
                                      "Pixie16InitSystem");
    if (!r) {
        return EXIT_FAILURE;
    }

    for (unsigned short i = 0; i < num_modules; i++) {
        module_config mcfg{};
        r = verify_api_return_value(::PixieGetModuleInfo(i, &mcfg), "PixieGetModuleInfo", false);
        if (!r) {
            std::cout << logging("INFO") << "Hardware Information for Module " << i << ":\n"
                      << "\tSlot:           " << mcfg.slot << "\n"
                      << "\tSerial Number:  " << mcfg.serial_number << "\n"
                      << "\tRevision:       " << mcfg.revision << "\n"
                      << "\tADC Bits:       " << mcfg.adc_bit_resolution << "\n"
                      << "\tADC MSPS:       " << mcfg.adc_sampling_frequency << "\n"
                      << "\tChannels:       " << mcfg.number_of_channels << std::endl;
        }
    }

    ::Pixie16ExitSystem(num_modules);
    return EXIT_SUCCESS;
}