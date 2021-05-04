#ifndef PIXIE_CHANNEL_H
#define PIXIE_CHANNEL_H

/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2020, XIA LLC
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

#include <functional>
#include <vector>

#include <pixie_error.hpp>
#include <pixie_hw.hpp>
#include <pixie_param.hpp>

namespace xia
{
namespace pixie
{
namespace module
{
class module;
}
namespace channel
{
/*
 * Module errors
 */
struct error
    : public pixie::error::error {
    typedef pixie::error::code code;
    explicit error(const int num, const int slot, const int channel,
                   const code type,
                   const std::ostringstream& what);
    explicit error(const int num, const int slot, const int channel,
                   const code type,
                   const std::string& what);
    explicit error(const int num, const int slot, const int channel,
                   const code type,
                   const char* what);
    virtual void output(std::ostream& out);
private:
    std::string make_what(const int num,
                          const int slot,
                          const int channel,
                          const char* what);
};

struct channel;

/*
 * Range, a list of channels for an operation.
 */
typedef std::vector<size_t> range;

/*
 * Set the range of all channels to [first, first + size).
 */
void range_set(range& range_, size_t first = 0);

/*
 * Baselines
 */
struct baseline {
    static const size_t max_num = 3640;

    typedef std::pair<double, double> value;
    typedef std::array<value, max_num> values;
    typedef std::vector<values> channels_values;

    module::module& module;
    range& channels;
    channels_values bl_values;

    /*
     * last found cut.
     */
    param::values cuts;

    baseline(module::module& module, range& channels);

    void find_cut(size_t num = max_num);
    void compute_cut(size_t num);
    void get(channels_values& chan_values, bool run = true);
    double time(hw::word time_word0, hw::word time_word1);
};

/*
 * Channel
 */
struct channel {

    /*
     * Number of this channel.
     */
    size_t number;

    /*
     * Configuration
     */
    hw::config config;

    /*
     * The module this channel is part of.
     */
    std::reference_wrapper<module::module> module;

    /*
     * Channel variables.
     */
    param::channel_variables vars;

    /*
     * A channel nust be part of a module.
     */
    channel(module::module& module);
    ~channel() = default;

    channel& operator=(const channel& c);

    /*
     * ADC trace
     */
    void read_adc(hw::adc_word* buffer, size_t size);

    /*
     * Histogram
     */
    void read_histogram(hw::word_ptr values, const size_t size);
    void read_histogram(hw::words& values);

    /*
     * Update fifo settings.
     */
    void update_fifo(param::value_type trace_delay);

    /*
     * Channel parameter handlers.
     */
    double trigger_risetime();
    void trigger_risetime(double value);

    double trigger_flattop();
    void trigger_flattop(double value);

    double trigger_threshold();
    void trigger_threshold(double value);

    double energy_risetime();
    double energy_flattop();
    void energy_risetime_flattop(param::channel_param par, double value);

    double tau();
    void tau(double value);

    double trace_length();
    void trace_length(double value);

    double trace_delay();
    void trace_delay(double value);

    double voffset();
    void voffset(double value);

    double xdt();
    void xdt(double value);

    double baseline_percent();
    void baseline_percent(double value);

    double emin();
    void emin(double value);

    double binfactor();
    void binfactor(double value);

    double baseline_average();
    void baseline_average(double value);

    double csra();
    void csra(double value);

    double csrb();
    void csrb(double value);

    double bl_cut();
    void bl_cut(double value);

    double integrator();
    void integrator(double value);

    double fast_trig_backlen();
    void fast_trig_backlen(double value);

    double cfd_delay();
    void cfd_delay(double value);

    double cfd_scale();
    void cfd_scale(double value);

    double cfd_thresh();
    void cfd_thresh(double value);

    double qdc_len(param::channel_param par);
    void qdc_len(param::channel_param par, double value);

    double ext_trig_stretch();
    void ext_trig_stretch(double value);

    double veto_stretch();
    void veto_stretch(double value);

    double multiplicity_mask_l();
    void multiplicity_mask_l(double value);

    double multiplicity_mask_h();
    void multiplicity_mask_h(double value);

    double extern_delay_len();
    void extern_delay_len(double value);

    double ftrig_out_delay();
    void ftrig_out_delay(double value);

    double chan_trig_stretch();
    void chan_trig_stretch(double value);
};

/*
 * Channels
 */
typedef std::vector<channel> channels;
}
}
}

#endif  // PIXIE_CHANNEL_H
