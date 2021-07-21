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

/** @file fpga.hpp
 * @brief Defines common data structures for the FPGAs on the Pixie-16 modules.
 */

#ifndef PIXIE_HW_FPGA_H
#define PIXIE_HW_FPGA_H

#include <cstdint>

#include <pixie/fw.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace module {
class module;
}
namespace hw {
namespace fpga {
struct control {
    /*
     * Controls.
     */
    struct controls {
        const uint32_t clear;
        const uint32_t set;
        const uint32_t done;

        controls(const uint32_t clear, const uint32_t set, const uint32_t done);
    };

    /*
     * Registers.
     */
    struct regs {
        const int DATACS;
        const int CTRLCS;
        const int RDCS;

        regs(const int DATACS, const int CTRLCS, const int RDCS);
    };

    module::module& module;
    const std::string name;
    const controls load_ctrl;
    const controls clear_ctrl;
    const regs reg;

    /*
     * Trace the load operation
     */
    const bool trace;

    control(module::module& module, const std::string name, const controls& load_ctrl,
            const controls& clear_ctrl, const regs& reg, bool trace = false);

    /*
     * Load
     */
    void load(const firmware::image& image, int retries = 10);

    /*
     * Has the programming completed?
     */
    bool done();

    /*
     * Low level access.
     */
    void bus_write(int reg, uint32_t data);
    uint32_t bus_read(int reg);

private:
    std::string make_what(const char* msg);
};
}  // namespace fpga
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_FPGA_H
