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

/** @file adjust_offsets.cpp
 * @brief Initializes the system, adjusts the VOFFSETs, writes the values to the settings file.
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

    auto num_modules = static_cast<unsigned short>(args.slots.size());

    /*
     * Runs against all modules in the crate.
     * For a single module, provide the mod number (Ex. 0).
     */
    std::cout << logging("INFO") << "Pixie16AdjustOffsets start" << std::endl;
    r = verify_api_return_value(::Pixie16AdjustOffsets(num_modules), "Pixie16AdjustOffsets");
    if (!r) {
        return EXIT_FAILURE;
    }

    std::cout << logging("INFO") << "Pixie16SaveDSPParametersToFile start" << std::endl;
    std::cout << logging("INFO") << "Writing to " << args.parfile << std::endl;
    r = verify_api_return_value(::Pixie16SaveDSPParametersToFile(args.parfile.c_str()),
                                "Pixie16SaveDSPParametersToFile");
    if (!r) {
        return EXIT_FAILURE;
    }

    ::Pixie16ExitSystem(num_modules);
    return EXIT_SUCCESS;
}