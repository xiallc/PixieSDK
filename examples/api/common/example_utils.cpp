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

/** @file example_utils.cpp
 * @brief
 */

#include <chrono>
#include <iomanip>
#include <iostream>

#include <args/args.hxx>

#include <example_utils.hpp>
#include <pixie16/pixie16.h>

CliArguments ParseCliArguments(int argc, char** argv) {
    CliArguments parsed_args;
    args::ArgumentParser parser("Pixie16 API Example Runner");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::ValueFlag<std::string> fw_path(parser, "firmware", "Alternate firmware path",
                                         args::Matcher{'f', "firmware"});
    args::ValueFlag<std::string> parfile(parser, "parfile", "Parameter settings file",
                                         args::Matcher{'p', "parfile"});
    args::ValueFlag<std::string> slots(parser, "slots",
                                       "Comma-separated list of slots (e.g., -s 2,3,4,5)",
                                       args::Matcher{'s', "slots"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        exit(EXIT_SUCCESS);
    } catch (const args::Error& e) {
        std::cerr << "Argument Error: " << e.what() << std::endl << parser;
        exit(EXIT_FAILURE);
    }

    if (slots) {
        std::stringstream ss(slots.Get());
        std::string item;
        try {
            while (std::getline(ss, item, ',')) {
                parsed_args.slots.push_back(static_cast<unsigned short>(std::stoi(item)));
            }
        } catch (const std::exception&) {
            std::cerr << "Error parsing slots. Ensure they are comma-separated integers."
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    if (parfile) {
        parsed_args.parfile = parfile.Get();
    }

    if (fw_path) {
        parsed_args.firmware_path = fw_path.Get();
    }

    return parsed_args;
}

std::string walltime_iso_string() {
    auto now = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::to_time_t(now);
    auto now2 = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    char timeBuffer[TIME_SIZE];
    std::strftime(timeBuffer, TIME_SIZE, "%FT%T", gmtime(&currentTime));

    std::stringstream tmp;
    tmp << timeBuffer << "." << std::setfill('0') << std::setw(3) << now2.count() % 1000 << "Z";
    return tmp.str();
}


std::ostream& operator<<(std::ostream& os, const logging& log) {
    os << "[" << log.type << "]" << " " << log.datetime << " ";
    return os;
}

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const bool& print_success) {
    if (val < 0) {
        std::string msg;
        msg.resize(MSG_SIZE);
        ::PixieGetReturnCodeText(val, &msg[0], MSG_SIZE);
        std::cout << logging("ERROR") << func_name << " failed with code " << val
                  << " and message: " << msg << std::endl;
        return false;
    }
    if (print_success) {
        std::cout << logging("INFO") << func_name << " finish" << std::endl;
    }
    return true;
}

bool InitializeAndBootSystem(CliArguments& args) {
    if (args.slots.empty()) {
        std::cerr << logging("ERROR") << "At least one slot must be provided." << std::endl;
        return false;
    }

    if (args.parfile.empty()) {
        std::cerr << logging("ERROR") << "Must provide settings file to load" << std::endl;
        return false;
    }

    std::cout << logging("INFO") << "Pixie16InitSystem start" << std::endl;
    auto r = verify_api_return_value(
        ::Pixie16InitSystem(static_cast<unsigned short>(args.slots.size()), args.slots.data(), 0),
        "Pixie16InitSystem");
    if (!r) {
        return false;
    }

    std::cout << logging("INFO") << "Pixie16LoadModuleFirmware start" << std::endl;
    if (!args.firmware_path.empty()) {
        r = verify_api_return_value(::Pixie16LoadModuleFirmware(args.firmware_path.c_str()),
                                    "Pixie16LoadModuleFirmware");
        if (!r) {
            return false;
        }
    }

    std::cout << logging("INFO") << "PixieBootCrate start" << std::endl;
    r = verify_api_return_value(::PixieBootCrate(args.parfile.c_str(), PIXIE_BOOT_RESET_LOAD),
                                "PixieBootCrate");
    if (!r) {
        return false;
    }

    return true;
}