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

/** @file memory.cpp
 * @brief Implements data and functions used to access Pixie-16 memory registers.
 */

#include <iomanip>
#include <iostream>

#include <pixie/log.hpp>

#include <pixie/pixie16/csr.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

/*
 * ADSP-21160 registers defined from
 * https://www.analog.com/media/en/dsp-documentation/processor-manuals/ADSP-21160_hwr_rev4.1.pdf
 * last accessed on 2021-06-24
 */
/*
 * DMA channel status register
 */
#define DMASTAT 0x37

namespace xia {
namespace pixie {
namespace hw {
namespace memory {
bus::bus(module::module& module_, const hw::hbr::host_bus_access access_)
    : module(module_), access(access_) {
}
host_bus::host_bus(module::module& module_, const hw::hbr::host_bus_access access_)
    : bus(module_, access_) {}

word host_bus::read(const address addr) {
    module::module::bus_guard guard(module);
    hbr::host_bus_request hbr(module, access);
    bus_write(hw::device::EXT_MEM_TEST, addr);
    word value = bus_read(hw::device::WRT_DSP_MMA);
    return value;
}

word host_bus::read(const size_t offset, const address addr) {
    return read(static_cast<hw::address>(addr + offset));
}

word host_bus::read(const size_t channel, const size_t offset, const address addr) {
    channel::channel& chan = module[channel];
    if ( chan.fixture->config.index < 0) {
        throw error(error::code::channel_invalid_index,
                    "dsp: invalid index: module=" + std::to_string(module.number) +
                        " channel=" + std::to_string(channel));
    }
    return read(static_cast<hw::address>(addr + chan.fixture->config.index + offset));
}

void host_bus::read(const address addr, word_ptr buffer, const size_t length) {
    module::module::bus_guard guard(module);

    size_t size = length;
    size_t offset = 0;
    /*
     * The 48 is taken from the legacy code. The significance of this value
     * is unknown.
     */
    while (size > 48) {
        const size_t block_size = size > hw::max_dma_block_size ? hw::max_dma_block_size : size;
        dma_read(hw::address(addr + offset), buffer + offset, block_size);
        size -= block_size;
        offset += block_size;
    }
    if (size > 0) {
        hbr::host_bus_request hbr(module, access);
        bus_write(hw::device::EXT_MEM_TEST, hw::word(addr + offset));
        buffer += offset;
        while (size-- > 0) {
        *buffer = bus_read(hw::device::WRT_DSP_MMA);
            ++buffer;
        }
    }
}

void host_bus::write(const address addr, const word value) {
    module::module::bus_guard guard(module);
    hbr::host_bus_request hbr(module, access);
    bus_write(hw::device::EXT_MEM_TEST, addr);
    bus_write(hw::device::WRT_DSP_MMA, value);
}

void host_bus::write(const size_t offset, const address addr, const word value) {
    write(static_cast<hw::address>(addr + offset), value);
}

void host_bus::write(const size_t channel, const size_t offset, const address addr, const word value) {
    channel::channel& chan = module[channel];
    if (chan.fixture->config.index < 0) {
        throw error(error::code::channel_invalid_index,
                    "dsp: invalid index: module=" + std::to_string(module.number) +
                        " channel=" + std::to_string(channel));
    }
    write(static_cast<hw::address>(addr + chan.fixture->config.index + offset), value);
}

void host_bus::write(const address addr, const words& values) {
    module::module::bus_guard guard(module);
    hbr::host_bus_request hbr(module, access);
    bus_write(hw::device::EXT_MEM_TEST, addr);
    for (auto value : values) {
        bus_write(hw::device::WRT_DSP_MMA, value);
    }
}

void host_bus::dma_read(const address addr, word_ptr buffer, const size_t length) {
    xia_log(log::debug) << module::module_label(module) << "dsp dma read: addr=0x" << std::hex << addr
                        << " length=" << std::dec << length;

    /*
     * The bus is held on entry.
     */

    hbr::host_bus_request hbr(module, access);

    bus_write(hw::device::EXT_MEM_TEST, DMASTAT);
    if ((bus_read(hw::device::WRT_DSP_MMA) & (1 << 11)) != 0) {
        throw error(error::code::device_dma_busy, "dsp: DMA busy");
    }

    bus_write(hw::device::WRT_DSP_II11, addr);
    bus_write(hw::device::WRT_DSP_C11, hw::word(length));
    bus_write(hw::device::WRT_DSP_IM11, 1);
    bus_write(hw::device::WRT_DSP_EC11, hw::word(length));
    bus_write(hw::device::WRT_DSP_DMAC11, 0x905);
    bus_write(hw::device::RD_WRT_FIFO_WML, hw::word(length) / 2);

    hbr.release();

    try {
        bus_write(hw::device::SET_INT_FIFO, 0);
        csr::fifo_ready_wait(module);
        module.dma_read(DSP_MEM_DMA, buffer, length);
    } catch (...) {
        hbr.request();
        bus_write(hw::device::WRT_DSP_DMAC11, 0x904);
        throw;
    }

    hbr.request();
    bus_write(hw::device::WRT_DSP_DMAC11, 0x904);
}

void host_bus::write(const size_t channel, const address addr, const words& values) {
    write(static_cast<hw::address>(addr + (channel * sizeof(word))), values);
}

dsp::dsp(module::module& module_)
    : host_bus(module_, hw::hbr::dsp_access) {}

fippi::fippi(module::module& module_)
    : host_bus(module_, hw::hbr::fpga_access) {}

mca::mca(module::module& module_)
    : bus(module_) {}

void mca::read(const address addr, words& values) {
    read(addr, values.data(), values.size());
}

void mca::read(const address addr, word_ptr values, size_t size) {
    module::module::bus_guard guard(module);

    /*
     * Guard the PCI active bit, so it is cleared when we exit.
     */
    csr::set_clear csr(module, 1 << hw::bit::PCIACTIVE);

    /*
     * Set up the address to read from.
     */
    bus_write(hw::device::WRT_EXT_MEM, addr);

    /*
     * Set up short FIFO in System FPGA
     */
    bus_write(hw::device::SET_EXMEM_FIFO, 0);

    /*
     * Read the data using DMA.
     */
    module.dma_read(MCA_MEM_DATA, values, size);
}

void mca::write(const address addr, const words& values) {
    write(addr, values.data(), values.size());
}

void mca::write(const address addr, const_word_ptr values, const size_t size) {
    module::module::bus_guard guard(module);

    /*
     * Guard the PCI active  bit, so it is cleared when we exit.
     */
    csr::set_clear csr(module, 1 << hw::bit::PCIACTIVE);

    bus_write(hw::device::WRT_EXT_MEM, addr);
    for (size_t i = 0; i < size; i++) {
        bus_write(MCA_MEM_DATA, values[i]);
    }
}

fifo::fifo(module::module& module_) : bus(module_) {}

size_t fifo::level() {
    module::module::bus_guard guard(module);
    return size_t(bus_read(hw::device::RD_WRT_FIFO_WML));
}

void fifo::read(word_ptr buffer, const size_t length) {
    module::module::bus_guard guard(module);

    bus_write(hw::device::SET_EXT_FIFO, hw::word(length));

    size_t polls = 1000;
    while (polls-- > 0) {
        if (bus_read(hw::device::RD_WRT_FIFO_WML) >= length) {
            break;
        }
    }
    if (polls == 0) {
        throw module::error(module.number, module.slot, module::error::code::device_fifo_failure,
                            "FIFO failed to reach watermark");
    }

    module.dma_read(FIFO_MEM_DMA, buffer, length);
}
};  // namespace memory
};  // namespace hw
};  // namespace pixie
};  // namespace xia
