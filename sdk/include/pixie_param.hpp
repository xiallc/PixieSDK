#ifndef PIXIE16_PARAM_H
#define PIXIE16_PARAM_H

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

#include <stdexcept>
#include <string>
#include <vector>

namespace xia
{
namespace pixie
{
namespace param
{
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
        HRTP,
        U00,
        /*
         * Out
         */
        RealTimeA,
        RealTimeB,
        RunTimeA,
        RunTimeB,
        SynchDone,
        UserOut,
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
        GSLTtime,
        DSPerror,
        AOutBuffer,
        AECorr,
        LECorr,
        U20,
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
     * Parameter errors
     */
    class error
        : public std::runtime_error {
    public:
        explicit error(const std::string& what);
        explicit error(const char* what);
    };

    /*
     * Variable descriptor.
     */
    template<typename V>
    struct variable_desc {
        const V var;                /* Variable name (index) */
        const rwrowr mode;          /* Inout of the variable */
        const size_t size;          /* Number of DSP words it covers */
        enabledisable state;        /* Varirable's state */
        const std::string name;     /* Name of the variable */
        uint32_t address;           /* DSP memory address */
        variable_desc(const V var_,
                      enabledisable state_,
                      const rwrowr mode_,
                      const size_t size_,
                      const std::string name_)
            : var(var_),
              mode(mode_),
              size(size_),
              state(state_),
              name(name_),
              address(0) {
        }
    };

    /*
     * Variable descriptor sets for modules and channels
     */
    typedef variable_desc<module_var> module_var_desc;
    typedef std::vector<module_var_desc> module_var_descs;
    typedef variable_desc<channel_var> channel_var_desc;
    typedef std::vector<channel_var_desc> channel_var_descs;

    /*
     * Get a copy of the defaults.
     */
    const module_var_descs& get_module_var_descriptors();
    const channel_var_descs& get_channel_var_descriptors();

    /*
     * A parameter.
     */
    template<typename Vdesc> struct param
    {
        const Vdesc& var;             /* The variable descriptor */
        std::vector<uint32_t> value;  /* The value(s) */

        param(const Vdesc& var_)
            : var(var_) {
            value.resize(var.size);
        }
    };

    /*
     * Module parameters
     */
    typedef param<module_var> module_param;
    typedef std::vector<module_param> module_parameters;

    /*
     * Channel parameters
     */
    typedef param<channel_var> channel_param;
    typedef std::vector<channel_param> channel_parameters;
    typedef std::vector<channel_parameters> channels;

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
     * Load the variables from the DSP variable file into the
     * descriptors.
     */
    void load(const std::string& dspvarfile,
              module_var_descs& module_var_descriptors,
              channel_var_descs& channel_var_descriptors);

    /*
     * Copy the parameter values based on the filter.
     */
    void copy_parameters(const copy_filter& filter,
                         const channel_parameters& source,
                         channel_parameters& dest);

    /*
     * Copy the parameter values based on the filter mask.
     */
    void copy_parameters(const unsigned int filter_mask,
                         const channel_parameters& source,
                         channel_parameters& dest);
};
};
};

#endif  // PIXIE16_PARAM_H
