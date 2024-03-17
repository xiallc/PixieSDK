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
#include <pixie/utils/io.hpp>
#include <pixie/utils/numerics.hpp>

#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

#include <pixie/pixie16/db/db04.hpp>
#include <pixie/pixie16/db/db10.hpp>
#include <pixie/pixie16/db/module.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {

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
        channel.fixture->get("dac.settle-period", channel_settle_period);
        settle_period = std::max(settle_period, channel_settle_period);
    }
    xia_log(log::debug) << pixie::module::module_label(mod, "afe-dbs: dac-settle-wait")
                        << "period=" << settle_period << " msecs";
    if (settle_period > 0) {
        hw::wait(settle_period * 1000);
    }
}

static void set_channel_voffset(module& asm_module, double voffset, int step) {
    pixie::module::module& mod = asm_module.module_;
    bool voffset_set = false;
    for (size_t chan = 0; chan < mod.num_channels; chan += step) {
        if (asm_module.check_channel_feature("adc.swap.disable", chan)) {
            mod.channels[chan].voffset(voffset);
            voffset_set = true;
        }
    }
    if (voffset_set) {
        mod.set_dacs();
        wait_dac_settle_period(mod);
    }
}

static void trace_memdump(
    log_output& out, const xia::pixie::hw::adc_trace& trace,
    size_t offset, size_t length, const std::string& label) {
    size_t start = offset & (8 - 1);
    if (start > 8) {
        start -= 8;
    }
    size_t end = start + length;
    if (end > trace.size()) {
        end = trace.size();
    }
    util::string::strings memdump;
    util::io::write_string_func writer =
        [&memdump](const std::string& s) {
            memdump.push_back(s);
        };
    util::io::memdump(trace.data() + start, end, writer, label, 2, 16, start);
    for (auto& s : memdump) {
        out(s, log::debug);
    }
}

/*
 * The ADC trace is a time based sample of the data arriving from the
 * ADC to the FIPPI. This means there is no continous stream of data
 * from the ADC we can verify the test mode selected by the ADC. This
 * makes the checkerboard test tricky because the word 1 and word 2
 * may not change or it could switch depending on the phase
 * relationship between the DSP sampling the ADC data rate.
 */
static size_t trace_check(
    const xia::pixie::hw::adc_trace& trace, channel::adc_test_mode mode,
    const size_t adc_bits, bool inverted = false) {
    const hw::adc_word mask = (1 << adc_bits) - 1;
    hw::adc_word w1;
    hw::adc_word w2;
    switch (mode) {
    default:
    case channel::adc_test_mode::fs_plus:
        w1 = w2 = mask;
        break;
    case channel::adc_test_mode::fs_minus:
        w1 = w2 = 0;
        break;
    case channel::adc_test_mode::midscale:
        w1 = w2 = 1 << (adc_bits - 1);
        break;
    case channel::adc_test_mode::checkerboard:
        w1 = 0x55555555 & mask;
        w2 = 0xaaaaaaaa & mask;
        break;
    case channel::adc_test_mode::one_zero:
        w1 = mask;
        w2 = 0;
        break;
    }
    if (inverted) {
        w1 = (~w1) & mask;
        w2 = (~w2) & mask;
    }
    for (size_t s = 0; s < trace.size(); s += 2) {
        if (trace[s] != w1 || trace[s + 1] != w2) {
            if (w1 != w2) {
                if (trace[s] == w1 || trace[s] == w2 ||
                    trace[s + 1] == w1 || trace[s + 1] == w2) {
                    continue;
                }
            }
            return s;
        }
    }
    return trace.size();
}

module::module(pixie::module::module& module__)
    : fixture::assembly(module__, "afe-dbs"), adcctrl { } {
    log(log::debug) << pixie::module::module_label(module_, "fixture: afe-dbs")
                    << "motherboard assembly";
    testing = mib::node(get_mib_base() + "testing", mib::type::string);
    testing = "idle";
    mib::event_func testing_set =
        [&self = *this](mib::event , mib::type , mib::data_type& data) {
            if (data.s == "adc") {
                self.adc_self_test(false);
            }
            data.s = "idle";
        };
    testing.set_event_func(mib::event::set, testing_set);
}

void module::fpga_fippi_loaded() {
    adcctrl = { };
}

void module::boot() {
    log(log::debug) << pixie::module::module_label(module_, "fixture: afe-dbs")
                    << "boot";

    /*
     * Tell the assemblies we have booted
     */
    fixture::assembly::boot();

    adc_self_test();
}

void module::adc_self_test(bool use_log) {
    log_output out = [use_log](const std::string& s, log::level level) {
        if (use_log) {
            xia_log(level) << s;
        } else {
            std::cout << s << std::endl;
        }
    };
    adc_sweet_spot(out);
    adc_pattern_check(out);
    adc_channel_swap(out);
}

bool module::adc_pattern_check(log_output& out) {
    if (!has_test("adc.pattern-check")) {
        return false;
    }
    bool result = true;
    std::vector<channel::adc_test_mode> modes = {
        channel::adc_test_mode::fs_plus,
        channel::adc_test_mode::fs_minus,
        channel::adc_test_mode::midscale,
        channel::adc_test_mode::checkerboard };
    std::vector<bool> test_chan(module_.num_channels);
    for (size_t chan = 0; chan < module_.num_channels; ++chan) {
        test_chan[chan] = check_channel_feature("adc.has.test-mode", chan);
    }
    for (auto mode : modes) {
        for (size_t chan = 0; chan < module_.num_channels; ++chan) {
            if (test_chan[chan]) {
                module_.channels[chan].fixture->set(
                    "adc.test-mode", static_cast<int>(mode));
            }
        }
        module_.get_traces();
        for (auto& channel : module_.channels) {
            if (test_chan[channel.number] &&
                (mode == channel::adc_test_mode::fs_plus ||
                 check_channel_feature("adc.test-mode.result.pass", channel.number))) {
                xia::pixie::hw::adc_trace trace;
                module_.read_adc(channel.number, trace, false);
                bool adc_inverted =
                    check_channel_feature("adc.default.inverted");
                auto fail_offset =
                    trace_check(trace, mode, channel.fixture->config.adc_bits, adc_inverted);
                int test_result = 0;
                if (fail_offset < trace.size()) {
                    result = false;
                    size_t db_number;
                    channel.fixture->get("db.number", db_number);
                    size_t db_offset;
                    channel.fixture->get("db.offset", db_offset);
                    std::ostringstream oss;
                    oss << "ADC Test: fail: channel=" << channel.number
                        << " db=" << db_number << '/' << db_offset
                        << " test-mode=" << int(mode);
                    trace_memdump(out, trace, fail_offset, 32, oss.str());
                    test_result = static_cast<int>(mode);
                }
                channel.fixture->set("adc.test-mode.result", test_result);
            }
        }
    }
    for (size_t chan = 0; chan < module_.num_channels; ++chan) {
        if (test_chan[chan]) {
            module_.channels[chan].fixture->set(
                "adc.test-mode", static_cast<int>(channel::adc_test_mode::off));
        }
    }
    return result;
}

bool module::adc_sweet_spot(log_output& out) {
    bool result = true;
    /*
     * Zero means sweet spot sweeping is not supported
     */
    const auto mode = channel::adc_test_mode::checkerboard;
    size_t max_bitslip_bits = 0;
    std::vector<size_t> bitslip_bits(module_.num_channels, 0);
    for (size_t chan = 0; chan < module_.num_channels; ++chan) {
        if (check_channel_feature("adc.has.test-mode", chan) &&
            check_channel_feature("adc.has.bitslip", chan)) {
            module_.channels[chan].fixture->get(
                "adc.bitslip.bits", bitslip_bits[chan]);
            if (bitslip_bits[chan] > max_bitslip_bits) {
                max_bitslip_bits = bitslip_bits[chan];
            }
        }
    }
    if (max_bitslip_bits > 0) {
        for (size_t chan = 0; chan < module_.num_channels; ++chan) {
            if (bitslip_bits[chan] > 0 &&
                bitslip_bits[chan] <= max_bitslip_bits) {
                module_.channels[chan].fixture->set(
                    "adc.test-mode", static_cast<int>(mode));
            }
        }
        std::vector<std::vector<bool>>
            results(module_.num_channels, std::vector<bool>(max_bitslip_bits, false));
        for (size_t bit = 0; bit < max_bitslip_bits; ++bit) {
            for (size_t chan = 0; chan < module_.num_channels; ++chan) {
                if (bitslip_bits[chan] > 0 &&
                    bitslip_bits[chan] <= max_bitslip_bits) {
                    module_.channels[chan].fixture->set("adc.bitslip", bit);
                }
            }
            hw::wait(1000);
            module_.get_traces();
            for (auto& channel : module_.channels) {
                auto chan = channel.number;
                if (bitslip_bits[chan] > 0 &&
                    bitslip_bits[chan] <= max_bitslip_bits) {
                    xia::pixie::hw::adc_trace trace;
                    module_.read_adc(chan, trace, false);
                    bool adc_inverted =
                        check_channel_feature("adc.default.inverted");
                    auto fail_offset =
                        trace_check(trace, mode, channel.fixture->config.adc_bits, adc_inverted);
                    results[chan][bit] = fail_offset == trace.size();
                    if (!results[chan][bit]) {
                        result = false;
                        std::ostringstream oss;
                        oss << "ADC Sweet Spot Test: channel=" << channel.number
                            << " bitslip=" << bit;
                        log_output writer =
                            [](const std::string& s, log::level ) {
                                xia_log(log::debug) << s;
                            };
                        trace_memdump(writer, trace, fail_offset, 32, oss.str());
                    }
                }
            }
        }
        std::vector<std::array<size_t, 2>> sweep_spot(module_.num_channels, {0, 0});
        for (size_t chan = 0; chan < module_.num_channels; ++chan) {
            if (bitslip_bits[chan] > 0 && bitslip_bits[chan] <= max_bitslip_bits) {
                module_.channels[chan].fixture->set(
                    "adc.test-mode", static_cast<int>(channel::adc_test_mode::off));
                bool in_good = false;
                size_t good_start = 0;
                size_t good_end = 0;
                for (size_t bit = 0; bit < bitslip_bits[chan]; ++bit) {
                    if (!in_good) {
                        if (results[chan][bit]) {
                            in_good = true;
                            good_start = good_end = bit;
                        }
                    } else {
                        if (results[chan][bit]) {
                            good_end = bit;
                        } else {
                            in_good = false;
                            size_t good_size = good_end - good_start;
                            size_t max_good_size =
                                sweep_spot[chan][1] - sweep_spot[chan][0];
                            if (good_size > max_good_size) {
                                sweep_spot[chan][0] = good_start;
                                sweep_spot[chan][1] = good_end;
                            }
                        }
                    }
                }
                if (sweep_spot[chan][0] == 0 && sweep_spot[chan][1] == 0) {
                    sweep_spot[chan][0] = good_start;
                    sweep_spot[chan][1] = good_end;
                }
            }
        }
        /*
         * Update the hardware to use the sweet spot
         */
        for (size_t chan = 0; chan < module_.num_channels; ++chan) {
            if (bitslip_bits[chan] > 0 && bitslip_bits[chan] <= max_bitslip_bits) {
                size_t max_good_size = sweep_spot[chan][1] - sweep_spot[chan][0];
                size_t sweet_spot = sweep_spot[chan][0] + max_good_size / 2;
                module_.channels[chan].fixture->set("adc.bitslip", sweet_spot);
            }
        }
        std::ostringstream hdr1;
        hdr1 << " X=bit  ";
        for (size_t bit = 0; bit < module_.num_channels; ++bit) {
            if (bit > 0 && (bit % 10) == 0) {
                hdr1 << bit / 10;
            } else {
                hdr1 << ' ';
            }
        }
        out(hdr1.str(), log::debug);
        std::ostringstream hdr2;
        hdr2 << " Y=chan ";
        for (size_t bit = 0; bit < module_.num_channels; ++bit) {
            hdr2 << bit % 10;
        }
        hdr2 << " SS Range";
        out(hdr2.str(), log::debug);
        for (size_t chan = 0; chan < module_.num_channels; ++chan) {
            std::ostringstream row;
            row << std::setw(7) << chan << ' ';
            for (size_t b = 0; b < max_bitslip_bits; ++b) {
                if (results[chan][b]) {
                    row << 'G';
                } else {
                    row << '_';
                }
            }
            size_t max_good_size = sweep_spot[chan][1] - sweep_spot[chan][0];
            size_t sweet_spot = sweep_spot[chan][0] + max_good_size / 2;
            row << std::setw(3) << sweet_spot
                << " [" << sweep_spot[chan][0] << ','
                << sweep_spot[chan][1] << ']';
            out(row.str(), log::debug);
        }
    }
    return result;
}

bool module::adc_channel_swap(log_output& out) {
    bool result = true;
    /*
     * Some DBs do not have the swap problem and will disable swapping
     */
    if (check_channel_feature("adc.swap.disable")) {
        return false;
    }

    util::time::timepoint tp(true);

    /*
     * Set the voffset for all channels to the low rail
     */
    baseline::channels bl_same;
    set_channel_voffset(*this, -1.5, 1);
    analyze_channel_baselines(bl_same);

    /*
     * Move the voffset for the even channels to high rail
     */
    baseline::channels bl_moved;
    set_channel_voffset(*this, 1.5, 2);
    analyze_channel_baselines(bl_moved);

    /*
     * Check all the channels and swap of the ADCs if required.
     */
    for (int chan = 0; chan < static_cast<int>(module_.num_channels); ++chan) {
        if (!check_channel_feature("adc.swap.disable", chan)) {
            std::stringstream oss;
            oss << pixie::module::module_label(module_, "fixture: afe-dbs")
                << "boot: adc_swap: disabled: " + std::to_string(chan);
            out(oss.str(), log::debug);
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
        module_.channels[chan].fixture->set("adc.swap", swapped);
        if (swapped) {
            int chan_db;
            int chan_offset;
            module_.channels[chan].fixture->get("db.number", chan_db);
            module_.channels[chan].fixture->get("db.offset", chan_offset);
            if (chan_db >= max_dbs) {
                throw pixie::module::error(
                    module_.number, module_.slot,
                    pixie::module::error::code::module_initialize_failure,
                    "invalid DB number for channel: " + std::to_string(chan));
            }
            hw::word last_adcctrl = adcctrl[chan_db];
            adcctrl[chan_db] |= 1 << (chan_offset / 2);
            std::stringstream oss;
            oss << pixie::module::module_label(module_, "fixture: afe-dbs")
                << "boot: adc_swap: chan=" << chan
                << " db=" << chan_db << " offset=" << chan_offset
                << " adcctrl=0x" << std::hex << adcctrl[chan_db];
            out(oss.str(), log::debug);
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
            if (!check_channel_feature("adc.swap.disable", chan)) {
                std::stringstream oss;
                oss << pixie::module::module_label(module_, "fixture: afe-dbs")
                    << "boot: adc_swap: verify: disabled: " + std::to_string(chan);
                out(oss.str(), log::debug);
                continue;
            }
            if ((chan % 2) == 0) {
                if (bl_same[chan] == bl_verify[chan]) {
                    failed = true;
                    std::stringstream oss;
                    oss << pixie::module::module_label(module_, "fixture: afe-dbs")
                        << "boot: ADC swap failed: " << chan;
                    out(oss.str(), log::debug);
                }
            } else {
                if (bl_same[chan] != bl_verify[chan]) {
                    failed = true;
                    std::stringstream oss;
                    oss << pixie::module::module_label(module_, "fixture: afe-dbs")
                        << "boot: ADC swap failed: " << chan;
                    out(oss.str(), log::debug);
                }
            }
        }
    }

    result = !failed;

    set_channel_voffset(*this, 0, 1);

    if (failed) {
        throw pixie::module::error(module_.number, module_.slot,
                                   pixie::module::error::code::module_initialize_failure,
                                   "DB AE ADC swap failure");
    }

    std::stringstream oss;
    oss << pixie::module::module_label(module_, "fixture: afe-dbs")
        << "boot: duration=" << tp;
    out(oss.str(), log::debug);

    return result;
}

void module::init_assemblies() {
    log(log::debug) << pixie::module::module_label(module_, "fixture: db::module")
                    << "init-assemblies: create subassemblies for " + label;
    for (auto& db_assembly : module_.eeprom.dbs) {
        assembly_ptr assem_fixture;
        switch (hw::get_module_fixture(db_assembly.label)) {
        case hw::module_fixture::DB04:
        case hw::module_fixture::DB05:
            assem_fixture = std::make_shared<db04>(module_, db_assembly);
            break;
        case hw::module_fixture::DB10:
            assem_fixture = std::make_shared<db10>(module_, db_assembly);
            break;
        default:
            throw pixie::module::error(
                module_.number, module_.slot,
                pixie::module::error::code::module_initialize_failure,
                "invalid DB for " + label + ": " + db_assembly.label);
            break;
        }
        subassemblies.push_back(assem_fixture);
    }
    assembly::init_assemblies();
}

void module::set_dacs() {
    for (auto& channel : module_.channels) {
        auto dac_offset = module_.read_var(param::channel_var::OffsetDAC, channel.number);
        channel.fixture->set_dac(dac_offset);
    }
}

void module::get_traces() {
    for (auto& channel : module_.channels) {
        channel.fixture->acquire_adc();
    }
}

void module::adjust_offsets() {
    const std::string log_leader =
        pixie::module::module_label(module_, "fixture: afe-dbs");
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
    set_channel_voffset(*this, voffset_start_voltage, 1);

    std::vector<double> bl_percents;
    std::vector<std::pair<int, int>> offsetdacs;
    std::vector<bool> has_offset_dacs;

    for (auto& channel : module_.channels) {
        bl_percents.push_back(channel.baseline_percent());
        offsetdacs.push_back(
            std::make_pair(module_.read_var(param::channel_var::OffsetDAC, channel.number), 0));
        bool has_offset_dac;
        channel.fixture->get("dac.has.offset", has_offset_dac);
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

bool module::has_test(const std::string test) {
    if (test == "adc.pattern-check") {
        return check_channel_feature("adc.has.test-mode");
    } else if (test == "adc.sweet-spot") {
        return
            check_channel_feature("adc.has.test-mode") &&
            check_channel_feature("adc.has.bitslip");
    }
    return false;
}

void module::run_test(const std::string test, bool& result, log_output& out) {
    result = false;
    if (test == "adc.pattern-check") {
        result = adc_pattern_check(out);
    } else if (test == "adc.sweet-spot") {
        result = adc_sweet_spot(out);
    } else {
        out("invalid test" + test, log::error);
    }
}

void module::analyze_channel_baselines(
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

bool module::check_channel_feature(std::string item) {
    bool enabled = false;
    for (size_t chan = 0; chan < module_.num_channels && !enabled; ++chan) {
        enabled = check_channel_feature(item, chan);
    }
    return enabled;
}

bool module::check_channel_feature(std::string item, size_t chan) {
    bool enabled = false;
    if (module_.channels[chan].fixture) {
        module_.channels[chan].fixture->get(item, enabled);
    }
    return enabled;
}
};  // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
