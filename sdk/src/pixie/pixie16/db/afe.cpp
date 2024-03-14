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
#include <pixie/utils/numerics.hpp>

#include <pixie/pixie16/db/afe.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace fixture {

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

afe_dbs::afe_dbs(pixie::module::module& module__)
    : module(module__), adcctrl { } {
    log(log::info) << pixie::module::module_label(module_, "fixture: afe_dbs")
                    << "create";
}

void afe_dbs::fpga_fippi_loaded() {
    adcctrl = { };
}

void afe_dbs::boot() {
    log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                    << "boot";

    util::time::timepoint tp(true);

    /*
     * Set the voffset for all channels to the low rail
     */
    baseline::channels bl_same;
    set_channel_voffset(module_, -1.5, 1);
    analyze_channel_baselines(bl_same);

    /*
     * Move the voffset for the even channels to high rail
     */
    baseline::channels bl_moved;
    set_channel_voffset(module_, 1.5, 2);
    analyze_channel_baselines(bl_moved);

    /*
     * Check all the channels and swap of the ADCs if required.
     */
    for (int chan = 0; chan < static_cast<int>(module_.num_channels); ++chan) {
        bool swap_disabled = true;
        module_.channels[chan].fixture->get("ADC_SWAP_DISABLE", swap_disabled);
        if (swap_disabled) {
            log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                            << "boot: adc_swap: disabled: " + std::to_string(chan);
            continue;
        }
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
                throw pixie::module::error(
                    module_.number, module_.slot,
                    pixie::module::error::code::module_initialize_failure,
                    "invalid DB number for channel: " + std::to_string(chan));
            }
            hw::word last_adcctrl = adcctrl[chan_db];
            adcctrl[chan_db] |= 1 << (chan_offset / 2);
            log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                            << "boot: adc_swap: chan=" << chan
                            << " db=" << chan_db << " offset=" << chan_offset
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
        analyze_channel_baselines(bl_verify);
        for (size_t chan = 0; chan < module_.num_channels; ++chan) {
            bool swap_disabled = true;
            module_.channels[chan].fixture->get("ADC_SWAP_DISABLE", swap_disabled);
            if (swap_disabled) {
                log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                                << "boot: adc_swap: verify: disabled: " + std::to_string(chan);
                continue;
            }
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
    const int runs = 30;

    util::time::timepoint tp(true);

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

    using bl_linear_fit = util::numerics::linear_fit<int>;
    std::vector<bl_linear_fit> bl_fits(module_.num_channels);
    std::vector<bool> inverted(module_.num_channels, false);

    /*
     * Run while the baseline of a channel is outside the percent+noise margin
     */
    bool run_again = true;
    for (int run = 0; run_again && run < runs; ++run) {
        log(log::debug) << log_leader << "adjust-offsets: run=" << run;
        run_again = false;
        baseline::channels baselines;
        analyze_channel_baselines(baselines, 1);
        for (size_t chan = 0; chan < module_.num_channels; ++chan) {
            auto& channel = module_.channels[chan];
            if (has_offset_dacs[chan]) {
                auto& bl = baselines[chan];
                const int adc_bottom_rail = 0;
                const int adc_top_rail = 1 << bl.adc_bits;
                const int adc_rail_dac_step = adc_top_rail / (runs - 5);
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
                    bl_fit.update(bl.baseline, dac);
                    if (bl_fit.count == 2) {
                        const int delta_adc =
                            std::get<0>(bl_fit.samples[1]) - std::get<0>(bl_fit.samples[0]);
                        const int delta_dac =
                            std::get<1>(bl_fit.samples[1]) - std::get<1>(bl_fit.samples[0]);
                        if ((delta_adc < 0) != (delta_dac < 0) && !inverted[chan]) {
                            inverted[chan] = true;
                            log(log::info) << log_leader
                                           << "adjust-offsets: channel=" << chan
                                           << " input signal may be inverted";
                        }
                    }
                    const char* action = "none";
                    if (bl.baseline == 0 || bl.baseline == adc_top_rail) {
                        action = "rail hit";
                        dac = std::get<0>(offsetdac) +
                            (adc_rail_dac_step * (inverted[chan] ? -1 : 1));
                        log(log::debug) << log_leader
                                        << "adjust-offsets: rail-hit: channel=" << chan
                                        << " baseline=" << bl.baseline
                                        << " od<0>=" << std::get<0>(offsetdac)
                                        << " od<1>=" << std::get<1>(offsetdac)
                                        << " dac=" << dac;

                    } else {
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
                                    << " action=" << action
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

void afe_dbs::analyze_channel_baselines(
    baseline::channels& baselines, const int traces) {
    baselines.resize(
        module_.num_channels,
        baseline::channel(baseline_noise_bins, baseline_noise_margin));
    for (auto& channel : module_.channels) {
        baselines[channel.number].start(channel.number, channel.fixture->config.adc_bits);
    }
    for (int t = 0; t < traces; ++t) {
        module_.get_traces();
        for (auto& channel : module_.channels) {
            xia::pixie::hw::adc_trace trace;
            module_.read_adc(channel.number, trace, false);
            baselines[channel.number].update(trace);
        }
    }
    for (auto& bl : baselines) {
        bl.end();
        log(log::debug) << pixie::module::module_label(module_, "afe-dbs: analyze-baselines")
                        << "channel=" << bl.number
                        << " baseline=" << bl.baseline;
    }
}

};  // namespace fixture
};  // namespace pixie
};  // namespace xia
