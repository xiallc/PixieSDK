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

/** @file csr.hpp
 * @brief Defines how we handle the Pixie-16 CSR.
 */

#ifndef PIXIE_HW_CSR_H
#define PIXIE_HW_CSR_H

#include <cstdint>

#include <pixie/pixie16/hw.hpp>

namespace xia
{
namespace pixie
{
namespace module
{
    class module;
}
namespace hw
{
namespace csr
{
/*
 * Reset the CSR to a default state.
 */
void reset(module::module& module);

/*
 * Low level read/write and bit set/clear support.
 */
word read(module::module& module);
void write(module::module& module, word value);
void set(module::module& module, word mask);
void clear(module::module& module, word mask);

/*
 * Set the bit mask into the CSR when constructed and clear
 * in the destructor.
 */
struct set_clear {
    module::module& module;
    const uint32_t mask;
    set_clear(module::module& module, uint32_t mask);
    ~set_clear();
};

/*
 * Wait for external FIFO
 */
void fifo_ready_wait(module::module& module, const size_t polls = 1000);
}
}
}
}

#endif  // PIXIE_HW_CSR_H
