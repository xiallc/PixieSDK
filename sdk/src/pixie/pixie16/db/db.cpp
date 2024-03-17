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
namespace db {

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

db::db(pixie::module::module& module__, const hw::db_assembly& db_assembly)
    : assembly(module__, db_assembly.label) {
    xia::log(log::debug) << pixie::module::module_label(module_, "fixture")
                         << db_assembly.label <<  " assembly init: " << db_assembly.index;
    /*
     * Need to build the MIB base here to create the DB's number MIB
     * that us used in the get_mib_base() call or the MIB will raise
     * an error that it is not valid.
     */
    std::ostringstream oss;
    oss << assembly::get_mib_base() << db_assembly.index << mib::mibsep;
    const auto mib_base = oss.str();
    mib::add_ro_int(mib_base + "number", db_assembly.index);
    number = mib::find(mib_base + "number");
    mib::add_ro_uint(
        mib_base + "channels", module_.eeprom.db_channel_count(db_assembly.index));
    channels = mib::find(mib_base + "channels");
    mib::add_ro_uint(
        mib_base + "base", module_.eeprom.db_channel_base(db_assembly.index));
    base = mib::find(mib_base + "base");
    label = db_assembly.label;
}

db::~db() {
    number.remove();
    channels.remove();
    base.remove();
}

std::string db::get_mib_base() {
    std::ostringstream oss;
    oss << assembly::get_mib_base() << number.get<int>() << mib::mibsep;
    return oss.str();
}

void db::online() {
    enable_mibs();
    for (size_t c = 0; c < channels.get<size_t>(); ++c) {
        auto& chan = module_.channels[c + base.get<size_t>()];
        if (chan.fixture) {
            chan.fixture->online();
        }
    }
}

void db::forced_online() {
    enable_mibs();
    for (size_t c = 0; c < channels.get<size_t>(); ++c) {
        auto& chan = module_.channels[c + base.get<size_t>()];
        if (chan.fixture) {
            chan.fixture->forced_online();
        }
    }
}

void db::forced_offline() {
    disable_mibs();
    for (size_t c = 0; c < channels.get<size_t>(); ++c) {
        auto& chan = module_.channels[c + base.get<size_t>()];
        if (chan.fixture) {
            chan.fixture->forced_offline();
        }
    }
}

void db::enable_mibs() {
    number.enable();
    channels.enable();
    base.enable();
    fippi::enable_mibs(fippi_regs);
}

void db::disable_mibs() {
    number.disable();
    channels.disable();
    base.disable();
    fippi::disable_mibs(fippi_regs);
}

channel::channel(
    pixie::channel::channel& module_channel_, db& board_, const hw::config& config_)
    : fixture::channel(module_channel_, board_, config_), board(board_) {
    pixie::module::module& mod = get_module();
    auto chan = static_cast<int>(module_channel.number);
    label = board.label;
    xia::log(log::debug) << pixie::module::module_label(mod, "fixture")
                         << label <<  '.' << board.number.get<int>() << '.' << chan
                         << ": channel init";
    std:: ostringstream oss;
    oss << board.get_mib_base()
        << "channel" << mib::mibsep
        << static_cast<int>(module_channel.number) - board.base.get<int>()
        << mib::mibsep;
    auto mib_base = oss.str();
    mib::add_ro_uint(mib_base + "offset", chan - board.base.get<int>());
    offset = mib::node(mib_base + "offset");
    adc_swap = mib::node(mib_base + "adc.swap", mib::type::string);
    auto key = persistent_key("adc_swap_disabled");
    if (mod.persistent_has(key)) {
        auto& val = mod.persistent_get(key);
        adc_swap = val == "true" ? "disabled" : "boot";
    } else {
        adc_swap = "boot";
    }
}

channel::~channel() {
    offset.remove();
    adc_swap.remove();
}

std::string channel::get_mib_base() {
    std:: ostringstream oss;
    oss << board.get_mib_base()
        << "channel" << mib::mibsep << offset.get<int>() << mib::mibsep;
    return oss.str();
}

void channel::online() {
    enable_mibs();
}

void channel::forced_online() {
    enable_mibs();
}

void channel::forced_offline() {
    disable_mibs();
}

void channel::enable_mibs() {
    offset.enable();
    adc_swap.enable();
}

void channel::disable_mibs() {
    offset.disable();
    adc_swap.disable();
}

void channel::acquire_adc() {
    pixie::module::module& module = get_module();
    {
        userin_save userins(module);
        userins.update(board.number.get<int>(), offset.get<int>());
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

void channel::read_adc(hw::adc_word* buffer, size_t size) {
    const size_t copy_size =
        (size < module_channel.adc_trace.size() ?
         size : module_channel.adc_trace.size()) * sizeof(hw::adc_word);
    memcpy(buffer, module_channel.adc_trace.data(), copy_size);
}

void channel::set(const std::string item, bool value) {
    if (item == "adc.swap.disable") {
        auto& mod = get_module();
        auto key = persistent_key("adc_swap_disabled");
        mod.persistent_set(key, value ? "true" : "false");
        if (adc_swap == "disabled" || adc_swap == "boot") {
            adc_swap = value ? "disabled" : "boot";
        } else {
            auto& mod = get_module();
            throw pixie::module::error(
                mod.number, mod.slot, pixie::module::error::code::invalid_value,
                "DB swap state already set");
        }
    } else if (item == "adc.swap") {
        if (adc_swap == "boot") {
            adc_swap = value ? "swapped" : "unswapped";
        }
    } else {
        fixture::channel::set(item, value);
    }
}

void channel::get(const std::string item, bool& value) {
    if (item == "adc.swap.disable") {
        value = (adc_swap == "disabled");
    } else if (item == "adc.swap") {
        value = (adc_swap == "swapped");
    } else {
        fixture::channel::get(item, value);
    }
}

void channel::get(const std::string item, unsigned int& value) {
    if (item == "db.number") {
        value = board.number.get<unsigned int>();
    } else if (item == "db.offset") {
        value = offset.get<unsigned int>();
    } else if (item == "dac.settle-period") {
        value = 0;
    } else {
        fixture::channel::get(item, value);
    }
}

channel::adc_test_mode channel::get_adc_test_mode(unsigned int mode_value) {
    return static_cast<adc_test_mode>(mode_value);
}

bool channel::check_adc_test_mode(unsigned int mode_value) {
    bool result = true;
    switch (get_adc_test_mode(mode_value)) {
    default:
        result = false;
        break;
    case adc_test_mode::off:
    case adc_test_mode::fs_plus:
    case adc_test_mode::fs_minus:
    case adc_test_mode::midscale:
    case adc_test_mode::checkerboard:
    case adc_test_mode::pn23:
    case adc_test_mode::pn9:
    case adc_test_mode::one_zero:
        break;
    }
    return result;
}

const char* channel::adc_mode_label(adc_test_mode mode) {
    const char* label = "invalid";
    switch (mode) {
    default:
        break;
    case adc_test_mode::off:
        label = "off";
        break;
    case adc_test_mode::fs_plus:
        label = "+FS";
        break;
    case adc_test_mode::fs_minus:
        label = "-FS";
        break;
    case adc_test_mode::midscale:
        label = "midscale";
        break;
    case adc_test_mode::checkerboard:
        label = "checkerboard";
        break;
    case adc_test_mode::pn23:
        label = "pn23";
        break;
    case adc_test_mode::pn9:
        label = "pn9";
        break;
    case adc_test_mode::one_zero:
        label = "one/zero";
        break;
    }
    return label;
}


}   // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
