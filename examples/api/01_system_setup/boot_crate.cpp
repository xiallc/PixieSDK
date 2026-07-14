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

/** @file boot_crate.cpp
 * @brief Boots the crate with the specified slot mapping, firmware, and settings file.
 */

#include <iostream>
#include <string>
#include <vector>

#include <pixie16/pixie16.h>

#include <example_utils.hpp>

int main(int argc, char** argv) {
    CliArguments args = ParseCliArguments(argc, argv);

    if (args.slots.empty()) {
        std::cerr << logging("ERROR") << "At least one slot must be provided." << std::endl;
        return EXIT_FAILURE;
    }

    if (args.parfile.empty()) {
        std::cerr << logging("ERROR") << "Must provide settings file to load" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned short num_modules = static_cast<unsigned short>(args.slots.size());

    std::cout << logging("INFO") << "Pixie16InitSystem start" << std::endl;
    auto r = verify_api_return_value(::Pixie16InitSystem(num_modules, args.slots.data(), 0),
                                     "Pixie16InitSystem");
    if (!r) {
        return EXIT_FAILURE;
    }

    std::cout << logging("INFO") << "Pixie16LoadModuleFirmware start" << std::endl;
    if (!args.firmware_path.empty()) {
        r = verify_api_return_value(::Pixie16LoadModuleFirmware(args.firmware_path.c_str()),
                                    "Pixie16LoadModuleFirmware");
        if (!r) {
            return EXIT_FAILURE;
        }
    }

    std::cout << logging("INFO") << "PixieBootCrate start" << std::endl;
    r = verify_api_return_value(::PixieBootCrate(args.parfile.c_str(), PIXIE_BOOT_RESET_LOAD),
                                "PixieBootCrate");
    if (!r) {
        return EXIT_FAILURE;
    }

    ::Pixie16ExitSystem(num_modules);
    return EXIT_SUCCESS;
}