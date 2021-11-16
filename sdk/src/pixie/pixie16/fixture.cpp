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

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/fixture.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace fixture {
/**
 * UserIn save to hold and restore UserIn variables.
 */
struct userin_save {
    hw::memory::dsp dsp;
    hw::address address;

    hw::word userin_0;
    hw::word userin_1;

    userin_save(module::module& module);

    void update(const hw::word& db_index, const hw::word& db_channel);

    ~userin_save();
};

/**
 * The daughter board fixture
 */
struct db : public channel {
    /**
     * The number is the position. The DBs are sorted by position so
     * asking for the index by channel returns the DB number.
     */
    int number;

    /**
     * Base channel for the daughter board
     */
    int base;

    /**
     * Channel offset relative to the fixture.
     */
    int offset;

    db(pixie::channel::channel& module_channel_, const hw::config& config_);

    virtual void acquire_adc() override;
};

/**
 * The DB04 fixture
 */
struct db04 : public db {
    db04(pixie::channel::channel& module_channel_, const hw::config& config_);

    virtual void set_dac() override;
};

userin_save::userin_save(module::module& module) : dsp(module) {
    address = module.module_var_descriptors[int(param::module_var::UserIn)].address;
    userin_0 = dsp.read(0, address);
    userin_1 = dsp.read(1, address);
}

void userin_save::update(const hw::word& db_index, const hw::word& db_channel) {
    dsp.write(0, address, db_index);
    dsp.write(1, address, db_channel);
}

userin_save::~userin_save() {
    dsp.write(0, address, userin_0);
    dsp.write(1, address, userin_1);
}

db::db(pixie::channel::channel& module_channel_, const hw::config& config_)
    : channel(module_channel_, config_) {
    module::module& mod = get_module();
    label = hw::get_module_fixture_label(config_.fixture);
    number = mod.eeprom.db_find(static_cast<int>(module_channel.number));
    base =  mod.eeprom.db_channel_base(number);
    offset = static_cast<int>(module_channel.number - base);
}

void db::acquire_adc() {
    module::module& module = get_module();
    {
        userin_save userins(module);
        userins.update(number, offset);
        hw::run::control(module, hw::run::control_task::get_traces);
    }
    /*
     * Make sure the buffer is the maximum size a user can ask for. The
     * allocation is only done if the buffer is used and it cannot be released
     * because the user can ask to read it at any time.
     */
    const size_t size = hw::max_adc_trace_length;
    if (module_channel.adc_trace.size() != size) {
        module_channel.adc_trace.resize(size);
    }
    hw::memory::dsp dsp(module);
    hw::adc_trace_buffer adc_trace;
    dsp.read(hw::memory::IO_BUFFER_ADDR, adc_trace, size / 2);
    for (size_t w = 0; w < size / 2; ++w) {
        module_channel.adc_trace[w * 2] = hw::adc_word(adc_trace[w] & 0xffff);
        module_channel.adc_trace[w * 2 + 1] = hw::adc_word((adc_trace[w] >> 16) & 0xffff);
    }
}

db04::db04(pixie::channel::channel& module_channel_, const hw::config& config_)
    : db(module_channel_, config_) {
}

void db04::set_dac() {
    module::module& mod = get_module();
    auto dac = mod.read_var(param::channel_var::OffsetDAC, module_channel.number);
    if (dac > 65535) {
        throw error::error(error::code::invalid_value,
                           module::module_label(mod, "DB04") + "invalid DAC offset: channel=" +
                           std::to_string(module_channel.number));
    }
    /*
     * Address bit 1 selects DAC for the upper 4 channels. Clear bit 0 and set
     * bit 1 if the DB channel offset is less than 4.
     */
    hw::word dac_addr = 0x20 + ((offset < 4 ? 1 : 0) << 1);
    /*
     * Channel offset DAC Output
     *     0, 4           B
     *     1, 5           C
     *     2, 6           A
     *     3, 7           D
     */
    hw::word dac_ctrl = 0x30;
    switch (offset) {
    case 0:
    case 4:
        dac_ctrl += 1;
        break;
    case 1:
    case 5:
        dac_ctrl += 2;
        break;
    case 2:
    case 6:
        dac_ctrl += 0;
        break;
    case 3:
    case 7:
        dac_ctrl += 3;
        break;
    default:
        break;
    }
    /*
     * Select the module port
     */
    mod.select_port(number + 1);
    /*
     * CFG_DAC expacts [addr(8), ctrl(8), data(16)]
     */
    mod.write_word(hw::device::CFG_DAC, (dac_addr << 24) | (dac_ctrl << 16) | dac);
     /*
     * It takes about 2ms to clock out the 32 bits
     */
    hw::wait(6000);
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

void channel::set_dac() {
    /* Do nothing */
}

void channel::acquire_adc() {
    throw error::error(error::code::internal_failure,
                       "invalid fixture op: ADC acquire is using the DSP");
}

module::module& channel::get_module() {
    return module_channel.module;
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

};  // namespace fixture
};  // namespace pixie
};  // namespace xia
