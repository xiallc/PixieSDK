/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright Tuesday, June 30, 2026 XIA LLC, All rights reserved.
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

/** @file example_utils.hpp
 * @brief
 */

#ifndef PIXIE_SDK_EXAMPLE_UTILS_HPP
#define PIXIE_SDK_EXAMPLE_UTILS_HPP

#include <string>
#include <vector>

#include <pixie16/pixie16.h>

#define MAX_SLOTS 15
#define MSG_SIZE 1024
#define TIME_SIZE 80

struct CliArguments {
    std::string firmware_path;
    std::string parfile;
    std::vector<unsigned short> slots;
};

std::string walltime_iso_string();

struct logging {
    logging(const std::string& type_) {
        type = type_;
        datetime = walltime_iso_string();
    }

    std::string type;
    std::string datetime;
};

std::ostream& operator<<(std::ostream& os, const logging& log);

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const bool& print_success = true);

CliArguments ParseCliArguments(int argc, char** argv);
bool InitializeAndBootSystem(CliArguments& args);

#endif  //PIXIE_SDK_EXAMPLE_UTILS_HPP
