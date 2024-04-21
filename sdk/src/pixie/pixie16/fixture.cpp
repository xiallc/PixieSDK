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

/** @file fixture.cpp
 * @brief Implements per channel hardware specific support for the Pixie-16 modules.
 */

#include <algorithm>
#include <cstring>

#include <pixie/error.hpp>
#include <pixie/log.hpp>
#include <pixie/utils/string.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/fixture.hpp>
#include <pixie/pixie16/module.hpp>

#include <pixie/pixie16/db/module.hpp>

namespace xia {
namespace pixie {
namespace fixture {

static void unsupported_op(const std::string what) {
    throw error::error(error::code::internal_failure, "invalid fixture op: " + what);
}

assembly::assembly(pixie::module::module& module__, std::string label_)
    : label(label_), module_(module__) {
    /* Do nothing */
}

assembly::~assembly() {
    /* Do nothing */
}

std::string assembly::get_mib_base() {
    std::ostringstream oss;
    std::string ll = label;
    util::string::tolower(ll);
    oss << "module" << mib::mibsep << module_.slot << mib::mibsep << ll << mib::mibsep;
    return oss.str();
}

void assembly::open() {
    for (auto assembly_ : subassemblies) {
        assembly_->open();
    }
}

void assembly::close() {
    for (auto assembly_ : subassemblies) {
        assembly_->close();
    }
}

void assembly::initialize() {
    for (auto assembly_ : subassemblies) {
        assembly_->initialize();
    }
}

void assembly::online() {
    for (auto assembly_ : subassemblies) {
        assembly_->online();
    }
}

void assembly::forced_online() {
    for (auto assembly_ : subassemblies) {
        assembly_->forced_online();
    }
}

void assembly::forced_offline() {
    for (auto assembly_ : subassemblies) {
        assembly_->forced_offline();
    }
}

void assembly::fpga_comms_loaded() {
    for (auto assembly_ : subassemblies) {
        assembly_->fpga_comms_loaded();
    }
}

void assembly::fpga_fippi_loaded() {
    for (auto assembly_ : subassemblies) {
        assembly_->fpga_comms_loaded();
    }
}

void assembly::dsp_loaded() {
    for (auto assembly_ : subassemblies) {
        assembly_->dsp_loaded();
    }
}

void assembly::boot() {
    for (auto assembly_ :  subassemblies) {
        assembly_->boot();
    }
}

void assembly::erase_values() {
    for (auto assembly_ :  subassemblies) {
        assembly_->erase_values();
    }
}

void assembly::init_values() {
    for (auto assembly_ : subassemblies) {
        assembly_->init_values();
    }
}

void assembly::erase_channels() {
    for (auto assembly_ : subassemblies) {
        assembly_->erase_channels();
    }
}

void assembly::init_assemblies() {
    /*
     * Assume the derived class has overloaded this and created the
     * subassemblies and this is used to then get each subassembly to
     * initialise
     */
    for (auto assembly_ : subassemblies) {
        assembly_->init_assemblies();
    }
}

void assembly::init_channels() {
    /*
     * If there are no subassemblies the channel hardware is on the
     * base assembly. Support channel initialisation here saving the
     * need to create module subassembly with this initialisation.
     */
    if (subassemblies.empty()) {
        log(log::info) << module::module_label(module_, "fixture: assembly")
                        << label << ": init-channels: create channel fixtures";
        for (int chan = 0; chan < static_cast<int>(module_.num_channels); ++chan) {
            auto& mod_chan = module_.channels[chan];
            auto& config = module_.eeprom.configs[chan];
            mod_chan.fixture = std::make_shared<channel>(mod_chan, *this, config);
        }
    } else {
        for (auto assembly_ : subassemblies) {
            assembly_->init_channels();
        }
    }
}

void assembly::sync_hw() {
    for (auto assembly_ : subassemblies) {
        assembly_->sync_hw();
    }
}

void assembly::sync_vars(const param::sync_mode sync_mode) {
    for (auto assembly_ : subassemblies) {
        assembly_->sync_vars(sync_mode);
    }
}

void assembly::set_dacs() {
    unsupported_op("set DACs is using the DSP");
}

void assembly::get_traces() {
    unsupported_op("get traces is using the DSP");
}

void assembly::adjust_offsets() {
    unsupported_op("adjust offsets is using the DSP");
}

void assembly::tau_finder() {
    unsupported_op("tau finder is using the DSP");
}

void assembly::event(const std::string name) {
    unsupported_op("no event support: " + name);
}

void assembly::set(const std::string item, bool ) {
    unsupported_op("no set support: bool: " + item);
}

void assembly::set(const std::string item, unsigned int ) {
    unsupported_op("no set support: int: " + item);
}

void assembly::set(const std::string item, double ) {
    unsupported_op("no set support: double: " + item);
}

void assembly::get(const std::string , bool& value) {
    value = false;
}

void assembly::get(const std::string item, unsigned int& ) {
    unsupported_op("no get support: unsigned int: " + item);
}

void assembly::get(const std::string item, double& ) {
    unsupported_op("no get support: double: " + item);
}

bool assembly::has_test(const std::string ) {
    return false;
}

void assembly::run_test(const std::string , bool& , log_output& ) {
    unsupported_op("no test for assembly");
}

channel::channel(
    pixie::channel::channel& module_channel_, assembly& assembly__, const hw::config& config_)
    : module_channel(module_channel_), assembly_(assembly__),
      config(config_), label("motherboard") {
}

channel::~channel() {
    close();
}

void channel::open() {
    /* Do nothing */
}

void channel::close() {
    /* Do nothing */
}

void channel::online() {
}

void channel::forced_online() {
}

void channel::forced_offline() {
}

void channel::set_dac(param::value_type ) {
    unsupported_op("set DAC is using the DSP");
}

void channel::adjust_offsetdac() {
    unsupported_op("adjust offsetdac is using the DSP");
}

void channel::acquire_adc() {
    unsupported_op("ADC acquire is using the DSP");
}

void channel::read_adc(hw::adc_word* , size_t ) {
    unsupported_op("read ADC is using the DSP");
}

void channel::event(const std::string name) {
    unsupported_op("no event support: " + name);
}

void channel::set(const std::string item, bool ) {
    unsupported_op("no set support: bool: " + item);
}

void channel::set(const std::string item, unsigned int ) {
    unsupported_op("no set support: int: " + item);
}

void channel::set(const std::string item, double ) {
    unsupported_op("no set support: double: " + item);
}

void channel::get(const std::string , bool& value) {
    value = false;
}

void channel::get(const std::string item, unsigned int& ) {
    unsupported_op("no get support: unsigned int: " + item);
}

void channel::get(const std::string item, double& ) {
    unsupported_op("no get support: double: " + item);
}

pixie::module::module& channel::get_module() {
    return module_channel.module;
}

const std::string channel::persistent_key(const std::string& item) const {
    return
        "fixture.channel." +
        std::to_string(module_channel.number) + '.' + label + '.' + item;
}

assembly_ptr make(pixie::module::module& module_) {
    assembly_ptr assembly_;
    switch (module_.get_rev_tag()) {
    case hw::rev_tag::rev_H:
        assembly_ = std::make_shared<db::module>(module_);
        break;
    default:
        assembly_ = std::make_shared<assembly>(module_);
        break;
    }
    return assembly_;
}

};  // namespace fixture
};  // namespace pixie
};  // namespace xia
