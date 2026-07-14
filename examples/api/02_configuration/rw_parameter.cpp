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

/** @file rw_parameter.cpp
 * @brief Reads and writes channel and module parameter.
 */

#include <iostream>
#include <string>
#include <vector>

#include <pixie16/pixie16.h>

#include <example_utils.hpp>

int main(int argc, char** argv) {
    CliArguments args = ParseCliArguments(argc, argv);

    auto r = InitializeAndBootSystem(args);
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
     * Update all channels on Module 0 to have CHANNEL_CSRA = 32 and verify that it got updated.
     */
    std::cout << logging("INFO") << "Pixie16WriteSglChanPar start" << std::endl;
    for (unsigned short ch = 0; ch < mcfg.number_of_channels; ch++) {
        double current_csra;
        r = verify_api_return_value(::Pixie16ReadSglChanPar("CHANNEL_CSRA", &current_csra, 0, ch),
                                    "Pixie16ReadSglChanPar", false);
        if (!r) {
            return EXIT_FAILURE;
        }

        r = verify_api_return_value(::Pixie16WriteSglChanPar("CHANNEL_CSRA", 32, 0, ch),
                                    "Pixie16WriteSglChanPar", false);
        if (!r) {
            return EXIT_FAILURE;
        }

        double new_csra;
        r = verify_api_return_value(::Pixie16ReadSglChanPar("CHANNEL_CSRA", &new_csra, 0, ch),
                                    "Pixie16ReadSglChanPar", false);
        if (!r) {
            return EXIT_FAILURE;
        }

        std::cout << logging("INFO") << "Chan " << ch << " | CHANNEL_CSRA " << current_csra
                  << " -> " << new_csra << std::endl;
    }
    std::cout << logging("INFO") << "Pixie16WriteSglChanPar finish" << std::endl;

    /*
     * Clear Module 0's MODULE_CSRB
     */
    unsigned int current_csrb;
    std::cout << logging("INFO") << "Pixie16ReadSglModPar start" << std::endl;
    r = verify_api_return_value(::Pixie16ReadSglModPar("MODULE_CSRB", &current_csrb, 0),
                                "Pixie16ReadSglModPar");
    if (!r) {
        return EXIT_FAILURE;
    }

    std::cout << logging("INFO") << "Pixie16WriteSglModPar start" << std::endl;
    r = verify_api_return_value(::Pixie16WriteSglModPar("MODULE_CSRB", 0, 0),
                                "Pixie16WriteSglModPar");
    if (!r) {
        return EXIT_FAILURE;
    }

    unsigned int new_csrb;
    std::cout << logging("INFO") << "Pixie16ReadSglModPar start" << std::endl;
    r = verify_api_return_value(::Pixie16ReadSglModPar("MODULE_CSRB", &new_csrb, 0),
                                "Pixie16ReadSglModPar");
    if (!r) {
        return EXIT_FAILURE;
    }
    std::cout << logging("INFO") << "Mod 0 | MODULE_CSRB " << current_csrb << " -> " << new_csrb
              << std::endl;

    std::cout << logging("INFO") << "Pixie16SaveDSPParametersToFile start" << std::endl;
    std::cout << logging("INFO") << "Writing to " << args.parfile << std::endl;
    r = verify_api_return_value(::Pixie16SaveDSPParametersToFile(args.parfile.c_str()),
                                "Pixie16SaveDSPParametersToFile");
    if (!r) {
        return EXIT_FAILURE;
    }

    ::Pixie16ExitSystem(static_cast<unsigned short>(args.slots.size()));
    return EXIT_SUCCESS;
}