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

/** @file sysctl_queries.cpp
 * @brief Demonstrates how to use the sysctl functions to get information about the hardware.
 */

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <example_utils.hpp>
#include <pixie16/pixie16.h>

#include <nolhmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char** argv) {
    CliArguments args = ParseCliArguments(argc, argv);

    /*
     * Check the current state of the MIB log
     */
    std::string logcmd_val(256, '\0');
    auto r = verify_api_return_value(
        PixieSysControlGet("log.command", &logcmd_val[0], logcmd_val.size()), "PixieSysControlGet");
    if (!r) {
        return EXIT_FAILURE;
    }
    logcmd_val.resize(std::strlen(logcmd_val.c_str()));
    std::cout << logging("INFO") << "MIB Logging status: " << logcmd_val << std::endl;

    /*
     * Configure the system so that the log entries get spooled to the MIB
     */
    r = verify_api_return_value(PixieSysControlSet("log.command", "on"), "PixieSysControlSet");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Readback so that we can check that we set the value properly.
     */
    r = verify_api_return_value(
        PixieSysControlGet("log.command", &logcmd_val[0], logcmd_val.size()), "PixieSysControlGet");
    if (!r) {
        return EXIT_FAILURE;
    }
    logcmd_val.resize(std::strlen(logcmd_val.c_str()));
    std::cout << logging("INFO") << "MIB Logging status: " << logcmd_val << std::endl;

    /*
     * Init and boot the hardware so that we get entries into the MIB.
     */
    r = InitializeAndBootSystem(args);
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Obtain the last 10 log entries. Note the escape characters in the regex!
     */
    r = verify_api_return_value(PixieSysControlOpen("log.\\b[0-9]\\b", PIXIE_SYSCTL_FORMAT_JSON),
                                "PixieSysControlOpen");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * We need to now get the size of the query result.
     */
    size_t size = 0;
    r = verify_api_return_value(PixieSysControlSize(&size), "PixieSysControlSize");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Read the query result into our log buffer
     */
    std::string logbuf(size, '\0');
    r = verify_api_return_value(PixieSysControlRead(&logbuf[0], &size), "PixieSysControlRead");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Close the query so that we can execute new ones in the future.
     */
    r = verify_api_return_value(PixieSysControlClose(), "PixieSysControlClose");
    if (!r) {
        return EXIT_FAILURE;
    }

    /*
     * Pretty print the log buffer to the terminal.
     */
    try {
        const json parsed_log = json::parse(logbuf);
        std::cout << logging("INFO") << "MIB Log Entries:\n" << parsed_log.dump(4) << std::endl;
    } catch (const json::parse_error& e) {
        std::cerr << logging("ERROR") << "Failed to parse MIB JSON output: " << e.what() << "\n"
                  << "Raw Output: " << logbuf << std::endl;
    }

    ::Pixie16ExitSystem(static_cast<unsigned short>(args.slots.size()));
    return EXIT_SUCCESS;
}