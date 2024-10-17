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

/** @file sim.hpp
 * @brief Defines a Pixie-16 simulation framework to facilitate testing
 */

#ifndef PIXIE_SDK_SYSTEM_SIMULATION_HPP
#define PIXIE_SDK_SYSTEM_SIMULATION_HPP

#include <iostream>

#include <pixie/error.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Collects tools for simulating the Pixie-16 hardware.
 */
namespace sim {
using error = xia::pixie::error::error;

/**
 * @brief A Simulated a module derived from the module class.
 */
class module : public xia::pixie::module::module {
public:
    static const size_t pci_addr_space_size = 8 * 1024 * 1024;

    using error = xia::pixie::module::error;

    module(backplane::backplane& backplane);
    ~module() override;

    void open(size_t device_number) override;
    void close() override;
    void probe(const firmware::firmware_set& firmware) override;
    void boot(const boot_params& params, const firmware::firmware_set& firmware) override;
    void initialize() override;
    void firmware_release(
        firmware::release_type& release, firmware::firmware_set::set_type& type) override;
    void init_values() override;
    void dma_read(const hw::address source, hw::word_ptr values, const size_t size) override;

    void load_var_defaults(const std::string& file);
    void load_var_defaults(std::istream& input);

    std::unique_ptr<uint8_t[]> pci_memory;
    std::string var_defaults;

    firmware::release_type fw_release;
    firmware::firmware_set::set_type fw_type;

    bool init_online;
    void sim_reg(int reg, hw::word val);
    void sim_csr(hw::word val);
};

/**
 * @brief Simulated a crate. Creates the simulated module.
 */
class crate : public xia::pixie::crate::crate {
public:
    using error = xia::pixie::module::error;

    crate(bool init_online = true);
    ~crate() override;

    void create_module_slots() override;

    bool init_online;
};

/**
 * @brief Firmware sets for simulation
 */
using firmware_set_def = std::vector<std::string>;
using firmware_set_defs = std::vector<firmware_set_def>;

void load_firmware_sets(firmware::system& firmwares);
void load_firmware_sets(firmware::system& firmwares, const firmware_set_defs& sets);

/**
 * @brief Module definition used to set up the simulation.
 */
struct module_def {
    size_t device_number;
    int slot;
    int revision;
    int eeprom_format;
    int serial_num;
    int num_channels;
    int adc_bits;
    int adc_msps;
    int adc_clk_div;
    int db_pid;
    int db_count;
    std::string var_defaults;

    module_def();
};

using module_defs = std::vector<module_def>;

extern module_defs mod_defs;

void load_module_defs(const std::string mod_def_file);
void load_module_defs(std::istream& input);
void add_module_def(const std::string mod_desc, const char delimiter = ' ');
void clear_module_defs();

/**
 * @brief EEPROM data for a given module.
 */
using eeprom_data = std::vector<std::string>;
using eeprom_slot_def = std::pair<std::vector<int>, std::vector<eeprom_data>>;

struct eeprom_def {
    eeprom_data data;
    int slot;
};

using eeprom_defs = std::vector<eeprom_def>;
using eeprom_addr = unsigned long;

extern eeprom_defs eep_defs;

void load_eeprom_defs(const eeprom_slot_def& defs);
void add_eeprom_def(const int slot, const eeprom_data& data);
void clear_eeprom_defs();

}  // namespace sim
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_SDK_SYSTEM_SIMULATION_HPP
