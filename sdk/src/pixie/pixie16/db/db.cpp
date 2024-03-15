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
#include <pixie/pixie16/db/db.hpp>
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

    userin_save(pixie::module::module& module);

    void update(const hw::word& db_index, const hw::word& db_channel);

    ~userin_save();
};

userin_save::userin_save(pixie::module::module& module) : dsp(module) {
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

db_mib::db_mib() {
}

void db_mib::register_mib(db& db_, const std::string& name, mib::type type) {
    mib::event_func set =
        [&self = *this, &db_ = db_](mib::event , mib::type type, mib::data_type& data) {
            self.setter(db_, type, data);
        };
    mib::event_func get =
        [&self = *this, &db_ = db_](mib::event , mib::type type, mib::data_type& data) {
            self.getter(db_, type, data);
        };
    mib::add(name, type);
    node = mib::find(name);
    node.set_event_func(mib::event::set, set);
    node.set_event_func(mib::event::get, get);
}

void db_mib::register_ro_mib(const std::string& name, int value) {
    mib::add_ro_int(name, value);
    node = mib::find(name);
}

void db_mib::setter(db& db_, mib::type type, mib::data_type& data) {
    (void) db_;
    (void) type;
    (void) data;
}

void db_mib::getter(db& db_, mib::type type, mib::data_type& data) {
    (void) db_;
    (void) type;
    (void) data;
}


db::db(pixie::channel::channel& module_channel_, const hw::config& config_)
    : channel(module_channel_, config_) {
    pixie::module::module& mod = get_module();
    label = hw::get_module_fixture_label(config_.fixture);
    auto db_number = mod.eeprom.db_find(module_channel.number);
    std::string ll = label;
    util::string::tolower(ll);
    std:: ostringstream oss;
    oss << module_channel.get_mib_base() << ll << mib::mibsep << db_number << mib::mibsep;
    number.register_ro_mib(oss.str() + "number", db_number);
    base.register_ro_mib(oss.str() + "base", mod.eeprom.db_channel_base(db_number));
    offset.register_ro_mib(
        oss.str() + "offset", static_cast<int>(module_channel.number) - base->get<int>());
    adc_state.register_mib(*this, oss.str() + "adc-state", mib::type::string);
    auto key = persistent_key("adc_swap_disabled");
    if (mod.persistent_has(key)) {
        auto& val = mod.persistent_get(key);
        *adc_state = val == "true" ? "disabled" : "boot";
    } else {
        *adc_state = "boot";
    }
}

void db::acquire_adc() {
    pixie::module::module& module = get_module();
    {
        userin_save userins(module);
        userins.update(number->get<int>(), offset->get<int>());
        hw::run::control_run_on_dsp(module, hw::run::control_task::get_traces);
    }
    /*
     * Make sure the buffer is the maximum size a user can ask for. The
     * allocation is only done if the buffer is used and it cannot be released
     * because the user can ask to read it at any time.
     */
    const size_t size = module_channel.fixture->config.max_adc_trace_length;
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

void db::read_adc(hw::adc_word* buffer, size_t size) {
    const size_t copy_size =
        (size < module_channel.adc_trace.size() ?
         size : module_channel.adc_trace.size()) * sizeof(hw::adc_word);
    memcpy(buffer, module_channel.adc_trace.data(), copy_size);
}

void db::set(const std::string item, bool value) {
    if (item == "ADC_SWAP_DISABLE") {
        auto& mod = get_module();
        auto key = persistent_key("adc_swap_disabled");
        mod.persistent_set(key, value ? "true" : "false");
        if (*adc_state == "disabled" || *adc_state == "boot") {
            *adc_state = value ? "disabled" : "boot";
        } else {
            auto& mod = get_module();
            throw pixie::module::error(
                mod.number, mod.slot, pixie::module::error::code::invalid_value,
                "DB swap state already set");
        }
    } else if (item == "ADC_SWAP") {
        if (*adc_state == "boot") {
            *adc_state = value ? "swapped" : "unswapped";
        }
    } else {
        channel::set(item, value);
    }
}

void db::get(const std::string item, bool& value) {
    if (item == "ADC_SWAP_DISABLE") {
        value = (*adc_state == "disabled");
    } else if (item == "ADC_SWAP") {
        value = (*adc_state == "swapped");
    } else {
        channel::get(item, value);
    }
}

void db::get(const std::string item, int& value) {
    if (item == "DB_NUMBER") {
        value = *number;
    } else if (item == "DB_OFFSET") {
        value = *offset;
    } else if (item == "DAC_SETTLE_PERIOD") {
        value = 0;
    } else {
        channel::get(item, value);
    }
}

void db::get(const std::string item, double& value) {
    channel::get(item, value);
}

};  // namespace fixture
};  // namespace pixie
};  // namespace xia
