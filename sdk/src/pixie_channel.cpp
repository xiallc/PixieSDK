/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
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

#include <cmath>

#include <pixie_channel.hpp>
#include <pixie_log.hpp>
#include <pixie_module.hpp>
#include <pixie_util.hpp>

#include <hw/memory.hpp>
#include <hw/run.hpp>

namespace xia
{
namespace pixie
{
namespace channel
{
/*
 * System FPGA clock frequency in MHZ
 */
static const size_t SYSTEM_CLOCK_MHZ = 100;

/*
 * Length of each baseline (default: 2 timestamp words + 16 baselines)
 */
static const size_t BASELINES_BLOCK_LEN = 18;

/*
 * Limits.
 */
static const size_t FASTFILTER_MAX_LEN = 127;
static const size_t FASTLENGTH_MIN_LEN = 2;
static const size_t FAST_THRESHOLD_MAX = 65535;
static const size_t SLOWFILTER_MAX_LEN = 127;
static const size_t SLOWLENGTH_MIN_LEN = 2;
static const size_t SLOWGAP_MIN_LEN = 3;
static const size_t TRACEDELAY_MAX = 1023;
static const size_t DAC_VOLTAGE_RANGE = 3;
static const size_t DSP_CLOCK_MHZ = 100;
static const size_t FASTTRIGBACKLEN_MIN_100MHZFIPCLK = 1;
static const size_t FASTTRIGBACKLEN_MIN_125MHZFIPCLK = 2;
static const size_t FASTTRIGBACKLEN_MAX = 4095;
static const size_t CCSRA_ENARELAY = 14;
static const size_t CFDDELAY_MIN = 1;
static const size_t CFDDELAY_MAX = 1;
static const size_t CFDSCALE_MAX = 7;
static const size_t CFDTHRESH_MIN = 1;
static const size_t CFDTHRESH_MAX = 65535;
static const size_t QDCLEN_MIN = 1;
static const size_t QDCLEN_MAX = 32767;
static const size_t EXTTRIGSTRETCH_MIN = 1;
static const size_t EXTTRIGSTRETCH_MAX = 4095;
static const size_t VETOSTRETCH_MIN = 1;
static const size_t VETOSTRETCH_MAX = 4095;
static const size_t EXTDELAYLEN_MAX_REVBCD = 255;
static const size_t EXTDELAYLEN_MAX_REVF = 511;
static const size_t EXTDELAYLEN_MIN = 0;
static const size_t CHANTRIGSTRETCH_MIN = 1;
static const size_t CHANTRIGSTRETCH_MAX = 4095;

static param::value_type
round(double value)
{
    value = value < 0.0 ? std::ceil(value - 0.5) : std::floor(value + 0.5);
    return static_cast<param::value_type>(value);
}

static std::string
channel_label(const int num, const int slot, const int chan)
{
    std::ostringstream oss;
    oss << "channel: num=" << num << ",slot=" << slot
        << ",chan=" << chan << ": ";
    return oss.str();
}

static std::string
channel_label(const channel& chan)
{
    module::module& mod = chan.module.get();
    return channel_label(mod.number, mod.slot, chan.number);
}

error::error(const int num, const int slot, const int channel,
             const code type, const std::ostringstream& what)
    : pixie::error::error(type,
                          make_what(num, slot, channel, what.str().c_str()))
{
}

error::error(const int num, const int slot, const int channel,
             const code type, const std::string& what)
    : pixie::error::error(type, make_what(num, slot, channel, what.c_str()))
{
}

error::error(const int num, const int slot, const int channel,
             const code type, const char* what)
    : pixie::error::error(type, make_what(num, slot, channel, what))
{
}

void
error::output(std::ostream& out)
{
    util::ostream_guard flags(out);
    out << std::setfill(' ')
        << "error: code=" << std::setw(2) << result()
        << ' ' << what();
}

std::string
error::make_what(const int num,
                 const int slot,
                 const int channel,
                 const char* what_)
{
    std::ostringstream oss;
    oss << channel_label(num, slot, channel) << what_;
    return oss.str();
}

baseline::baseline(channel& channel__)
    : channel_(channel__),
      cut(0)
{
}

void
baseline::find_cut(size_t num)
{
    log(log::info) << channel_label(channel_) << "find bl cut: num=" << num;

    module::module& mod = channel_.module.get();
    const size_t chan = channel_.number;

    util::timepoint tp(true);

    values.resize(num);

    param::value_type log2_bweight =
        mod.read_var(param::channel_var::Log2Bweight, chan, 0, false);
    param::value_type current_bl_cut =
        mod.read_var(param::channel_var::BLcut, chan, 0, false);

    mod.write_var(param::channel_var::Log2Bweight, 0, chan);
    mod.write_var(param::channel_var::BLcut, 0, chan);

    try {
        compute_cut();
        mod.write_var(param::channel_var::BLcut, cut, chan);
        compute_cut();
        mod.write_var(param::channel_var::BLcut, cut, chan);
        log(log::info) << channel_label(channel_) << "bl cut=" << cut;
    } catch (...) {
        try {
            mod.write_var(param::channel_var::Log2Bweight,
                          log2_bweight,
                          chan);
            mod.write_var(param::channel_var::Log2Bweight,
                          current_bl_cut,
                          chan);
        } catch (...) {
            /* ignore nesting exceptions, keep the first */
        }
        throw;
    }

    mod.write_var(param::channel_var::Log2Bweight, log2_bweight, chan);

    tp.end();

    log(log::info) << channel_label(channel_) << "find bl cut: cut=" << cut
                   << " duration=" << tp;
}

double
baseline::time(hw::word time_word0, hw::word time_word1)
{
    double time;
    time = double(static_cast<uint64_t>(time_word0) << 32);
    time += double(time_word1);
    time *= 1.0e-6 / double(SYSTEM_CLOCK_MHZ);
    return time;
}

void
baseline::get()
{
    module::module& mod = channel_.module.get();
    const size_t chan = channel_.number;

    hw::memory::dsp dsp(mod);
    hw::io_buffer buffer;

    log(log::debug) << channel_label(channel_) << "baseline get";

    hw::run::control(mod, hw::run::control_task::get_baselines);

    dsp.read(hw::memory::IO_BUFFER_ADDR, buffer);

    double starttime = time(buffer[0], buffer[1]);

    for (size_t bl = 0; bl < values.size(); ++bl) {
        const size_t offset = 2 + (bl * BASELINES_BLOCK_LEN);
        double timestamp =
            time(buffer[offset], buffer[offset + 1]) - starttime;
        double baseline = util::ieee_float(buffer[offset + 2 + chan]);
        values[bl] = value(timestamp, baseline);
    }
}

void
baseline::compute_cut()
{
    double sdev = 0.0;
    size_t sdev_count = 0;

    cut = 0;

    for (size_t count = 0; count < 10; ++count) {
        get();
        for (size_t bl = 0; bl < (values.size() - 1); ++bl) {
            double val =
                std::fabs(values[bl].second - values[bl + 1].second);
            if (val != 0) {
                if (val < (10.0 * values[bl].second) &&
                    val < (10.0 * values[bl + 1].second)) {
                    /*
                     * @todo This peice of logic does not make sense because
                     * `cut` is set to 0 and not touched. It is a form of the
                     * code from before but it needs to be checked.
                     */
                    if (cut == 0 || val < cut) {
                        sdev += val;
                        ++sdev_count;
                    }
                }
            }
        }
    }

    if (sdev_count > 0) {
        const double sqrpi = std::sqrt(M_PI_2);
        double bl_sigma = sdev * sqrpi / sdev_count;
        cut = static_cast<param::value_type>(std::floor(8.0 * bl_sigma));
    } else {
        cut = 0;
    }

    log(log::debug) << channel_label(channel_) << "computed cut=" << cut;
}

channel::channel(module::module& module_)
    : number(-1),
      module(module_)
{
}

channel&
channel::operator=(const channel& c)
{
    number = c.number;
    module = c.module;
    std::copy(c.vars.begin(), c.vars.end(), std::back_inserter(vars));
    return *this;
}

void
channel::read_adc(hw::adc_word* buffer, size_t size)
{
    const hw::address addr =
        hw::memory::IO_BUFFER_ADDR + (number * (hw::max_adc_trace_length / 2));

    hw::memory::dsp dsp(module);
    hw::adc_trace_buffer adc_trace;

    dsp.read(addr, adc_trace, size / 2);

    for (size_t w = 0; w < size / 2; ++w) {
        buffer[w * 2] = hw::adc_word(adc_trace[w] & 0xffff);
        buffer[w * 2 + 1] = hw::adc_word((adc_trace[w] >> 16) & 0xffff);
    }
}

void
channel::update_fifo(param::value_type trace_delay)
{
    log(log::debug) << channel_label(*this)
                    << "fifo update: trace-delay=" << trace_delay;

    module::module& mod = module.get();

    const param::value_type slow_filter_range =
        1 << mod.read_var(param::module_var::SlowFilterRange, 0, false);
    const param::value_type fast_filter_range =
        1 << mod.read_var(param::module_var::FastFilterRange, 0, false);
    const param::value_type fifo_length =
        mod.read_var(param::module_var::FIFOLength, 0, false);
    const param::value_type peak_sep =
        mod.read_var(param::channel_var::PeakSep, number, 0, false);

    param::value_type trigger_delay;
    param::value_type paf_length;

    /*
     * Calulate the trigger delay and PAF length.
     */
    trigger_delay = (peak_sep - 1) * slow_filter_range;
    paf_length = (trigger_delay / fast_filter_range) + trigger_delay;

    /*
     * PAF Length must be larger than the FIFO Length
     */
    if (paf_length > fifo_length) {
        paf_length = fifo_length - 1;
        trigger_delay = (paf_length - trace_delay) * fast_filter_range;
    }

    mod.write_var(param::channel_var::TriggerDelay, trigger_delay, number);
    mod.write_var(param::channel_var::PAFlength, paf_length, number);
}

double
channel::trigger_risetime()
{
    module::module& mod = module.get();

    param::value_type fast_filter_range =
        1 << mod.read_var(param::module_var::FastFilterRange);
    double fast_length =
        mod.read_var(param::channel_var::FastLength, number);

    double result =
        (fast_length * fast_filter_range) / mod.fpga_clk_mhz;

    return result;
}

void
channel::trigger_risetime(double value)
{
    module::module& mod = module.get();

    param::value_type fast_filter_range =
        1 << mod.read_var(param::module_var::FastFilterRange, 0, false);
    param::value_type fast_gap =
        mod.read_var(param::channel_var::FastGap, number, 0, false);

    param::value_type fast_length =
        round((value * mod.fpga_clk_mhz) / fast_filter_range);

    if ((fast_length + fast_gap) > FASTFILTER_MAX_LEN) {
        fast_length = FASTFILTER_MAX_LEN - fast_gap;
    }
    if (fast_length < FASTLENGTH_MIN_LEN) {
        fast_length = FASTLENGTH_MIN_LEN;
        if ((fast_length + fast_gap) > FASTFILTER_MAX_LEN) {
            fast_gap = FASTFILTER_MAX_LEN - FASTLENGTH_MIN_LEN;
        }
    }

    mod.write_var(param::channel_var::FastLength, fast_length, number);
    mod.write_var(param::channel_var::FastGap, fast_gap, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::trigger_flattop()
{
    module::module& mod = module.get();

    param::value_type fast_filter_range =
        1 << mod.read_var(param::module_var::FastFilterRange);
    double fast_gap =
        mod.read_var(param::channel_var::FastGap, number);

    double result =
        (fast_gap * fast_filter_range) / mod.fpga_clk_mhz;

    return result;
}

void
channel::trigger_flattop(double value)
{
    module::module& mod = module.get();

    param::value_type fast_filter_range =
        1 << mod.read_var(param::module_var::FastFilterRange, 0, false);
    param::value_type fast_length =
        mod.read_var(param::channel_var::FastLength, number, 0, false);

    param::value_type fast_gap =
        round((value * mod.fpga_clk_mhz) / fast_filter_range);

    if ((fast_length + fast_gap) > FASTFILTER_MAX_LEN) {
        fast_gap = FASTFILTER_MAX_LEN - fast_length;
    }

    mod.write_var(param::channel_var::FastGap, fast_gap, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::trigger_threshold()
{
    module::module& mod = module.get();

    double fast_thresh =
        mod.read_var(param::channel_var::FastThresh, number);
    double fast_length =
        mod.read_var(param::channel_var::FastLength, number);

    double result =
        fast_thresh / (fast_length * double(mod.adc_clk_div));

    return result;
}

void
channel::trigger_threshold(double value)
{
    module::module& mod = module.get();

    double fast_length =
        mod.read_var(param::channel_var::FastLength, number);

    param::value_type fast_thresh =
        static_cast<param::value_type>(value * fast_length * mod.adc_clk_div);

    if (fast_thresh >= FAST_THRESHOLD_MAX) {
        double dbl_fast_thresh(fast_thresh);
        value =
            (double(FAST_THRESHOLD_MAX) / (dbl_fast_thresh - 0.5)) * dbl_fast_thresh;
        fast_thresh = static_cast<param::value_type>(value);
    }

    mod.write_var(param::channel_var::FastThresh, fast_thresh, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::energy_risetime()
{
    module::module& mod = module.get();

    param::value_type slow_filter_range =
        1 << mod.read_var(param::module_var::SlowFilterRange);
    double slow_length =
        mod.read_var(param::channel_var::SlowLength, number);

    double result =
        (slow_length * slow_filter_range) / mod.fpga_clk_mhz;

    return result;
}

double
channel::energy_flattop()
{
    module::module& mod = module.get();

    param::value_type slow_filter_range =
        1 << mod.read_var(param::module_var::SlowFilterRange);
    double slow_gap =
        mod.read_var(param::channel_var::SlowGap, number);

    double result =
        (slow_gap * slow_filter_range) / mod.fpga_clk_mhz;

    return result;
}

void
channel::energy_risetime_flattop(param::channel_param par, double value)
{
    module::module& mod = module.get();

    param::value_type fast_filter_range =
        1 << mod.read_var(param::module_var::FastFilterRange, 0, false);
    param::value_type slow_filter_range =
        1 << mod.read_var(param::module_var::SlowFilterRange, 0, false);
    param::value_type paf_length =
        mod.read_var(param::channel_var::PAFlength, number, 0, false);
    param::value_type trigger_delay =
        mod.read_var(param::channel_var::TriggerDelay, number, 0, false);

    param::value_type trace_delay =
        paf_length - (trigger_delay / fast_filter_range);

    param::value_type slow_length;
    param::value_type slow_gap;

    if (par == param::channel_param::energy_risetime) {
        slow_length = round((value * mod.adc_msps) / fast_filter_range);
        slow_gap = mod.read_var(param::channel_var::SlowGap, number, 0, false);
        if ((slow_length + slow_gap) > SLOWFILTER_MAX_LEN) {
            slow_length = SLOWFILTER_MAX_LEN - slow_gap;
        }
        if (slow_length < SLOWLENGTH_MIN_LEN) {
            slow_length = SLOWLENGTH_MIN_LEN;
            if ((slow_length + slow_gap) > SLOWFILTER_MAX_LEN) {
                slow_gap = SLOWFILTER_MAX_LEN - SLOWLENGTH_MIN_LEN;
            }
        }
    } else if (par == param::channel_param::energy_flattop) {
        slow_gap = round((value * mod.fpga_clk_mhz) / slow_filter_range);
        slow_length =
            mod.read_var(param::channel_var::SlowLength, number, 0, false);
        if ((slow_length + slow_gap) > SLOWFILTER_MAX_LEN) {
            slow_gap = SLOWFILTER_MAX_LEN - slow_length;
        }
        if (slow_gap < SLOWGAP_MIN_LEN) {
            slow_gap = SLOWGAP_MIN_LEN;
            if ((slow_length + slow_gap) > SLOWFILTER_MAX_LEN) {
                slow_length = SLOWFILTER_MAX_LEN - SLOWGAP_MIN_LEN;
            }
        }
    } else {
        throw error(mod.number, mod.slot, number,
                    error::code::channel_invalid_param,
                    "param not enery risetime or flattop");
    }

    mod.write_var(param::channel_var::SlowLength, slow_length, number);
    mod.write_var(param::channel_var::SlowGap, slow_gap, number);

    param::value_type peak_sep = slow_length + slow_gap;
    param::value_type peak_sample;

    switch (slow_filter_range) {
    case 1 << 1:
        peak_sample = peak_sep - 3;
        break;
    case 1 << 2:
        peak_sample = peak_sep - 2;
        break;
    case 1 << 3:
        peak_sample = peak_sep - 2;
        break;
    case 1 << 4:
        peak_sample = peak_sep - 1;
        break;
    case 1 << 5:
        peak_sample = peak_sep;
        break;
    case 1 << 6:
        peak_sample = peak_sep - 1;
        break;
    default:
        peak_sample = peak_sep - 2;
        break;
    }

    mod.write_var(param::channel_var::PeakSample, peak_sample, number);
    mod.write_var(param::channel_var::PeakSep, peak_sep, number);

    update_fifo(trace_delay);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::tau()
{
    module::module& mod = module.get();
    util::ieee_float preamp_tau =
        mod.read_var(param::channel_var::PreampTau, number);
    return preamp_tau;
}

void
channel::tau(double value)
{
    module::module& mod = module.get();

    param::value_type preamp_tau = util::ieee_float(value);

    mod.write_var(param::channel_var::PreampTau, preamp_tau, number);

    baseline bl(*this);
    bl.find_cut();
}

double
channel::trace_length()
{
    module::module& mod = module.get();

    double trace_len =
        mod.read_var(param::channel_var::TraceLength, number);
    param::value_type fast_filter_range =
        1 << mod.read_var(param::module_var::FastFilterRange);

    double result =
        trace_len / (double(mod.adc_msps) * fast_filter_range);

    return result;
}

void
channel::trace_length(double value)
{
    module::module& mod = module.get();

    param::value_type fast_filter_range =
        1 << mod.read_var(param::module_var::FastFilterRange);
    param::value_type fifo_length =
         mod.read_var(param::module_var::FIFOLength);

    param::value_type trace_length = (value * mod.adc_msps) / fast_filter_range;

    /*
     * Adjust the length to suit the FPGA requirements
     */
    switch (mod.adc_msps) {
    case 500:
        trace_length = (trace_length / 10) * 10;
        break;
    case 250:
    case 100:
        trace_length = (trace_length / 2) * 2;
        break;
    default:
        break;
    }

    if (trace_length > fifo_length) {
        trace_length = fifo_length;
    }

    mod.write_var(param::channel_var::TraceLength, trace_length, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::trace_delay()
{
    module::module& mod = module.get();

    double paf_length =
        mod.read_var(param::channel_var::PAFlength, number);
    double trigger_delay =
        mod.read_var(param::channel_var::TriggerDelay, number);
    param::value_type fast_filter_range =
        mod.read_var(param::module_var::FastFilterRange);
    param::value_type ffr_mask = 1 << fast_filter_range;

    double result =
        (paf_length - (trigger_delay / ffr_mask)) /
        (mod.fpga_clk_mhz * ffr_mask);

    return result;
}

void
channel::trace_delay(double value)
{
    module::module& mod = module.get();

    param::value_type fast_filter_range =
        1 << mod.read_var(param::module_var::FastFilterRange);
    param::value_type trace_length =
        mod.read_var(param::channel_var::TraceLength, number);

    param::value_type trace_delay = (value * mod.fpga_clk_mhz) / fast_filter_range;

    if (trace_delay > trace_length) {
        trace_delay = trace_length / 2;
    }

    if (trace_delay > TRACEDELAY_MAX) {
        trace_delay = TRACEDELAY_MAX;
    }

    update_fifo(trace_delay);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::voffset()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::OffsetDAC, number);

    double result =
        double(value) / 65536.0 * DAC_VOLTAGE_RANGE - DAC_VOLTAGE_RANGE / 2.0;

    return result;
}

void
channel::voffset(double value)
{
    module::module& mod = module.get();

    param::value_type offset_dac =
        (65536 * (((value / DAC_VOLTAGE_RANGE) + 1) / 2));

    /*
     * 16-bit DAC, range check.
     */
    if (offset_dac > 65535) {
        offset_dac = 65535;
    }

    mod.write_var(param::channel_var::OffsetDAC, offset_dac, number);

    hw::run::control(mod, hw::run::control_task::set_dacs);
}

double
channel::xdt()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::Xwait, number);

    double result = double(value) / DSP_CLOCK_MHZ;

    return result;
}

void
channel::xdt(double value)
{
    module::module& mod = module.get();

    param::value_type current_xwait =
        mod.read_var(param::channel_var::Xwait, number);

    param::value_type xwait = round(value * DSP_CLOCK_MHZ);

    double multiple;

    switch (mod.adc_msps) {
    case 100:
    case 500:
        /*
         * For 100 MSPS RevB/C/D or RevF, or 500 MSPS RevF, xwait should be
         * multiples of 6
         */
        multiple = 6;
        break;
    default:
        /*
         * For 250 RevF, xwait should be multiples of 8
         */
        multiple = 8;
        break;
    }

    if (xwait < multiple) {
        xwait = multiple;
    }
    if (xwait > current_xwait) {
        xwait = std::ceil(double(xwait) / multiple) * multiple;
    } else {
        xwait = std::floor(double(xwait) / multiple) * multiple;
    }

    mod.write_var(param::channel_var::Xwait, xwait, number);
}

double
channel::baseline_percent()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::BaselinePercent, number);

    double result = double(value);

    return result;
}

void
channel::baseline_percent(double value)
{
    module::module& mod = module.get();

    param::value_type bl_percent = value;

    if (bl_percent < 1) {
        bl_percent = 1;
    } else if (bl_percent > 99) {
        bl_percent = 99;
    }

    mod.write_var(param::channel_var::BaselinePercent, bl_percent, number);
}

double
channel::emin()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::EnergyLow, number);

    double result = double(value);

    return result;
}

void
channel::emin(double value)
{
    module::module& mod = module.get();

    mod.write_var(param::channel_var::EnergyLow, value, number);
}

double
channel::binfactor()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::Log2Ebin, number);

    double result = double(1ULL << 32) - double(value);

    return result;
}

void
channel::binfactor(double value)
{
    module::module& mod = module.get();

    param::value_type log2ebin = value;

    if (log2ebin < 1) {
        log2ebin = 1;
    } else if (log2ebin > 6) {
        log2ebin = 6;
    }

    log2ebin = (1ULL << 32) - uint64_t(log2ebin);

    mod.write_var(param::channel_var::Log2Ebin, log2ebin, number);
}

double
channel::baseline_average()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::Log2Bweight, number);

    double result;

    if (value == 0) {
        result = 0.0;
    } else {
        result = double(1ULL << 32) - double(value);
    }

    return result;
}

void
channel::baseline_average(double value)
{
    module::module& mod = module.get();

    param::value_type bl_avg = value;

    if (bl_avg > 16) {
        bl_avg = 16;
    }

    if (bl_avg > 0) {
        bl_avg = (1ULL << 32) - uint64_t(bl_avg);
    }

    mod.write_var(param::channel_var::Log2Bweight, bl_avg, number);
}

double
channel::csra()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::ChanCSRa, number);

    double result = double(value);

    return result;
}

void
channel::csra(double value)
{
    module::module& mod = module.get();

    param::value_type current_csra =
        mod.read_var(param::channel_var::ChanCSRa, number);

    param::value_type csra = value;

    mod.write_var(param::channel_var::ChanCSRa, csra, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
    hw::run::control(mod, hw::run::control_task::set_dacs);

    if ((csra & (1 << CCSRA_ENARELAY)) !=
        (current_csra & (1 << CCSRA_ENARELAY))) {
        baseline bl(*this);
        bl.find_cut();
    }
}

double
channel::csrb()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::ChanCSRb, number);

    double result = double(value);

    return result;
}

void
channel::csrb(double value)
{
    module::module& mod = module.get();

    mod.write_var(param::channel_var::ChanCSRb, value, number);
}

double
channel::bl_cut()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::BLcut, number);

    double result = double(value);

    return result;
}

void
channel::bl_cut(double value)
{
    module::module& mod = module.get();

    mod.write_var(param::channel_var::BLcut, value, number);
}

double
channel::integrator()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::Integrator, number);

    double result = double(value);

    return result;
}

void
channel::integrator(double value)
{
    module::module& mod = module.get();

    if (value > 7) {
        value = 7;
    }

    mod.write_var(param::channel_var::Integrator, value, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::fast_trig_backlen()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::FastTrigBackLen, number);

    double result = double(value) / mod.fpga_clk_mhz;

    return result;
}

void
channel::fast_trig_backlen(double value)
{
    module::module& mod = module.get();

    param::value_type fast_trig_blen = round(value * mod.fpga_clk_mhz);

    param::value_type fast_trig_blen_max;
    switch (mod.adc_msps) {
    case 100:
    case 500:
        fast_trig_blen_max = FASTTRIGBACKLEN_MIN_100MHZFIPCLK;
        break;
    default:
        fast_trig_blen_max = FASTTRIGBACKLEN_MIN_125MHZFIPCLK;
        break;
    }

    if (fast_trig_blen < fast_trig_blen_max) {
        fast_trig_blen = fast_trig_blen_max;
    } else if (fast_trig_blen > FASTTRIGBACKLEN_MAX) {
        fast_trig_blen = FASTTRIGBACKLEN_MAX;
    }

    mod.write_var(param::channel_var::FastTrigBackLen, fast_trig_blen, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::cfd_delay()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::CFDDelay, number);

    double result = double(value) / mod.fpga_clk_mhz;

    return result;
}

void
channel::cfd_delay(double value)
{
    module::module& mod = module.get();

    param::value_type cfddelay = round(value * mod.fpga_clk_mhz);

    if (cfddelay < CFDDELAY_MIN) {
        cfddelay = CFDDELAY_MIN;
    }
    if (cfddelay > CFDDELAY_MAX) {
        cfddelay = CFDDELAY_MAX;
    }

     mod.write_var(param::channel_var::CFDScale, cfddelay, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::cfd_scale()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::CFDDelay, number);

    double result = double(value);

    return result;
}

void
channel::cfd_scale(double value)
{
    module::module& mod = module.get();

    param::value_type cfdscale = value;

    if (cfdscale > CFDSCALE_MAX) {
        cfdscale = CFDSCALE_MAX;
    }

    mod.write_var(param::channel_var::CFDScale, cfdscale, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::cfd_thresh()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::CFDThresh, number);

    double result = double(value);

    return result;
}

void
channel::cfd_thresh(double value)
{
    module::module& mod = module.get();

    param::value_type cfdthresh = value;

    if (cfdthresh < CFDTHRESH_MIN || cfdthresh > CFDTHRESH_MAX) {
        std::ostringstream oss;
        oss << ": out of range CFDThresh: " << cfdthresh;
        throw error(mod.number, mod.slot, number,
                    error::code::channel_invalid_param,
                    oss.str());
    }

    mod.write_var(param::channel_var::CFDThresh, cfdthresh, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::qdc_len(param::channel_param par)
{
    module::module& mod = module.get();

    param::channel_var var;

    switch (par) {
    case param::channel_param::qdclen0:
        var = param::channel_var::QDCLen0;
        break;
    case param::channel_param::qdclen1:
        var = param::channel_var::QDCLen1;
        break;
    case param::channel_param::qdclen2:
        var = param::channel_var::QDCLen2;
        break;
    case param::channel_param::qdclen3:
        var = param::channel_var::QDCLen3;
        break;
    case param::channel_param::qdclen4:
        var = param::channel_var::QDCLen4;
        break;
    case param::channel_param::qdclen5:
        var = param::channel_var::QDCLen5;
        break;
    case param::channel_param::qdclen6:
        var = param::channel_var::QDCLen6;
        break;
    case param::channel_param::qdclen7:
        var = param::channel_var::QDCLen7;
        break;
    default:
        throw error(mod.number, mod.slot, number,
                    error::code::channel_invalid_param,
                    "invalid QDCLen param offset");
    }

    double divider = mod.adc_msps;
    if (mod.adc_msps == 500) {
        divider /= 5;
    }

    param::value_type value = mod.read_var(var, number);

    double result = double(value) / divider;

    return result;
}

void
channel::qdc_len(param::channel_param par, double value)
{
    module::module& mod = module.get();

    param::channel_var var;

    switch (par) {
    case param::channel_param::qdclen0:
        var = param::channel_var::QDCLen0;
        break;
    case param::channel_param::qdclen1:
        var = param::channel_var::QDCLen1;
        break;
    case param::channel_param::qdclen2:
        var = param::channel_var::QDCLen2;
        break;
    case param::channel_param::qdclen3:
        var = param::channel_var::QDCLen3;
        break;
    case param::channel_param::qdclen4:
        var = param::channel_var::QDCLen4;
        break;
    case param::channel_param::qdclen5:
        var = param::channel_var::QDCLen5;
        break;
    case param::channel_param::qdclen6:
        var = param::channel_var::QDCLen6;
        break;
    case param::channel_param::qdclen7:
        var = param::channel_var::QDCLen7;
        break;
    default:
        throw error(mod.number, mod.slot, number,
                    error::code::channel_invalid_param,
                    "invalid QDCLen param offset");
    }

    double divider = mod.adc_msps;
    if (mod.adc_msps == 500) {
        divider /= 5;
    }

    param::value_type qdclen = round(value * divider);

    if (qdclen < QDCLEN_MIN) {
        qdclen = QDCLEN_MIN;
    }
    if (qdclen > QDCLEN_MAX) {
        qdclen = QDCLEN_MAX;
    }

    mod.write_var(var, qdclen, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::ext_trig_stretch()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::ExtTrigStretch, number);

    double result = double(value) / mod.fpga_clk_mhz;

    return result;
}

void
channel::ext_trig_stretch(double value)
{
    module::module& mod = module.get();

    param::value_type exttrigstretch = round(value * mod.fpga_clk_mhz);

    if (exttrigstretch < EXTTRIGSTRETCH_MIN) {
        exttrigstretch = EXTTRIGSTRETCH_MIN;
    }
    if (exttrigstretch > EXTTRIGSTRETCH_MAX) {
        exttrigstretch = EXTTRIGSTRETCH_MAX;
    }

    mod.write_var(param::channel_var::ExtTrigStretch, exttrigstretch, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::veto_stretch()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::VetoStretch, number);

    double result = double(value) / mod.fpga_clk_mhz;

    return result;
}

void
channel::veto_stretch(double value)
{
    module::module& mod = module.get();

    param::value_type vetostretch = round(value * mod.fpga_clk_mhz);

    if (vetostretch < VETOSTRETCH_MIN) {
        vetostretch = VETOSTRETCH_MIN;
    }
    if (vetostretch > VETOSTRETCH_MAX) {
        vetostretch = VETOSTRETCH_MAX;
    }

    mod.write_var(param::channel_var::VetoStretch, vetostretch, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::multiplicity_mask_l()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::MultiplicityMaskL, number);

    double result = double(value);

    return result;
}

void
channel::multiplicity_mask_l(double value)
{
    module::module& mod = module.get();

    param::value_type multiplicitymaskl = value;

    mod.write_var(param::channel_var::MultiplicityMaskL, multiplicitymaskl, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::multiplicity_mask_h()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::MultiplicityMaskH, number);

    double result = double(value);

    return result;
}

void
channel::multiplicity_mask_h(double value)
{
    module::module& mod = module.get();

    param::value_type multiplicitymaskh = value;

    mod.write_var(param::channel_var::MultiplicityMaskH, multiplicitymaskh, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::extern_delay_len()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::ExternDelayLen, number);

    double result = double(value) / mod.fpga_clk_mhz;

    return result;
}

void
channel::extern_delay_len(double value)
{
    module::module& mod = module.get();

    param::value_type externdelaylen = round(value * mod.fpga_clk_mhz);
    param::value_type externdelaylen_max;

    switch (mod.revision) {
    case module::module::rev_B:
    case module::module::rev_C:
    case module::module::rev_D:
        externdelaylen_max = EXTDELAYLEN_MAX_REVBCD;
        break;
    case module::module::rev_F:
    case module::module::rev_H:
        externdelaylen_max = EXTDELAYLEN_MAX_REVF;
        break;
    default:
        throw error(mod.number, mod.slot, number,
                    error::code::not_supported,
                    "ExternDelayLen not supported on this revision");
    }

#if EXTDELAYLEN_MIN > 0
    if (externdelaylen < EXTDELAYLEN_MIN) {
        externdelaylen = EXTDELAYLEN_MIN;
    }
#endif
    if (externdelaylen > externdelaylen_max) {
        externdelaylen = externdelaylen_max;
    }

    mod.write_var(param::channel_var::ExternDelayLen, externdelaylen, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::ftrig_out_delay()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::FtrigoutDelay, number);

    double result = double(value) / mod.fpga_clk_mhz;

    return result;
}

void
channel::ftrig_out_delay(double value)
{
    module::module& mod = module.get();

    param::value_type ftrigoutdelay = round(value * mod.fpga_clk_mhz);
    param::value_type ftrigoutdelay_max;

    switch (mod.revision) {
    case module::module::rev_B:
    case module::module::rev_C:
    case module::module::rev_D:
        ftrigoutdelay_max = EXTDELAYLEN_MAX_REVBCD;
        break;
    case module::module::rev_F:
    case module::module::rev_H:
        ftrigoutdelay_max = EXTDELAYLEN_MAX_REVF;
        break;
    default:
        throw error(mod.number, mod.slot, number,
                    error::code::not_supported,
                    "FtrigoutDelay not supported on this revision");
    }

#if FASTTRIGBACKDELAY_MIN_MIN > 0
    if (ftrigoutdelay < FASTTRIGBACKDELAY_MIN) {
        ftrigoutdelay = FASTTRIGBACKDELAY_MIN;
    }
#endif
    if (ftrigoutdelay > ftrigoutdelay_max) {
        ftrigoutdelay = ftrigoutdelay_max;
    }

    mod.write_var(param::channel_var::FtrigoutDelay, ftrigoutdelay, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double
channel::chan_trig_stretch()
{
    module::module& mod = module.get();

    param::value_type value =
        mod.read_var(param::channel_var::ChanTrigStretch, number);

    double result = double(value) / mod.fpga_clk_mhz;

    return result;
}

void
channel::chan_trig_stretch(double value)
{
    module::module& mod = module.get();

    param::value_type chantrigstretch = round(value * mod.fpga_clk_mhz);

    if (chantrigstretch < CHANTRIGSTRETCH_MIN) {
        chantrigstretch = CHANTRIGSTRETCH_MIN;
    }
    if (chantrigstretch > CHANTRIGSTRETCH_MAX) {
        chantrigstretch = CHANTRIGSTRETCH_MAX;
    }

    mod.write_var(param::channel_var::ChanTrigStretch, chantrigstretch, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

}
}
}
