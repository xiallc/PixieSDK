/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file base_test_runner.cpp
 * @brief Initializes a test runner to generate test suites.
 */

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>

#include <thread>

int
get_num_cpus(void) {
    return std::thread::hardware_concurrency();
}

std::string
get_version(void) {
#if __GNUC__
    return __VERSION__;
#elif defined(_MSC_VER)
    return std::to_string(_MSC_FULL_VER);
#else
    return "N/A";
#endif
}

int
main(int argc, char** argv) {
    int num_cpus = get_num_cpus();
    auto version = get_version();
    std::cout << std::endl
              << "Pixie Tests" << std::endl
              << "   Compiler: " << version << std::endl
              << "   CPU(s): " << num_cpus << std::endl
              << std::endl;
    int rc = doctest::Context(argc, argv).run();
    std::cout << std::endl
              << "result: " << rc << std::endl;
    return rc;
}
