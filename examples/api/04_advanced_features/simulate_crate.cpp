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

/** @file simulate_crate.cpp
 * @brief Starts the crate simulation via the C API.
 */

#include <iostream>
#include <string>
#include <vector>

#include <pixie16/pixie16.h>

#include <example_utils.hpp>

int main(int argc, char** argv) {
    CliArguments args = ParseCliArguments(argc, argv);

    /*
     * Crate simulation via the C API has a fixed set of 4 modules. We ignore the CLI
     * input and override it here.
     */
    std::vector<unsigned short> slots = {2, 3, 4, 5};
    const auto num_modules = static_cast<unsigned short>(slots.size());

    /*
     * Init the system with OfflineMode argument equal to 1 to tell the system to use the
     * simulated crate.
     */
    std::cout << logging("INFO") << "Pixie16InitSystem start" << std::endl;
    auto r = verify_api_return_value(::Pixie16InitSystem(num_modules, slots.data(), 1),
                                     "Pixie16InitSystem");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Load non-standard install paths for the firmware if necessary.
     */
    std::cout << logging("INFO") << "Pixie16LoadModuleFirmware start" << std::endl;
    if (!args.firmware_path.empty()) {
        r = verify_api_return_value(::Pixie16LoadModuleFirmware(args.firmware_path.c_str()),
                                    "Pixie16LoadModuleFirmware");
        if (!r) {
            return EXIT_FAILURE;
        }
    }

    /*
     * Boot the crate as normal.
     */
    std::cout << logging("INFO") << "PixieBootCrate start" << std::endl;
    r = verify_api_return_value(::PixieBootCrate(args.parfile.c_str(), PIXIE_BOOT_RESET_LOAD),
                                "PixieBootCrate");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Loop over the modules and get their configs, also set some parameters.
     */
    for (unsigned short i = 0; i < num_modules; i++) {
        module_config mcfg{};
        r = verify_api_return_value(::PixieGetModuleInfo(i, &mcfg), "PixieGetModuleInfo", false);
        if (r) {
            std::cout << logging("INFO") << "Hardware Information for Module " << i << ":\n"
                      << "\tSlot:           " << mcfg.slot << "\n"
                      << "\tSerial Number:  " << mcfg.serial_number << "\n"
                      << "\tRevision:       " << mcfg.revision << "\n"
                      << "\tADC Bits:       " << mcfg.adc_bit_resolution << "\n"
                      << "\tADC MSPS:       " << mcfg.adc_sampling_frequency << "\n"
                      << "\tChannels:       " << mcfg.number_of_channels << std::endl;
        }

        /*
         * We'll set the CHANNEL_CSRA for Channel 0 on all modules to 32
         */
        double current_csra;
        r = verify_api_return_value(
            ::Pixie16ReadSglChanPar("CHANNEL_CSRA", &current_csra, mcfg.number, 0),
            "Pixie16ReadSglChanPar", false);
        if (!r) {
            return EXIT_FAILURE;
        }

        r = verify_api_return_value(::Pixie16WriteSglChanPar("CHANNEL_CSRA", 32, mcfg.number, 0),
                                    "Pixie16WriteSglChanPar", false);
        if (!r) {
            return EXIT_FAILURE;
        }

        double new_csra;
        r = verify_api_return_value(
            ::Pixie16ReadSglChanPar("CHANNEL_CSRA", &new_csra, mcfg.number, 0),
            "Pixie16ReadSglChanPar", false);
        if (!r) {
            return EXIT_FAILURE;
        }

        std::cout << logging("INFO") << "Mod " << mcfg.number << " Chan " << 0 << " | CHANNEL_CSRA "
                  << current_csra << " -> " << new_csra << std::endl;
    }

    ::Pixie16ExitSystem(static_cast<unsigned short>(slots.size()));
    return EXIT_SUCCESS;
}