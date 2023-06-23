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
#include <pixie/util.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/db.hpp>
#include <pixie/pixie16/fixture.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace fixture {

static void unsupported_op(const std::string what) {
    throw error::error(error::code::internal_failure, "invalid fixture op: " + what);
}

channel::channel(pixie::channel::channel& module_channel_, const hw::config& config_)
    : module_channel(module_channel_), label("motherboard"),
      config(config_) {
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

void channel::report(std::ostream& out, const std::string& prefix) const {
    out << prefix << "Fixture        : " << label << std::endl;
    config.report(out, prefix);
}

void channel::set(const std::string item, bool ) {
    unsupported_op("not set support: bool: " + item);
}

void channel::set(const std::string item, int ) {
    unsupported_op("no set support: int: " + item);
}

void channel::set(const std::string item, double ) {
    unsupported_op("no set support: double: " + item);
}

void channel::set(const std::string item, hw::word ) {
    unsupported_op("no set support: hw::word: " + item);
}

void channel::get(const std::string item, bool& ) {
    unsupported_op("no get support: bool: " + item);
}

void channel::get(const std::string item, int& ) {
    unsupported_op("no get support: int: " + item);
}

void channel::get(const std::string item, double& ) {
    unsupported_op("no get support: double: " + item);
}

void channel::get(const std::string item, hw::word& ) {
    unsupported_op("no get support: hw::word: " + item);
}

pixie::module::module& channel::get_module() {
    return module_channel.module;
}

const std::string channel::persistent_key(const std::string& item) const {
    return
        "fixture.channel." +
        std::to_string(module_channel.number) + '.' + label + '.' + item;
}

module::module(pixie::module::module& module__)
    : module_(module__), label("none") {
    /* Do nothing */
}

module::~module() {
    /* Do nothing */
}

void module::open() {
    /* Do nothing */
}

void module::close() {
    /* Do nothing */
}

void module::initialize() {
    /* Do nothing */
}

void module::online() {
    /* Do nothing */
}

void module::forced_online() {
    /* Do nothing */
}

void module::forced_offline() {
    /* Do nothing */
}

void module::fgpa_comms_loaded() {
    /* Do nothing */
}

void module::fgpa_fippi_loaded() {
    /* Do nothing */
}

void module::dsp_loaded() {
    /* Do nothing */
}

void module::boot() {
    /* Do nothing */
}

void module::erase_values() {
    /* Do nothing */
}

void module::init_values() {
    /* Do nothing */
}

void module::erase_channels() {
    /* Do nothing */
}

void module::init_channels() {
    log(log::debug) << pixie::module::module_label(module_, "fixture: module")
                    << "init-channels: create channel fixtures";
    for (size_t chan = 0; chan < module_.num_channels; ++chan) {
        module_.channels[chan].fixture =
            fixture::make(module_.channels[chan], module_.eeprom.configs[chan]);
    }
}

void module::sync_hw() {
    /* Do nothing */
}

void module::sync_vars() {
    /* Do nothing */
}

void module::set_dacs() {
    unsupported_op("set DACs is using the DSP");
}

void module::get_traces() {
    unsupported_op("get traces is using the DSP");
}

void module::adjust_offsets() {
    unsupported_op("adjust offsets is using the DSP");
}

void module::tau_finder() {
    unsupported_op("tau finder is using the DSP");
}

channel_ptr make(pixie::channel::channel& module_channel, const hw::config& config) {
    channel_ptr chan_fixture;
    switch (config.fixture) {
    case hw::module_fixture::DB04:
        chan_fixture = std::make_shared<db04>(module_channel, config);
        break;
    default:
        chan_fixture = std::make_shared<channel>(module_channel, config);
        break;
    }
    chan_fixture->open();
    return chan_fixture;
}

module_ptr make(pixie::module::module& module_) {
    module_ptr mod_fixtures;
    switch (module_.get_rev_tag()) {
    case hw::rev_tag::rev_H:
        mod_fixtures = std::make_shared<afe_dbs>(module_);
        break;
    default:
        mod_fixtures = std::make_shared<module>(module_);
        break;
    }
    return mod_fixtures;
}

};  // namespace fixture
};  // namespace pixie
};  // namespace xia
