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

/** @file param.hpp
 * @brief Defines functions and data structures related to handling parameter sets
 */

#ifndef PIXIE_PARAM_H
#define PIXIE_PARAM_H

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <pixie/os_compat.hpp>

#include <pixie/error.hpp>
#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace firmware {
struct firmware;
typedef std::shared_ptr<firmware> firmware_ref;
}  // namespace firmware
/**
 * @brief Tools for working with parameters and variables in the SDK.
 */
namespace param {
/**
 * @brief Defines System parameters that we use in various locations.
 *
 * @note Order of entries in this enum must match with the definitions in
 * `system_params`.
 */
enum struct system_param {
    /**
     * @brief The number of modules that are online.
     */
    number_modules,
    /**
     * @brief **Not implemented.**
     */
    offline_analysis,
    /**
     * @brief **Not implemented.**
     */
    pxi_slot_map,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief Defines user facing module parameters.
 *
 * @remark All ranges are integer steps unless otherwise noted.
 * @note Order of entries in this enum must match with the definitions in
 * `module_params` and `module_param_var_map`.
 */
enum struct module_param {
    crateid,
    fast_filter_range,
    fasttrigbackplaneena,
    host_rt_preset,
    in_synch,
    max_events,
    module_csra,
    module_csrb,
    module_format,
    module_id,
    module_number,
    slotid,
    slow_filter_range,
    synch_wait,
    trigconfig0,
    trigconfig1,
    trigconfig2,
    trigconfig3,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief Define user-facing Channel parameters.
 *
 * Custom interfaces can set and read these variables in whatever form is most
 * suitable. For all parameters, also see the corresponding entry in DSP
 * parameter section for details.
 *
 * In most cases, the PixieSDK adjusts the input parameter to the closest value
 * that corresponds to a valid DSP parameter value, for example entering an
 * ENERGY_FLATTOP of 22ns will be adjusted to 20ns.
 *
 * For compatibility, the SDK uses a lookup table to match the enumeration with
 * the original string values. For example, `CHANNEL_CSRA` maps to the
 * enumerator `channel_csra`.
 *
 * @note Order of entries in this enum must match with the definitions in
 * `channel_params`.
 */
enum struct channel_param {
    baseline_average,
    baseline_percent,
    binfactor,
    blcut,
    cfddelay,
    cfdscale,
    cfdthresh,
    channel_csra,
    channel_csrb,
    chantrigstretch,
    emin,
    energy_flattop,
    energy_risetime,
    externdelaylen,
    exttrigstretch,
    fasttrigbacklen,
    ftrigoutdelay,
    integrator,
    multiplicitymaskl,
    multiplicitymaskh,
    qdclen0,
    qdclen1,
    qdclen2,
    qdclen3,
    qdclen4,
    qdclen5,
    qdclen6,
    qdclen7,
    reset_delay,
    tau,
    trace_delay,
    trace_length,
    trigger_flattop,
    trigger_risetime,
    trigger_threshold,
    vetostretch,
    voffset,
    xdt,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief Module variables that are defined within the DSP VAR file
 *
 * @note Order of entries in this enum must match with the definitions in
 * `module_vars` and `module_var_descriptors_default`.
 */
enum struct module_var {
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    AECorr,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    AOutBuffer,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    BufHeadLen,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    ChanHeadLen,
    /**
     * @brief [INPUT] The chosen channel number of a Pixie module. Mainly used
     * by the host to set the designated channel.
     */
    ChanNum,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    CoincPattern,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    CoincWait,
    /**
     * @brief [INPUT] Use this variable to select a control task. The control task will
     * be launched when a run start command is issued with RUNTASK=0. <br>
     * See @ref xia::pixie::hw::run::control_task for more information.
     */
    ControlTask,
    CrateID,
    /**
     * @brief [OUTPUT] DSP software build number.
     */
    DSPbuild,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    DSPerror,
    /**
     * @brief [OUTPUT] DSP software release number.
     */
    DSPrelease,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    DSPVariant,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    EventHeadLen,
    FastFilterRange,
    FastTrigBackplaneEna,
    /**
     * @brief [OUTPUT] Length of the trace buffer in the signal processing FPGAs.
     */
    FIFOLength,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    FippiID,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    FippiVariant,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    GSLTtime,
    /**
     * @brief [OUTPUT] Variant of the hardware.
     */
    HardVariant,
    /**
     * @brief [OUTPUT] ID of the hardware version.
     */
    HardwareID,
    /**
     * @brief [INPUT] A 16 word data block that is used to specify command
     * options.
     */
    HostIO,
    HostRunTimePreset,
    InSynch,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    LECorr,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    LOutBuffer,
    MaxEvents,
    ModCSRA,
    ModCSRB,
    ModFormat,
    ModID,
    ModNum,
    /**
     * @brief [OUTPUT] **Not implemented.
     */
    NumEventsA,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    NumEventsB,
    /**
     * @brief [OUTPUT] Indicates whether routine to initialize DSP on RESET
     * is done (1) or not (0).
     */
    PowerUpInitDone,
    /**
     * @brief [OUTPUT] Upper 32-bits of the time spent on data acquisition processes.
     *
     * The clock is zeroed on power up, and also in response to a synchronous
     * data acquisition start when InSynch was set to 0 prior to the run start.
     */
    RealTimeA,
    /**
     * @brief [OUTPUT] Lower 32-bits of the time spent on data acquisition processes.
     *
     * The clock is zeroed on power up, and also in response to a synchronous
     * data acquisition start when InSynch was set to 0 prior to the run start.
     *
     * RealTime = (RealTimeA * 2<sup>32</sup> + RealTimeB) * 10ns
     */
    RealTimeB,
    /**
     * @brief [INPUT] Set this variable to 1 to resume a data run; otherwise,
     * set it to 0.
     */
    Resume,
    /**
     * @brief [INPUT] What kind of run to start in response to a run start
     * request. <br>
     * See @ref xia::pixie::hw::run::run_task for more information.
     */
    RunTask,
    /**
     * @brief [OUTPUT] Upper 32-bits of the time spent acquiring data.
     *
     * This time counter is active only while a data acquisition run is in
     * progress and thus counts the elapsed run time.
     */
    RunTimeA,
    /**
     * @brief [OUTPUT] Lower 32-bits of the time spent acquiring data.
     *
     * This time counter is active only while a data acquisition run is in
     * progress and thus counts the elapsed run time.
     *
     * RunTime = (RunTimeA * 2<sup>32</sup> + RunTimeB) * 10ns
     */
    RunTimeB,
    SlotID,
    SlowFilterRange,
    /**
     * @brief [OUTPUT] This variable is used to indicate whether the
     * synchronization interrupt has successfully occurred in the DSP when a
     * run start request was issued with SYNCHWAIT=1. If the DSP is stuck in
     * an infinite loop caused by a malfunctioning Busy-Synch loop, indicated
     * by the value of SynchDone being always 0, the module should be rebooted
     * and then the reason which caused the malfunctioning Busy-Synch loop
     * should be investigated.
     */
    SynchDone,
    SynchWait,
    TrigConfig,
    /**
     * @brief [INPUT] **Not implemented. Reserved.**
     */
    U00,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    U20,
    /**
     * @brief [INPUT] A block of 16 input variables used by user-written DSP
     * code.
     */
    UserIn,
    /**
     * @brief [OUTPUT] 16 words of user output data, which may be used by the
     * user written DSP code.
     */
    UserOut,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief Defines Channel variables that are defined within the DSP VAR file.
 *
 * @note Order of entries in this enum must match with the definitions in
 * `channel_vars` and `channel_var_descriptors_default`.
 */
enum struct channel_var {
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    AutoTau,
    BaselinePercent,
    BLcut,
    CFDDelay,
    CFDScale,
    CFDThresh,
    ChanCSRa,
    /**
     * @brief [INPUT] **Not implemented. Reserved.**
     */
    ChanCSRb,
    /**
     * @brief [OUTPUT] The upper 32-bits of the number of events processed by
     * the DSP.
     */
    ChanEventsA,
    /**
     * @brief [OUTPUT] The lower 32-bits of the number of events processed by
     * the DSP. Calculated with
     *
     * TotalEvents = ChanEventsA * 2<sup>32</sup> + ChanEventsB
     */
    ChanEventsB,
    ChanTrigStretch,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    DigGain,
    EnergyLow,
    ExternDelayLen,
    ExtTrigStretch,
    FastGap,
    FastLength,
    /**
     * @brief [OUTPUT] The upper 32-bits of the number of fast filter triggers.
     */
    FastPeaksA,
    /**
     * @brief [OUTPUT] The lower 32-bits of the number of fast filter triggers.
     * Calculated with
     *
     * NumEvents = FastPeaksA * 2<sup>32</sup> + FastPeaksB
     */
    FastPeaksB,
    FastThresh,
    FastTrigBackLen,
    FtrigoutDelay,
    /**
     * @brief [INPUT] **Not implemented. Reserved.**
     */
    GainDAC,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    InSpecA,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    InSpecB,
    Integrator,
    /**
     * @brief [OUTPUT] Upper 32-bits of the channel's live time as measured by
     * the trigger/filter FPGA of that channel. It excludes times during which
     * the input signal is out of the ADC’s voltage range, or when the run
     * was stopped.
     */
    LiveTimeA,
    /**
     * @brief [OUTPUT] Lower 32-bits of the channel's live time as measured by
     * the trigger/filter FPGA of that channel. It excludes times during which
     * the input signal is out of the ADC’s voltage range, or when the run
     * was stopped.
     * | ADC Freq. (MHz) | Live Time calculation |
     * |---|---|
     * | 100/500 | (LiveTimeA * 2<sup>FastFilterRange</sup> + LiveTimeB) * 10 ns |
     * | 250 | (LiveTimeA * 2<sup>FastFilterRange</sup> + LiveTimeB) * 8 ns |
     */
    LiveTimeB,
    Log2Bweight,
    Log2Ebin,
    MultiplicityMaskH,
    MultiplicityMaskL,
    OffsetDAC,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    OverflowA,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    OverflowB,
    /**
     * @brief [INPUT] Legacy parameter used by obsolete designs.
     * @note Do not set manually, it is computed by the DSP and/or the SDK
     * from the filter settings.
     */
    PAFlength,
    /**
     * @brief [OUTPUT] This variable determines at what time the value from the
     * energy filter will be sampled. Its value is set according to the value
     * of SlowFilterRange. If the sampling point is chosen poorly, the
     * resulting spectrum will show energy resolutions of 10% and wider rather
     * than the expected fraction of a percent. For some parameter
     * combinations PeakSample needs to be varied by one or two units in
     * either direction, due to the pipelined architecture of the
     * trigger/filter FPGA.
     *
     * | SlowFilterRange | PeakSample |
     * |---|---|
     * | 1 | SlowLength + Slow Gap - 3 |
     * | 2 | SlowLength + Slow Gap - 2 |
     * | 3 | SlowLength + Slow Gap - 2 |
     * | 4 | SlowLength + Slow Gap - 1 |
     * | 5 | SlowLength + Slow Gap |
     * | 6 | SlowLength + Slow Gap + 1 |
     *
     * @note Do not set manually, it is computed by the DSP and/or the SDK
     * from the filter settings.
     */
    PeakSample,
    /**
     * @brief [INPUT] This value governs the minimum time separation between
     * two pulses. Two pulses that arrive within a time span shorter than
     * determined by PeakSep will be rejected as piled up. The SDK sets this
     * value to be SlowLength + SlowGap.
     *
     * @note Do not set manually, it is computed by the DSP and/or the SDK
     * from the filter settings.
     */
    PeakSep,
    PreampTau,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    PSAlength,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    PSAoffset,
    QDCLen0,
    QDCLen1,
    QDCLen2,
    QDCLen3,
    QDCLen4,
    QDCLen5,
    QDCLen6,
    QDCLen7,
    ResetDelay,
    SlowGap,
    SlowLength,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    ThreshWidth,
    TraceLength,
    /**
     * @brief [INPUT] Legacy parameter used by obsolete designs.
     * @note Do not set manually, it is computed by the DSP and/or the SDK
     * from the filter settings.     */
    TriggerDelay,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    TrigOutLen,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    U30,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    UnderflowA,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    UnderflowB,
    VetoStretch,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    Xavg,
    Xwait,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief defines the Variable's input/output mode.
 */
enum rwrowr { rw, ro, wr };

/**
 * @brief Variable enabled or disabled
 */
enum enabledisable { enable, disable };

/**
 * @brief Variable addressing defines what chip this variable is associated with.
 */
enum addressing { dsp_reg, fpga_reg, composite };

/*
 * Value type.
 */
typedef uint32_t value_type;

/*
 * Values.
 */
typedef std::vector<value_type> values;

/**
 * @brief A data structure describing information about Parameters.
 */
template<typename P>
struct parameter_desc {
    const P par; /*!< Parameter (index) */
    const rwrowr mode; /*!< In/out of the variable */
    const size_t size; /*!< Number of DSP words it covers */
    enabledisable state; /*!< Variable's state */
    const std::string name; /*!< Name of the variable */
    parameter_desc(const P par_, enabledisable state_, const rwrowr mode_, const size_t size_,
                   const std::string name_)
        : par(par_), mode(mode_), size(size_), state(state_), name(name_) {}
    bool writeable() const {
        return state == enable && mode != ro;
    }
};

/**
 * @brief A data structure describing information about a Variable.
 */
template<typename V>
struct variable_desc : public parameter_desc<V> {
    hw::address address; /*!< DSP memory address */
    variable_desc(const V var_, enabledisable state_, const rwrowr mode_, const size_t size_,
                  const std::string name_)
        : parameter_desc<V>(var_, state_, mode_, size_, name_), address(0) {}
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

/**
 * @brief A variable is an object that combines descriptors with values.
 */
template<typename Vdesc>
struct variable {
    /**
     * @brief Structure to describe the data associated with a descriptor.
     */
    struct data {
        bool dirty; /*!< Written to hardware? */
        value_type value;
        data() : dirty(false), value(0) {}
    };
    const Vdesc& var; /*!< The variable descriptor */
    std::vector<data> value; /*!< The value(s) */

    variable(const Vdesc& var_) : var(var_) {
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

/**
 * @brief Copies filter variables from one channel to another. Only used with channel objects.
 */
template<typename V>
struct copy_filter_var {
    V var;
    uint32_t mask;

    copy_filter_var(V var_, uint32_t mask_ = UINT32_MAX) : var(var_), mask(mask_) {}
};

typedef std::vector<copy_filter_var<module_var>> copy_filter_module;
typedef std::vector<copy_filter_var<channel_var>> copy_filter_channel;

/*
 * Copy filter masks. The masks select the filters used in a copy.
 */
const unsigned int energy_mask = 1 << 0;
const unsigned int trigger_mask = 1 << 0;
const unsigned int analog_signal_cond_mask = 1 << 1;
const unsigned int histogram_control_mask = 1 << 2;
const unsigned int decay_time_mask = 1 << 3;
const unsigned int pulse_shape_analysis_mask = 1 << 4;
const unsigned int baseline_control_mask = 1 << 5;
const unsigned int channel_csra_mask = 1 << 7;
const unsigned int cfd_trigger_mask = 1 << 8;
const unsigned int trigger_stretch_len_mask = 1 << 9;
const unsigned int fifo_delays_mask = 1 << 10;
const unsigned int multiplicity_mask = 1 << 11;
const unsigned int qdc_mask = 1 << 12;
const unsigned int all_mask = (1 << 12) - 1;

/*
 * Look up maps. A fast way to map a name to a parameter or variable.
 */
typedef std::map<std::string, system_param> system_param_map;
typedef std::map<std::string, module_param> module_param_map;
typedef std::map<std::string, channel_param> channel_param_map;
typedef std::map<std::string, module_var> module_var_map;
typedef std::map<std::string, channel_var> channel_var_map;

/**
 * @brief Defines an Address map that can be used to parse binary data blobs.
 *
 * This structure is typically used to parse Statistics or configuration data from the DSP.
 */
struct address_map {
    typedef std::pair<size_t, hw::address> desc_address;
    typedef std::vector<desc_address> desc_addresses;
    /**
     * @brief Data structure for working with address ranges
     */
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

    void set(const size_t num_channels, const module_var_descs& module_descs,
             const channel_var_descs& channel_descs);
    hw::address channel_base(const size_t channel);

    void output(std::ostream& out, bool lines = false) const;

private:
    void check_channel_gap(const size_t max_channels, const channel_var_descs& channel_descs,
                           const desc_addresses& addresses);

    template<typename V>
    void get(const V& vars, desc_addresses& addresses, const rwrowr mode);
    hw::address min(const desc_addresses& addresses);
    hw::address max(const desc_addresses& addresses);
};

/**
 * @brief Get a descriptor from the descriptors by its variable name.
 */
template<typename D, typename V>
const typename D::value_type& get_descriptor(const D& descs, const V var) {
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
 * Look up parameters and variables descriptors.
 */
const module_var_desc& lookup_module_descriptor(
    const std::string& label, const module_var_descs& descs);
const channel_var_desc& lookup_channel_descriptor(
    const std::string& label, const channel_var_descs& descs);

/**
 * @brief Maps a Module parameter to module variable.
 */
module_var map_module_param(module_param par);

/*
 * Load the variables from the DSP variable file into the
 * descriptors.
 */
void load(const std::string& dspvarfile, module_var_descs& module_var_descriptors,
          channel_var_descs& channel_var_descriptors);
void load(firmware::firmware_ref& dspvarfw, module_var_descs& module_var_descriptors,
          channel_var_descs& channel_var_descriptors);
void load(std::istream& input, module_var_descs& module_var_descriptors,
          channel_var_descs& channel_var_descriptors);

/**
 * @brief Copy the module variables based on the filter.
 */
void copy_parameters(const copy_filter_module& filter, const module_variables& source,
                     module_variables& dest);

/**
 * @brief Copy the channel variables based on the filter.
 */
void copy_parameters(const copy_filter_channel& filter, const channel_variables& source,
                     channel_variables& dest);

/**
 * @brief Copy the module variables based on the filter mask.
 */
void copy_parameters(const unsigned int filter_mask, const module_variables& source,
                     module_variables& dest);

/**
 * @brief Copy the module and/or channel variables based on the filter mask.
 */
void copy_parameters(const unsigned int filter_mask, const channel_variables& chan_source,
                     channel_variables& chan_dest, const module_variables& mod_source,
                     module_variables& mod_dest);

/**
 * @brief Return the rwrowr label.
 */
std::string label(rwrowr mode);

/**
 * @brief Return the enabledisable label.
 */
std::string label(enabledisable state);

};  // namespace param
};  // namespace pixie
};  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::param::address_map& config);

#endif  // PIXIE_PARAM_H
