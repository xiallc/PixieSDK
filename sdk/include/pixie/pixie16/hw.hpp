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

/** @file hw.hpp
 * @brief Defines hardware specific data for the Pixie-16 modules.
 */

#ifndef PIXIE_HW_H
#define PIXIE_HW_H

#include <array>
#include <limits>
#include <stdexcept>
#include <vector>

#include <cstdint>

#include <pixie/error.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Collects Pixie-16 specific hardware information.
 */
namespace hw {
/**
 * @brief Revision tags for the Pixie-16 hardware
 */
enum rev_tag {
    rev_A = 10,
    rev_B,
    rev_C,
    rev_D,
    rev_E,
    rev_F,
    rev_G,
    rev_H,
    rev_I,
    rev_J,
    rev_K,
    rev_L
};

/**
 * Slot number type
 */
using slot_type = size_t;
using slot_numbers = std::vector<hw::slot_type>;

/**
 * Invalid slot number
 */
static constexpr slot_type slot_invalid = std::numeric_limits<slot_type>::max();

/**
 * Maximum number of physical slots in a crate.
 *
 * @note Some slots may not be available for modules. The modules shall
 *       return not present.
 */
static const int max_slots = 15;

/**
 * Maximum number of channels a module can have.
 */
static const int max_channels = 32;

/**
 * Maximum ADC trace size.
 */
static const size_t max_adc_trace_length = 8192;

/**
 * Histogram sizes.
 *
 * The depends on the revision of module and so the number of supported
 * channels.
 */
static const size_t large_histogram_length = 32768;
static const size_t small_histogram_length = 16384;

/**
 * @brief The maximum number of baselines collected by the get_baselines control task on Rev F.
 */
static const size_t max_large_num_baselines = 3640;
/**
 * @brief The maximum number of baselines collected by the get_baselines control task on Rev H.
 */
static const size_t max_small_num_baselines = 1927;

/**
 * IO buffer length
 */
static const size_t io_buffer_length = 65536;

/**
 * System FPGA clock frequency in MHz
 */
static const size_t system_clock_mhz = 100;

/**
 * FIFO size in words
 */
static const size_t fifo_size_words = 131072;

/**
 * DMA block size.
 */
static const size_t max_dma_block_size = 8192;

/**
 * EEPROM block size
 */
static const size_t eeprom_block_size = 128;

/**
 * Baselines block length
 *
 * Length of each baseline (default: 2 timestamp words + 16 baselines)
 */
static const size_t baselines_block_len = 18;

/**
 * PCI Bus maximum data transfer rate in MBytes.
 */
static const size_t pci_bus_datarate = 100;

/**
 * @brief Defines a type for memory register addresses.
 */
using address = uint32_t;

/**
 * @brief Defines a type for the basic unit of read/write communication
 */
using word = uint32_t;

/**
 * @brief defines a type for a pointer to a word
 */
using word_ptr = word*;

/**
 * @brief defines a type for a const pointer to a word
 */
using const_word_ptr = const word*;

/**
 * @brief Defines a type for a vector of words. Used to handle blocks of memory.
 */
using words = std::vector<word>;

/**
 * @brief Defines a type for ADC trace words.
 */
using adc_word = uint16_t;

/**
 * @brief Defines a type used to handle IO data from the hardware.
 */
using io_buffer = std::array<word, io_buffer_length>;

/**
 * @brief Defines a type for an ADC trace buffer, as read from the channel
 */
using adc_trace_buffer = std::array<word, max_adc_trace_length>;

/**
 * @brief defines a type for ADC traces
 */
using adc_trace = std::vector<adc_word>;

/**
 * @brief defines a type for a container of ADC traces
 */
using adc_traces = std::vector<adc_trace>;

/**
 * @brief defines a type for a container of doubles
 */
using doubles = std::vector<double>;

/**
 * @brief Defines a type for Hardware errors
 */
using error = error::error;

/**
 * @brief Type of fixtures modules support.
 *
 * A module with no fixtures uses mainboard.
 */
enum struct module_fixture {
    mainboard,
    DB01,
    DB02,
    DB04,   /* DB04 PCB DB04 parts */
    DB05,   /* DB05 PCB DB05 parts */
    DB06,
    DB07,
    DB10
};

/**
 * @brief The object representing the physical hardware configuration
 *
 * This includes information about the number of channels, and their
 * analog front-end hardware.
 */
struct config {
    /**
     * Channel index in variable array.
     */
    int index;

    /*
     * Type of fixture.
     */
    module_fixture fixture;

    /*
     * Analog front end (AFE)
     */
    int adc_bits;
    int adc_msps;
    int adc_clk_div;
    int fpga_clk_mhz;

    /**
     * @brief Maximum size of the histogram data for this channel in words.
     */
    size_t max_histogram_length;

    /**
     * @brief Maximum size of the ADC trace for this channel in words.
     */
    size_t max_adc_trace_length;

    /**
     * @brief maximum number of baselines captured for this channel with the get_baselines
     * control task.
     */
    size_t max_num_baselines;

    config(size_t histogram_size, size_t adc_trace_length, size_t num_baselines,
           int adc_bits, int adc_msps, int adc_clk_div, int fpga_clk_mhz);
    config();

    bool operator==(const config& cfg);
    bool operator!=(const config& cfg);

    /**
     * Clear the configuration
     */
    void clear();
};

using configs = std::vector<config>;

/**
 * @brief A daughter board assembly
 */
struct db_assembly {
    /*
     * EEPROM data
     */
    int prom_id;     /**< DB PROM ID defined in Steps to Release Spreadsheet */
    int position;    /**< Position on the main board */

    std::string label;
    size_t index;

    db_assembly(const int prom_id, const int position);
};

using db_assemblies = std::vector<db_assembly>;

/**
 * Return the module fixture given a label. The motherboard fixture
 * is not a valid result.
 */
module_fixture get_module_fixture(const std::string label);

/**
 * Return the module fixture label given a fixture.
 */
std::string get_module_fixture_label(const module_fixture fixture);

/**
 * Convertor. Use with caution as this steps around the type system.
 */
template<typename I, typename O>
inline void convert(I vin, O& vout) {
    vout = static_cast<O>(vin);
}

/**
 * @brief Wait in microseconds.
 * @todo We need to check how well this works.
 * @param microseconds The number of microseconds we should wait.
 */
void wait(size_t microseconds);

/**
 * Bus interface calls.
 */
inline word read_word(void* addr, const int offset) {
    volatile word* p = static_cast<volatile word*>(addr);
    return *(p + (offset / 4));
}

inline void write_word(void* addr, const int offset, const word value) {
    volatile word* p = static_cast<volatile word*>(addr);
    *(p + (offset / 4)) = value;
}
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_H
