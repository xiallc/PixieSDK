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

/** @file config_list_mode_fifo.cpp
 * @brief Configures the list-mode data FIFO
 */

#include <iostream>
#include <string>
#include <vector>

#include <pixie16/pixie16.h>

#include <example_utils.hpp>

void print_fifo_config(const module_fifo_config& fifo_config) {
    std::cout << logging("INFO") << "Bandwidth (MB/sec): " << fifo_config.bandwidth_mb_per_sec
              << std::endl;
    std::cout << logging("INFO") << "Buffers : " << fifo_config.buffers << std::endl;
    std::cout << logging("INFO") << "DMA Trigger Level (B): " << fifo_config.dma_trigger_level_bytes
              << std::endl;
    std::cout << logging("INFO") << "Hold (usec): " << fifo_config.hold_usecs << std::endl;
    std::cout << logging("INFO") << "Idle wait (usec): " << fifo_config.idle_wait_usecs
              << std::endl;
    std::cout << logging("INFO") << "Run wait (usec): " << fifo_config.run_wait_usecs << std::endl;
}

int main(int argc, char** argv) {
    CliArguments args = ParseCliArguments(argc, argv);

    auto r = InitializeAndBootSystem(args);
    if (!r) {
        return EXIT_FAILURE;
    }

    // Allocate a struct to hold the FIFO configuration.
    module_fifo_config fifo_config{};

    // Get the current fifo configuration.
    r = verify_api_return_value(::PixieGetFifoConfiguration(0, &fifo_config),
                                "PixieGetFifoConfiguration");
    if (!r) {
        return EXIT_FAILURE;
    }

    std::cout << logging("INFO") << "Current FIFO Config:" << std::endl;
    print_fifo_config(fifo_config);

    // Increases the number of available buffers in the pool from 100 to 1000
    fifo_config.buffers = 1000;
    // Minimizes the amount of data in the buffer that will trigger a read.
    fifo_config.dma_trigger_level_bytes = 512;
    // Minimizes the time between successive reads
    fifo_config.hold_usecs = 1000;

    r = verify_api_return_value(::PixieSetFifoConfiguration(0, &fifo_config),
                                "PixieSetFifoConfiguration");
    if (!r) {
        return EXIT_FAILURE;
    }

    // Verify the FIFO configuration got written
    r = verify_api_return_value(::PixieGetFifoConfiguration(0, &fifo_config),
                                "PixieGetFifoConfiguration");
    if (!r) {
        return EXIT_FAILURE;
    }

    std::cout << logging("INFO") << "Updated FIFO Config:" << std::endl;
    print_fifo_config(fifo_config);

    ::Pixie16ExitSystem(static_cast<unsigned short>(args.slots.size()));
    return EXIT_SUCCESS;
}