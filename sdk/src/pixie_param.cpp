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
#include <cstring>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <sstream>

#include <pixie16app_defs.h>

#include <pixie16_param.hpp>

namespace xia
{
namespace pixie
{
namespace param
{
    static const module_var_descs module_var_descriptors_default = {
        { module_var::ModNum,               enable,  rw,  1, "ModNum" },
        { module_var::ModCSRA,              disable, ro,  1, "ModCSRA" },
        { module_var::ModCSRB,              enable,  rw,  1, "ModCSRB" },
        { module_var::ModFormat,            disable, rw,  1, "ModFormat" },
        { module_var::RunTask,              enable,  rw,  1, "RunTask" },
        { module_var::ControlTask,          enable,  rw,  1, "ControlTask" },
        { module_var::MaxEvents,            disable, rw,  1, "MaxEvents" },
        { module_var::CoincPattern,         disable, rw,  1, "CoincPattern" },
        { module_var::CoincWait,            disable, rw,  1, "CoincWait" },
        { module_var::SynchWait,            enable,  rw,  1, "SynchWait" },
        { module_var::InSynch,              enable,  rw,  1, "InSynch" },
        { module_var::Resume,               enable,  rw,  1, "Resume" },
        { module_var::SlowFilterRange,      enable,  rw,  1, "SlowFilterRange" },
        { module_var::FastFilterRange,      enable,  rw,  1, "FastFilterRange" },
        { module_var::ChanNum,              enable,  rw,  1, "ChanNum" },
        { module_var::HostIO,               enable,  rw, 16, "HostIO" },
        { module_var::UserIn,               enable,  rw, 16, "UserIn" },
        { module_var::FastTrigBackplaneEna, enable,  rw,  1, "FastTrigBackplaneEna" },
        { module_var::CrateID,              enable,  rw,  1, "CrateID" },
        { module_var::SlotID,               enable,  rw,  1, "SlotID" },
        { module_var::ModID,                disable, rw,  1, "ModID" },
        { module_var::TrigConfig,           enable,  rw,  4, "TrigConfig" },
        { module_var::HRTP,                 enable,  rw,  1, "HRTP" },
        { module_var::U00,                  disable, rw,  1, "U00" },
        { module_var::RealTimeA,            enable,  ro,  1, "RealTimeA" },
        { module_var::RealTimeB,            enable,  ro,  1, "RealTimeB" },
        { module_var::RunTimeA,             enable,  ro,  1, "RunTimeA" },
        { module_var::RunTimeB,             enable,  ro,  1, "RunTimeB" },
        { module_var::SynchDone,            enable,  ro,  1, "SynchDone" },
        { module_var::UserOut,              enable,  ro, 16, "UserOut" },
        { module_var::HardwareID,           enable,  ro,  1, "HardwareID" },
        { module_var::HardVariant,          enable,  ro,  1, "HardVariant" },
        { module_var::FIFOLength,           enable,  ro,  1, "FIFOLength" },
        { module_var::DSPrelease,           enable,  ro,  1, "DSPrelease" },
        { module_var::DSPbuild,             enable,  ro,  1, "DSPbuild" },
        { module_var::NumEventsA,           disable, ro,  1, "NumEventsA" },
        { module_var::NumEventsB,           disable, ro,  1, "NumEventsB" },
        { module_var::BufHeadLen,           disable, ro,  1, "BufHeadLen" },
        { module_var::EventHeadLen,         disable, ro,  1, "EventHeadLen" },
        { module_var::ChanHeadLen,          disable, ro,  1, "ChanHeadLen" },
        { module_var::LOutBuffer,           disable, ro,  1, "LOutBuffer" },
        { module_var::FippiID,              disable, ro,  1, "FippiID" },
        { module_var::FippiVariant,         disable, ro,  1, "FippiVariant" },
        { module_var::DSPVariant,           disable, ro,  1, "DSPVariant" }
    };

    static const channel_var_descs channel_var_descriptors_default = {
        { channel_var::ChanCSRa,          enable,  rw,  1, "ChanCSRa" },
        { channel_var::ChanCSRb,          disable, rw,  1, "ChanCSRb" },
        { channel_var::GainDAC,           disable, rw,  1, "GainDAC" },
        { channel_var::OffsetDAC,         enable,  rw,  1, "OffsetDAC" },
        { channel_var::DigGain,           disable, rw,  1, "DigGain" },
        { channel_var::SlowLength,        enable,  rw,  1, "SlowLength" },
        { channel_var::SlowGap,           enable,  rw,  1, "SlowGap" },
        { channel_var::FastLength,        enable,  rw,  1, "FastLength" },
        { channel_var::FastGap,           enable,  rw,  1, "FastGap" },
        { channel_var::PeakSample,        enable,  rw,  1, "PeakSample" },
        { channel_var::PeakSep,           enable,  rw,  1, "PeakSep" },
        { channel_var::CFDThresh,         enable,  rw,  1, "CFDThresh" },
        { channel_var::FastThresh,        enable,  rw,  1, "FastThresh" },
        { channel_var::ThreshWidth,       disable, rw,  1, "ThreshWidth" },
        { channel_var::PAFlength,         enable,  rw,  1, "PAFlength" },
        { channel_var::TriggerDelay,      enable,  rw,  1, "TriggerDelay" },
        { channel_var::ResetDelay,        disable, rw,  1, "ResetDelay" },
        { channel_var::ChanTrigStretch,   enable,  rw,  1, "ChanTrigStretch" },
        { channel_var::TraceLength,       enable,  rw,  1, "TraceLength" },
        { channel_var::Xwait,             enable,  rw,  1, "Xwait" },
        { channel_var::TrigOutLen,        disable, rw,  1, "TrigOutLen" },
        { channel_var::EnergyLow,         enable,  rw,  1, "EnergyLow" },
        { channel_var::Log2Ebin,          enable,  rw,  1, "Log2Ebin" },
        { channel_var::MultiplicityMaskL, enable,  rw,  1, "MultiplicityMaskL" },
        { channel_var::MultiplicityMaskH, enable,  rw,  1, "MultiplicityMaskH" },
        { channel_var::PSAoffset,         disable, rw,  1, "PSAoffset" },
        { channel_var::PSAlength,         disable, rw,  1, "PSAlength" },
        { channel_var::Integrator,        disable, rw,  1, "Integrator" },
        { channel_var::BLcut,             enable,  rw,  1, "BLcut" },
        { channel_var::BaselinePercent,   enable,  rw,  1, "BaselinePercent" },
        { channel_var::FtrigoutDelay,     enable,  rw,  1, "FtrigoutDelay" },
        { channel_var::Log2Bweight,       enable,  rw,  1, "Log2Bweight" },
        { channel_var::PreampTau,         enable,  rw,  1, "PreampTau" },
        { channel_var::FastTrigBackLen,   enable,  rw,  1, "FastTrigBackLen" },
        { channel_var::CFDDelay,          enable,  rw,  1, "CFDDelay" },
        { channel_var::CFDScale,          enable,  rw,  1, "CFDScale" },
        { channel_var::ExtTrigStretch,    enable,  rw,  1, "ExtTrigStretch" },
        { channel_var::VetoStretch,       enable,  rw,  1, "VetoStretch" },
        { channel_var::ExternDelayLen,    enable,  rw,  1, "ExternDelayLen" },
        { channel_var::QDCLen0,           enable,  rw,  1, "QDCLen0" },
        { channel_var::QDCLen1,           enable,  rw,  1, "QDCLen1" },
        { channel_var::QDCLen2,           enable,  rw,  1, "QDCLen2" },
        { channel_var::QDCLen3,           enable,  rw,  1, "QDCLen3" },
        { channel_var::QDCLen4,           enable,  rw,  1, "QDCLen4" },
        { channel_var::QDCLen5,           enable,  rw,  1, "QDCLen5" },
        { channel_var::QDCLen6,           enable,  rw,  1, "QDCLen6" },
        { channel_var::QDCLen7,           enable,  rw,  1, "QDCLen7" },
        { channel_var::GSLTtime,          enable,  ro,  1, "GSLTtime" },
        { channel_var::DSPerror,          enable,  ro,  1, "DSPerror" },
        { channel_var::AOutBuffer,        enable,  ro,  1, "AOutBuffer" },
        { channel_var::AECorr,            enable,  ro,  1, "AECorr" },
        { channel_var::LECorr,            enable,  ro,  1, "LECorr" },
        { channel_var::U20,               enable,  ro,  1, "U20" },
        { channel_var::LiveTimeA,         enable,  ro,  1, "LiveTimeA" },
        { channel_var::LiveTimeB,         enable,  ro,  1, "LiveTimeB" },
        { channel_var::FastPeaksA,        enable,  ro,  1, "FastPeaksA" },
        { channel_var::FastPeaksB,        enable,  ro,  1, "FastPeaksB" },
        { channel_var::OverflowA,         enable,  ro,  1, "OverflowA" },
        { channel_var::OverflowB,         enable,  ro,  1, "OverflowB" },
        { channel_var::InSpecA,           enable,  ro,  1, "InSpecA" },
        { channel_var::InSpecB,           enable,  ro,  1, "InSpecB" },
        { channel_var::UnderflowA,        enable,  ro,  1, "UnderflowA" },
        { channel_var::UnderflowB,        enable,  ro,  1, "UnderflowB" },
        { channel_var::ChanEventsA,       enable,  ro,  1, "ChanEventsA" },
        { channel_var::ChanEventsB,       enable,  ro,  1, "ChanEventsB" },
        { channel_var::AutoTau,           enable,  ro,  1, "AutoTau" },
        { channel_var::U30,               disable, ro,  1, "U30" }
    };

    const copy_filter energy_filter = {
        channel_var::SlowLength,
        channel_var::SlowGap,
        channel_var::PeakSep,
        channel_var::PeakSample,
        channel_var::TriggerDelay,
        channel_var::PAFlength,
        channel_var::BLcut,
        /* channel_var::CoincWait,      MODULE var?? */
        /* channel_var::SlowFilterRange MODULE var?? */
    };

    const copy_filter trigger_filter = {
        channel_var::FastLength,
        channel_var::FastGap,
        channel_var::FastThresh,
        /* channel_var::FastFilterRange MODULE var?? */
    };

    const copy_filter analog_signal_cond_filter = {
        { channel_var::ChanCSRa, CCSRA_POLARITY | CCSRA_ENARELAY },
        channel_var::OffsetDAC
    };

    const copy_filter histogram_control_filter = {
        channel_var::EnergyLow,
        channel_var::Log2Ebin
    };

    const copy_filter decay_time_filter = {
        channel_var::PreampTau
    };

    const copy_filter pulse_shape_analysis_filter = {
        channel_var::TraceLength,
        channel_var::TriggerDelay,
        channel_var::PAFlength
    };

    const copy_filter baseline_control_filter = {
        channel_var::BLcut,
        channel_var::BaselinePercent,
        channel_var::Log2Bweight
    };

    const copy_filter channel_csra_filter = {
        channel_var::ChanCSRa
    };

    const copy_filter cfd_trigger_filter = {
        channel_var::CFDDelay,
        channel_var::CFDScale,
        channel_var::CFDThresh
    };

    const copy_filter trigger_stretch_len_filter = {
        channel_var::ExtTrigStretch,
        channel_var::ChanTrigStretch,
        channel_var::VetoStretch,
        channel_var::FastTrigBackLen
    };

    const copy_filter fifo_delays_filter = {
        channel_var::ExternDelayLen,
        channel_var::FtrigoutDelay
    };

    const copy_filter multiplicity_filter = {
        channel_var::MultiplicityMaskL,
        channel_var::MultiplicityMaskH
    };

    const copy_filter qdc_filter = {
        channel_var::QDCLen0,
        channel_var::QDCLen1,
        channel_var::QDCLen2,
        channel_var::QDCLen3,
        channel_var::QDCLen4,
        channel_var::QDCLen5,
        channel_var::QDCLen6,
        channel_var::QDCLen7
    };

    error::error(const std::string& what)
        : runtime_error(what) {
    }

    error::error(const char* what)
        : runtime_error(what) {
    }

    const module_var_descs&
    get_module_var_descriptors() {
        return module_var_descriptors_default;
    }

    const channel_var_descs&
    get_channel_var_descriptors() {
        return channel_var_descriptors_default;
    }

    void
    load(const std::string& dspvarfile,
         module_var_descs& module_var_descriptors,
         channel_var_descs& channel_var_descriptors)
    {
        std::ifstream input(dspvarfile, std::ios::in | std::ios::binary);
        if (!input) {
            throw std::runtime_error(
                std::string("channel_var file open: ") + dspvarfile +
                ": " + std::strerror(errno)
            );
        }

        try {
            for (std::string line; std::getline(input, line); ) {
                if (!line.empty()) {
                    std::istringstream iss(line);
                    uint32_t address;
                    std::string name;
                    iss >> std::hex >> address >> name;
                    auto mvi = std::find_if(module_var_descriptors.begin(),
                                            module_var_descriptors.end(),
                                            [name](module_var_desc desc){
                                                return desc.name == name;
                                            });
                    if (mvi != module_var_descriptors.end()) {
                        (*mvi).address = address;
                    } else {
                        auto cvi = std::find_if(channel_var_descriptors.begin(),
                                                channel_var_descriptors.end(),
                                                [name](channel_var_desc desc){
                                                    return desc.name == name;
                                                });
                        if (cvi != channel_var_descriptors.end()) {
                            (*cvi).address = address;
                        } else {
                            std::string what = "DSP variable not found: ";
                            what += name;
                            throw error(what);
                        }
                    }
                }
            }
        } catch (...) {
            input.close();
            throw;
        }
        input.close();
    }

    void
    copy_parameters(const copy_filter& filter,
                    const channel_parameters& source,
                    channel_parameters& dest)
    {
        if (source.size() != dest.size())
            throw error("copy source and dest size do not match");

        for (auto f : filter) {
            int v = static_cast<int>(f.var);
            for (size_t i = 0; i < dest.size(); ++i) {
                dest[v].value[i] =
                    (dest[v].value[i] & ~f.mask) | (source[v].value[i] & f.mask);
            }
        }
    }

    void
    copy_parameters(const unsigned int filter_mask,
                    const channel_parameters& source,
                    channel_parameters& dest)
    {
        if ((filter_mask & energy_mask) != 0) {
            copy_parameters(energy_filter, source, dest);
        }
        if ((filter_mask & trigger_mask) != 0) {
            copy_parameters(trigger_filter, source, dest);
        }
        if ((filter_mask & analog_signal_cond_mask) != 0) {
            copy_parameters(analog_signal_cond_filter, source, dest);
        }
        if ((filter_mask & histogram_control_mask) != 0) {
            copy_parameters(histogram_control_filter, source, dest);
        }
        if ((filter_mask & decay_time_mask) != 0) {
            copy_parameters(decay_time_filter, source, dest);
        }
        if ((filter_mask & pulse_shape_analysis_mask) != 0) {
            copy_parameters(pulse_shape_analysis_filter, source, dest);
        }
        if ((filter_mask & baseline_control_mask) != 0) {
            copy_parameters(baseline_control_filter, source, dest);
        }
        if ((filter_mask & channel_csra_mask) != 0) {
            copy_parameters(channel_csra_filter, source, dest);
        }
        if ((filter_mask & cfd_trigger_mask) != 0) {
            copy_parameters(cfd_trigger_filter, source, dest);
        }
        if ((filter_mask & trigger_stretch_len_mask) != 0) {
            copy_parameters(trigger_stretch_len_filter, source, dest);
        }
        if ((filter_mask & fifo_delays_mask) != 0) {
            copy_parameters(fifo_delays_filter, source, dest);
        }
        if ((filter_mask & multiplicity_mask) != 0) {
            copy_parameters(multiplicity_filter, source, dest);
        }
        if ((filter_mask & qdc_mask) != 0) {
            copy_parameters(qdc_filter, source, dest);
        }
    }

};
};
};
