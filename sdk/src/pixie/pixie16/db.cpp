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

#include <pixie/pixie16/baseline.hpp>
#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/db.hpp>
#include <pixie/pixie16/defs.hpp>
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

static void wait_dac_settle_period(pixie::module::module& mod) {
    /*
     * Find the longest DB settling period and wait that period of time.
     *
     * @note this is only ever called for an `afe-dbs` module fixture and
     *       the default daughter board returns 0.
     */
    int settle_period = 0;
    for (auto& channel : mod.channels) {
        int channel_settle_period;
        channel.fixture->get("DAC_SETTLE_PERIOD", channel_settle_period);
        settle_period = std::max(settle_period, channel_settle_period);
    }
    xia_log(log::debug) << pixie::module::module_label(mod, "afe-dbs: dac-settle-wait")
                        << "period=" << settle_period << " msecs";
    if (settle_period > 0) {
        hw::wait(settle_period * 1000);
    }
}

static void set_channel_voffset(pixie::module::module& mod, double voffset, int step) {
    for (size_t chan = 0; chan < mod.num_channels; chan += step) {
        mod.channels[chan].voffset(voffset);
    }
    mod.set_dacs();
    wait_dac_settle_period(mod);
}

static void analyze_channel_baselines(pixie::module::module& mod,
                                      baseline::channels& baselines,
                                      const int traces = 1) {
    baselines.resize(mod.num_channels);
    for (auto& channel : mod.channels) {
        baselines[channel.number].start(channel.number, channel.fixture->config.adc_bits);
    }
    for (int t = 0; t < traces; ++t) {
        mod.get_traces();
        for (auto& channel : mod.channels) {
            xia::pixie::hw::adc_trace trace;
            mod.read_adc(channel.number, trace, false);
            baselines[channel.number].update(trace);
        }
    }
    for (auto& bl : baselines) {
        bl.end();
        log(log::debug) << pixie::module::module_label(mod, "afe-dbs: analyze-baselines")
                        << "channel=" << bl.number
                        << " baseline=" << bl.baseline;
    }
}

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

db::db(pixie::channel::channel& module_channel_, const hw::config& config_)
    : channel(module_channel_, config_) {
    pixie::module::module& mod = get_module();
    label = hw::get_module_fixture_label(config_.fixture);
    number = mod.eeprom.db_find(module_channel.number);
    base =  mod.eeprom.db_channel_base(number);
    offset = static_cast<int>(module_channel.number) - base;
    adc_state = adc_boot_state;
}

void db::acquire_adc() {
    pixie::module::module& module = get_module();
    {
        userin_save userins(module);
        userins.update(number, offset);
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
    if (item == "ADC_SWAP") {
        if (adc_state == adc_boot_state) {
            adc_state = value ? adc_swapped : adc_unswapped;
        }
    } else {
        channel::set(item, value);
    }
}

void db::get(const std::string item, bool& value) {
    if (item == "ADC_SWAP") {
        value = (adc_state == adc_swapped);
    } else {
        channel::get(item, value);
    }
}

void db::get(const std::string item, int& value) {
    if (item == "DB_NUMBER") {
        value = number;
    } else if (item == "DB_OFFSET") {
        value = offset;
    } else if (item == "DAC_SETTLE_PERIOD") {
        value = 0;
    } else {
        channel::get(item, value);
    }
}

void db::get(const std::string item, double& value) {
    channel::get(item, value);
}

void db::report(std::ostream& out) const {
    channel::report(out);
    out << "DB Number      : " << number << std::endl
        << "DB Base        : " << base << std::endl
        << "DB Offset      : " << offset << std::endl
        << "ADC swap state : ";
    switch (adc_state) {
    case adc_boot_state:
        out << "boot state";
        break;
    case adc_unswapped:
        out << "not swapped";
        break;
    case adc_swapped:
        out << "swapped";
        break;
    }
    out << std::endl;
}


afe_dbs::afe_dbs(pixie::module::module& module__)
    : module(module__), adcctrl { } {
    log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                    << "create";
}

void afe_dbs::fgpa_fippi_loaded() {
    adcctrl = { };
}

void afe_dbs::boot() {
    log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                    << "boot";

    util::timepoint tp(true);

    /*
     * Set the voffset for all channels to the low rail
     */
    baseline::channels bl_same;
    set_channel_voffset(module_, -1.5, 1);
    analyze_channel_baselines(module_, bl_same);

    /*
     * Move the voffset for the even channels to high rail
     */
    baseline::channels bl_moved;
    set_channel_voffset(module_, 1.5, 2);
    analyze_channel_baselines(module_, bl_moved);

    /*
     * Check all the channels and swap of the ADCs if required.
     */
    for (int chan = 0; chan < static_cast<int>(module_.num_channels); ++chan) {
        bool swapped = false;
        if ((chan % 2) == 0) {
            if (bl_same[chan] == bl_moved[chan]) {
                swapped = true;
            }
        } else {
            if (bl_same[chan] != bl_moved[chan]) {
                swapped = true;
            }
        }
        module_.channels[chan].fixture->set("ADC_SWAP", swapped);
        if (swapped) {
            int chan_db;
            int chan_offset;
            module_.channels[chan].fixture->get("DB_NUMBER", chan_db);
            module_.channels[chan].fixture->get("DB_OFFSET", chan_offset);
            if (chan_db >= max_dbs) {
                throw pixie::module::error(module_.number, module_.slot,
                                           pixie::module::error::code::module_initialize_failure,
                                           "invalid DB number for channel: " + std::to_string(chan));
            }
            hw::word last_adcctrl = adcctrl[chan_db];
            adcctrl[chan_db] |= 1 << (chan_offset / 2);
            log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                            << "boot: adc_swap: db="
                            << chan_db << " offset=" << chan_offset
                            << " adcctrl=0x" << std::hex << adcctrl[chan_db];
            if (adcctrl[chan_db] != last_adcctrl) {
                hw::memory::fippi fippi(module_);
                fippi.write(hw::fippi_addr(chan_db, hw::fippi::ADCCTRL), adcctrl[chan_db]);
            }
        }
    }

    /*
     * Verify.
     */
    bool failed = false;
    if (adc_swap_verify) {
        baseline::channels bl_verify;
        analyze_channel_baselines(module_, bl_verify);
        for (size_t chan = 0; chan < module_.num_channels; ++chan) {
            if ((chan % 2) == 0) {
                if (bl_same[chan] == bl_verify[chan]) {
                    failed = true;
                    log(log::error) << pixie::module::module_label(module_, "fixture: afe_dbs")
                                    << "boot: ADC swap failed: " << chan;
                }
            } else {
                if (bl_same[chan] != bl_verify[chan]) {
                    failed = true;
                    log(log::error) << pixie::module::module_label(module_, "fixture: afe_dbs")
                                    << "boot: ADC swap failed: " << chan;
                }
            }
        }
    }

    set_channel_voffset(module_, 0, 1);

    if (failed) {
        throw pixie::module::error(module_.number, module_.slot,
                                   pixie::module::error::code::module_initialize_failure,
                                   "DB AE ADC swap failure");
    }

    log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                    << "boot: duration=" << tp;
}

void afe_dbs::init_channels() {
    log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                    << "init-channels: create channel fixtures";
    for (int chan = 0; chan < static_cast<int>(module_.num_channels); ++chan) {
        module_.channels[chan].fixture =
            fixture::make(module_.channels[chan], module_.eeprom.configs[chan]);
    }
}

void afe_dbs::set_dacs() {
    for (auto& channel : module_.channels) {
        auto dac_offset = module_.read_var(param::channel_var::OffsetDAC, channel.number);
        channel.fixture->set_dac(dac_offset);
    }
}

void afe_dbs::get_traces() {
    for (auto& channel : module_.channels) {
        channel.fixture->acquire_adc();
    }
}

void afe_dbs::adjust_offsets() {
    const std::string log_leader =
        pixie::module::module_label(module_, "fixture: afe_dbs");
    log(log::debug) << log_leader << "adjust-offsets";

    /*
     * Adjust the offset of the signal using the OffsetDAC. This is the VOFFSET
     * channel parameter.
     *
     * The speed of this process is driven by:
     *  - The speed we can capture ADC traces for all channels
     *  - The settling time of the offset voltage circuit.
     *
     * The offset procedure consists of 2 stages. The first moves the DAC a
     * small amount measuring the baseline at each point and the least squares
     * estimate. The second stage uses the least squares estimate to
     * interpolate the target ADC baseline value to a DAC value. Repeat until
     * the baseline converges to the target ADC value.
     */
    const double voffset_start_voltage = 0.0;
    const int dac_bits = 16;
    const int dac_linear_fit_steps = 200;
    const int dac_linear_fit_samples = 2;
    const double baseline_noise_margin = 0.5; /* % */
    const int runs = 10;

    util::timepoint tp(true);

    /*
     * Remove any offset
     */
    set_channel_voffset(module_, voffset_start_voltage, 1);

    std::vector<double> bl_percents;
    std::vector<std::pair<int, int>> offsetdacs;
    std::vector<bool> has_offset_dacs;

    for (auto& channel : module_.channels) {
        bl_percents.push_back(channel.baseline_percent());
        offsetdacs.push_back(
            std::make_pair(module_.read_var(param::channel_var::OffsetDAC, channel.number), 0));
        bool has_offset_dac;
        channel.fixture->get("HAS_OFFSET_DAC", has_offset_dac);
        has_offset_dacs.push_back(has_offset_dac);
    }

    using bl_linear_fit = util::linear_fit<int>;
    std::vector<bl_linear_fit> bl_fits(module_.num_channels);
    std::vector<bool> inverted(module_.num_channels, false);

    /*
     * Run while the baseline of a channel is outside the percent+noise margin
     */
    bool run_again = true;
    for (int run = 0; run_again && run < runs; ++run) {
        log(log::debug) << log_leader << "adjust-offsets: run=" << run;
        run_again = false;
        baseline::channels baselines(baseline_noise_margin);
        analyze_channel_baselines(module_, baselines, 1);
        for (size_t chan = 0; chan < module_.num_channels; ++chan) {
            auto& channel = module_.channels[chan];
            if (has_offset_dacs[chan]) {
                auto& bl = baselines[chan];
                const int adc_bottom_rail = 0;
                const int adc_top_rail = 1 << bl.adc_bits;
                const int adc_target =
                    int((adc_top_rail - adc_bottom_rail) * (bl_percents[chan] / 100));
                auto& offsetdac = offsetdacs[chan];
                int dac = std::get<0>(offsetdac);
                /*
                 * The compare includes the noise margin that is set in the
                 * baseline
                 */
                if (bl != adc_target) {
                    /*
                     * If a rail has been been hit do not update the linear fit
                     * because the ADC value is unknown, ie we cannot see past
                     * the voltage rail we have hit. Half the step distance and
                     * try again.
                     */
                    log(log::debug) << log_leader
                                    << "adjust-offsets: channel=" << chan
                                    << " adc-target=" << adc_target
                                    << " bl=" << bl.baseline
                                    << " offset-dac=" << dac;
                    bl_linear_fit& bl_fit = bl_fits[chan];
                    const char* action = "none";
                    if (bl.baseline == 0 || bl.baseline == adc_top_rail) {
                        action = "rail hit";
                        dac = std::get<1>(offsetdac) +
                            ((std::get<0>(offsetdac) - std::get<1>(offsetdac)) / 2);
                        log(log::debug) << log_leader
                                        << "adjust-offsets: rail-hit: channel=" << chan
                                        << " od<0>=" << std::get<0>(offsetdac)
                                        << " od<1>=" << std::get<1>(offsetdac)
                                        << " dac=" << dac;

                    } else {
                        bl_fit.update(bl.baseline, dac);
                        if (bl_fit.count == 2) {
                            const int delta_adc =
                                std::get<0>(bl_fit.samples[1]) - std::get<0>(bl_fit.samples[0]);
                            const int delta_dac =
                                std::get<1>(bl_fit.samples[1]) - std::get<1>(bl_fit.samples[0]);
                            if ((delta_adc < 0) != (delta_dac < 0)) {
                                inverted[chan] = true;
                                log(log::info) << log_leader
                                               << "adjust-offsets: channel=" << chan
                                               << " input signal may be inverted";
                            }
                        }
                        if (bl_fit.count < dac_linear_fit_samples) {
                            action = "linear-fit";
                            const int steps = dac_linear_fit_steps * (inverted[chan] ? 1 : -1);
                            if (adc_target > bl.baseline) {
                                dac += steps;
                            } else {
                                dac -= steps;
                            }
                        } else {
                            action = "interpolate";
                            bl_fit.calc();
                            log(log::debug) << log_leader
                                            << "adjust-offsets: update: channel=" << chan
                                            << " " << bl_fit.k << "X + " << bl_fit.c;
                            dac = bl_fit.y(adc_target);
                        }
                    }
                    dac = std::max(0, dac);
                    dac = std::min(1 << dac_bits, dac);
                    log(log::debug) << log_leader
                                    << "adjust-offsets: update: channel=" << chan
                                    << ' ' << action
                                    << ": adc-error=" << adc_target - bl.baseline
                                    << " dac-error=" << std::get<0>(offsetdac) - dac
                                    << " dac=" << dac;
                    offsetdacs[chan] = std::make_pair(dac, std::get<0>(offsetdac));
                    channel.fixture->set_dac(dac);
                    run_again = true;
                }
            }
        }
        if (run_again) {
            /*
             * Wait until the signal settles after the update.
             */
            wait_dac_settle_period(module_);
        }
    }
    for (size_t chan = 0; chan < module_.num_channels; ++chan) {
        module_.write_var(
            param::channel_var::OffsetDAC, std::get<0>(offsetdacs[chan]), chan);
    }
    log(log::debug) << log_leader
                    << "adjust-offsets: duration=" << tp;
}

db04::db04(pixie::channel::channel& module_channel_, const hw::config& config_)
    : db(module_channel_, config_) {
}

void db04::set_dac(param::value_type value) {
    pixie::module::module& mod = get_module();
    if (value > 65535) {
        throw error::error(error::code::invalid_value,
                           pixie::module::module_label(mod, "DB04") + "invalid DAC offset: channel=" +
                           std::to_string(module_channel.number));
    }
    /*
     * Select the module port
     */
    mod.select_port(number + 1);
    /*
     * Address bit 1 selects DAC for the upper 4 channels. Clear bit 0 and set
     * bit 1 if the DB channel offset is less than 4.
     */
    hw::word dac_addr = 0x20 | ((offset < 4 ? 1 : 0) << 1);
    /*
     * Compensate for PCB ADC swapping:
     *
     * Channel offset DAC Output
     *     0, 4           B (1)
     *     1, 5           C (2)
     *     2, 6           A (0)
     *     3, 7           D (3)
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
     * CFG_DAC expacts [addr(8), ctrl(8), data(16)]
     */
    const hw::word dac = (dac_addr << 24) | (dac_ctrl << 16) | value;
    log(log::debug) << pixie::module::module_label(mod, "fixture: db04")
                    << "db=" << number
                    << " db_channel=" << offset
                    << std::hex
                    << " dac_addr=0x" << dac_addr
                    << " dac_ctrl=0x" << dac_ctrl
                    << " dac_value=0x" << value
                    << " write=0x" << dac;
    mod.write_word(hw::device::CFG_DAC, dac);
     /*
     * It takes about 4ms to clock out the 32 bits
     */
    hw::wait(6000);
}

void db04::get(const std::string item, bool& value) {
    if (item == "HAS_OFFSET_DAC") {
        value = true;
    } else {
        db::get(item, value);
    }
}

void db04::get(const std::string item, int& value) {
    if (item == "DAC_SETTLE_PERIOD") {
        value = dac_settle_time_ms;
    } else {
        db::get(item, value);
    }
}

};  // namespace fixture
};  // namespace pixie
};  // namespace xia
