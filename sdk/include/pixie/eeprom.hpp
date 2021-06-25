#ifndef PIXIE_EEPROM_H
#define PIXIE_EEPROM_H

/*----------------------------------------------------------------------
* Copyright (c) 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------*/
/// @file pixie_eeprom.cpp
/// @brief
/// @author Chris Johns
/// @date June 6, 2021

#include <cstdint>
#include <array>
#include <vector>

#include <pixie/error.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia
{
namespace pixie
{
namespace eeprom
{
/*
 * Local error
 */
typedef pixie::error::error error;

/*
 * Supported tags. Gaps allow additions in each section.
 */
enum struct tag {
    null = 0,    /* invalid */
    /*
     * Build state
     */
    model = 10,  /* model, part number */
    serial_num,  /* serial number */
    revision,    /* revision string */
    mod_strike,  /* hardware modifications */
    /*
     * Parameters
     */
    index = 30,  /* number, index, slot, id */
    size,        /* number of items, channels, modules */
    format_ver,  /* format version */
    /*
     * Analog Front End (AFE)
     */
    adc_msps = 100,
    adc_bits,
    adc_clk_div,
    fpga_clk,
    /*
     * Last maps to the erase EEPROM value
     */
    end = 0xff
};

enum element_type {
    null,
    string,
    num8,
    num16,
    num32,
    ieee_float,
    mac,
    ipv4
};

using mac_addr = std::array<uint8_t, 6>;
using ipv4_addr = std::array<uint8_t, 4>;

struct element_decs {
    tag key;
    std::string name;
    element_type type;
    size_t size;
    bool mandatory;
    bool depreciated;
};

struct header {
    static const size_t size = 4 + 1 + 1;

    uint32_t crc;
    uint8_t control;

    header();

    void clear();

    int version() const;
};

using contents = std::vector<uint8_t>;

struct eeprom {
    contents data;
    util::crc32 crc;

    header hdr;

    int format;

    std::string model;
    int serial_num;
    int revision;
    int mod_strike;

    int num_channels;
    int max_channels;

    hw::configs configs;

    eeprom();

    void clear();

    /*
     * Process the loaded data
     */
    void process();

    /*
     * Is there a valid EEPROM configuration.
     */
    bool valid() const;

    /*
     * Tag queries.
     */
    std::string get_string(const tag key, size_t count = 1);
    int get_number(const tag key, size_t count = 1);
    double get_float(const tag key, size_t count = 1);
    mac_addr get_mac(const tag key, size_t count = 1);
    ipv4_addr get_ipv4(const tag key, size_t count = 1);

    /*
     * Little endian reads of values from the data.
     */
    template <typename T> T get32(const size_t offset) const;
    template <typename T> T get16(const size_t offset) const;
    template <typename T> T get8(const size_t offset) const;

    /*
     * Find an instance of a tag in the data
     */
    size_t find(const tag key, size_t count = 0) const;

    /*
     * Return the tag's descriptor
     */
    const element_decs& lookup(const tag key) const;

protected:
    void throw_bad_tag_type(const tag key);
};

template <typename T> inline T
eeprom::get32(const size_t offset) const {
    return static_cast<T>((data[offset + 3] << 24) |
                          (data[offset + 2] << 16) |
                          (data[offset + 1] << 8) |
                          data[offset + 0]);
}

template <typename T> inline T
eeprom::get16(const size_t offset) const {
    return static_cast<T>((data[offset + 1] << 8) | data[offset + 0]);
}

template <typename T> inline T
eeprom::get8(const size_t offset) const {
    return static_cast<T>(data[offset + 0]);
}

}
}
}

#endif  // PIXIE_EEPROM_H
