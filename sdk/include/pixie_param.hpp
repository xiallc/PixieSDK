#ifndef PIXIE_PARAM_H
#define PIXIE_PARAM_H

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

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <pixie_hw.hpp>

namespace xia
{
namespace pixie
{
namespace firmware
{
    struct firmware;
    typedef std::shared_ptr<firmware> firmware_ref;
}
namespace param
{
    /*
     * System parameters
     *
     * Do not force any values onto the enum.
     */
    enum struct system_param
    {
        number_modules,
        offline_analysis,
        pxi_slot_map,
        /*
         * Size marker
         */
        END
    };

    /*
     * Module parameters
     *
     * Do not force any values onto the enum.
     */
    enum struct module_param
    {
        module_number,
        module_csra,
        module_csrb,
        module_format,
        max_events,
        synch_wait,
        in_synch,
        slow_filter_range,
        fast_filter_range,
        fasttrigbackplaneena,
        crateid,
        slotid,
        modid,
        trigconfig0,
        trigconfig1,
        trigconfig2,
        trigconfig3,
        host_rt_preset,
        /*
         * Size marker
         */
        END
    };

    /*
     * Channel parameters
     *
     * Do not force any values onto the enum.
     */
    enum struct channel_param
    {
        trigger_risetime,
        trigger_flattop,
        trigger_threshold,
        energy_risetime,
        energy_flattop,
        tau,
        trace_length,
        trace_delay,
        voffset,
        xdt,
        baseline_percent,
        emin,
        binfactor,
        baseline_average,
        channel_csra,
        channel_csrb,
        blcut,
        integrator,
        fasttrigbacklen,
        cfddelay,
        cfdscale,
        cfdthresh,
        qdclen0,
        qdclen1,
        qdclen2,
        qdclen3,
        qdclen4,
        qdclen5,
        qdclen6,
        qdclen7,
        exttrigstretch,
        vetostretch,
        multiplicitymaskl,
        multiplicitymaskh,
        externdelaylen,
        ftrigoutdelay,
        chantrigstretch,
        /*
         * Size marker
         */
        END
    };

    /*
     * Module variables.
     *
     * Do not force any values onto the enum.
     */
    enum struct module_var
    {
        /*
         * In
         */
        ModNum,
        ModCSRA,
        ModCSRB,
        ModFormat,
        RunTask,
        ControlTask,
        MaxEvents,
        CoincPattern,
        CoincWait,
        SynchWait,
        InSynch,
        Resume,
        SlowFilterRange,
        FastFilterRange,
        ChanNum,
        HostIO,
        UserIn,
        FastTrigBackplaneEna,
        CrateID,
        SlotID,
        ModID,
        TrigConfig,
        U00,
        HostRunTimePreset,
        PowerUpInitDone,
        /*
         * Out
         */
        RealTimeA,
        RealTimeB,
        RunTimeA,
        RunTimeB,
        GSLTtime,
        DSPerror,
        SynchDone,
        UserOut,
        AOutBuffer,
        AECorr,
        LECorr,
        HardwareID,
        HardVariant,
        FIFOLength,
        DSPrelease,
        DSPbuild,
        NumEventsA,
        NumEventsB,
        BufHeadLen,
        EventHeadLen,
        ChanHeadLen,
        LOutBuffer,
        FippiID,
        FippiVariant,
        DSPVariant,
        U20,
        /*
         * Size marker
         */
        END
    };

    /*
     * Channel variables.
     *
     * Do not force any values onto the enum.
     */
    enum struct channel_var
    {
        /*
         * In
         */
        ChanCSRa,
        ChanCSRb,
        GainDAC,
        OffsetDAC,
        DigGain,
        SlowLength,
        SlowGap,
        FastLength,
        FastGap,
        PeakSample,
        PeakSep,
        CFDThresh,
        FastThresh,
        ThreshWidth,
        PAFlength,
        TriggerDelay,
        ResetDelay,
        ChanTrigStretch,
        TraceLength,
        Xwait,
        TrigOutLen,
        EnergyLow,
        Log2Ebin,
        MultiplicityMaskL,
        MultiplicityMaskH,
        PSAoffset,
        PSAlength,
        Integrator,
        BLcut,
        BaselinePercent,
        FtrigoutDelay,
        Log2Bweight,
        PreampTau,
        Xavg,
        FastTrigBackLen,
        CFDDelay,
        CFDScale,
        ExternDelayLen,
        ExtTrigStretch,
        VetoStretch,
        QDCLen0,
        QDCLen1,
        QDCLen2,
        QDCLen3,
        QDCLen4,
        QDCLen5,
        QDCLen6,
        QDCLen7,
        /*
         * Out
         */
        LiveTimeA,
        LiveTimeB,
        FastPeaksA,
        FastPeaksB,
        OverflowA,
        OverflowB,
        InSpecA,
        InSpecB,
        UnderflowA,
        UnderflowB,
        ChanEventsA,
        ChanEventsB,
        AutoTau,
        U30,
        /*
         * Size marker
         */
        END
    };

    /*
     * Variable input/output mode.
     */
    enum rwrowr {
        rw,
        ro,
        wr
    };

    /*
     * Variable enabled or disabled
     */
    enum enabledisable {
        enable,
        disable
    };

    /*
     * Variable addressing.
     */
    enum addressing {
        dsp_reg,
        fpga_reg,
        composite
    };

    /*
     * Value type.
     */
    typedef uint32_t value_type;

    /*
     * Values.
     */
    typedef std::vector<value_type> values;

    /*
     * Parameter descriptor.
     */
    template<typename P>
    struct parameter_desc {
        const P par;                /* Parameter (index) */
        const rwrowr mode;          /* In/out of the variable */
        const size_t size;          /* Number of DSP words it covers */
        enabledisable state;        /* Varirable's state */
        const std::string name;     /* Name of the variable */
        parameter_desc(const P par_,
                       enabledisable state_,
                       const rwrowr mode_,
                       const size_t size_,
                       const std::string name_)
            : par(par_),
              mode(mode_),
              size(size_),
              state(state_),
              name(name_) {
        }
    };

    /*
     * Variable descriptor.
     */
    template<typename V>
    struct variable_desc
        : public parameter_desc<V> {
        hw::address address;           /* DSP memory address */
        variable_desc(const V var_,
                      enabledisable state_,
                      const rwrowr mode_,
                      const size_t size_,
                      const std::string name_)
            : parameter_desc<V>(var_, state_, mode_, size_, name_),
              address(0) {
        }
    };

    /*
     * Parameter descriptor sets for the system, modules and channels
     */
    typedef parameter_desc<system_param> system_param_desc;
    typedef std::vector<system_param_desc> system_param_descs;
    typedef parameter_desc<module_param> module_param_desc;
    typedef std::vector<module_param_desc> module_param_descs;
    typedef parameter_desc<channel_param> channel_param_desc;
    typedef std::vector<channel_param_desc> channel_param_descs;

    /*
     * Variable descriptor sets for modules and channels
     */
    typedef variable_desc<module_var> module_var_desc;
    typedef std::vector<module_var_desc> module_var_descs;
    typedef variable_desc<channel_var> channel_var_desc;
    typedef std::vector<channel_var_desc> channel_var_descs;

    /*
     * A variable.
     */
    template<typename Vdesc> struct variable
    {
        const Vdesc& var;               /* The variable descriptor */
        std::vector<value_type> value;  /* The value(s) */

        variable(const Vdesc& var_)
            : var(var_) {
              value.resize(var.size);
        }
    };

    /*
     * Module variables
     */
    typedef variable<module_var_desc> module_variable;
    typedef std::vector<module_variable> module_variables;

    /*
     * Channel variables
     */
    typedef variable<channel_var_desc> channel_variable;
    typedef std::vector<channel_variable> channel_variables;
    typedef std::vector<channel_variables> channels_variables;

    /*
     * Copy filters. Only used on channels.
     */
    template<typename V> struct copy_filter_var {
        V var;
        uint32_t mask;

        copy_filter_var(V var_, uint32_t mask_ = UINT32_MAX)
            : var(var_),
              mask(mask_) {
        }
    };

    typedef std::vector<copy_filter_var<channel_var>> copy_filter;

    /*
     * Copy filter masks. The masks select the filters used in a copy.
     */
    const unsigned int energy_mask = 1 << 0;
    const unsigned int trigger_mask = 1 << 1;
    const unsigned int analog_signal_cond_mask = 1 << 2;
    const unsigned int histogram_control_mask = 1 << 3;
    const unsigned int decay_time_mask = 1 << 4;
    const unsigned int pulse_shape_analysis_mask = 1 << 5;
    const unsigned int baseline_control_mask = 1 << 6;
    const unsigned int channel_csra_mask = 1 << 7;
    const unsigned int cfd_trigger_mask = 1 << 8;
    const unsigned int trigger_stretch_len_mask = 1 << 9;
    const unsigned int fifo_delays_mask = 1 << 10;
    const unsigned int multiplicity_mask = 1 << 11;
    const unsigned int qdc_mask = 1 << 12;
    const unsigned int all_mask = (1 << 12) - 1;

    /*
     * Look up maps. A fast way to map a name to a parameter or vatiable.
     */
    typedef std::map<std::string, system_param> system_param_map;
    typedef std::map<std::string, module_param> module_param_map;
    typedef std::map<std::string, channel_param> channel_param_map;
    typedef std::map<std::string, module_var> module_var_map;
    typedef std::map<std::string, channel_var> channel_var_map;

    /*
     * Address map
     */
    struct address_map {
        typedef std::pair<size_t, hw::address> desc_address;
        typedef std::vector<desc_address> desc_addresses;
        struct range {
            hw::address start;
            hw::address end;
            size_t size;
            range();
            void set_size();
            void output(std::ostream& out) const;
        };

        range full;

        range module;
        range module_in;
        range module_out;

        range channels;
        range channels_in;
        range channels_out;

        size_t vars;
        size_t module_vars;
        size_t channel_vars;
        size_t vars_per_channel;

        address_map();

        void set(const size_t num_channels,
                 const module_var_descs& module_descs,
                 const channel_var_descs& channel_descs);
        hw::address channel_base(const size_t channel);

        void output(std::ostream& out) const;

    private:
        void check_channel_gap(const size_t num_channels,
                               const channel_var_descs& channel_descs,
                               const desc_addresses& addresses);

        template<typename V>
        void get(const V& vars,
                 desc_addresses& addresses,
                 const rwrowr mode);
        hw::address min(const desc_addresses& addresses);
        hw::address max(const desc_addresses& addresses);
    };

    /*
     * Get a descriptor from the descriptors.
     */
    template<typename D, typename V>
    const typename D::value_type&
    get_descriptor(const D& descs, const V var) {
        return descs[size_t(var)];
    }

    /*
     * Get a copy of the defaults.
     */
    const module_var_descs& get_module_var_descriptors();
    const channel_var_descs& get_channel_var_descriptors();

    /*
     * Get the maps
     */
    const system_param_map get_system_param_map();
    const module_param_map get_module_param_map();
    const channel_param_map get_channel_param_map();

    /*
     * Check is the parameter or variable is valid?
     */
    bool is_system_param(const std::string& label);
    bool is_module_param(const std::string& label);
    bool is_channel_param(const std::string& label);
    bool is_module_var(const std::string& label);
    bool is_channel_var(const std::string& label);

    /*
     * Look up parameters and variables.
     */
    system_param lookup_system_param(const std::string& label);
    module_param lookup_module_param(const std::string& label);
    channel_param lookup_channel_param(const std::string& label);
    module_var lookup_module_var(const std::string& label);
    channel_var lookup_channel_var(const std::string& label);

    /*
     * Module parammeter to module variable map.
     */
    module_var map_module_param(module_param par);

    /*
     * Load the variables from the DSP variable file into the
     * descriptors.
     */
    void load(const std::string& dspvarfile,
              module_var_descs& module_var_descriptors,
              channel_var_descs& channel_var_descriptors);
    void load(firmware::firmware_ref& dspvarfw,
              module_var_descs& module_var_descriptors,
              channel_var_descs& channel_var_descriptors);
    void load(std::istream& input,
              module_var_descs& module_var_descriptors,
              channel_var_descs& channel_var_descriptors);

    /*
     * Copy the variables based on the filter.
     */
    void copy_parameters(const copy_filter& filter,
                         const channel_variables& source,
                         channel_variables& dest);

    /*
     * Copy the variables based on the filter mask.
     */
    void copy_parameters(const unsigned int filter_mask,
                         const channel_variables& source,
                         channel_variables& dest);

};
};
};

std::ostream&
operator<<(std::ostream& out, const xia::pixie::param::address_map& config);

#endif  // PIXIE_PARAM_H
