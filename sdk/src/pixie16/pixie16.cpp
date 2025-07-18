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

/** @file pixie16.cpp
 * @brief C wrappers for the C++ API that expose the same signature as the legacy code
 */

#include <bitset>
#include <cstring>
#include <regex>

#include <sys/stat.h>

#include <pixie16/pixie16.h>

#include <pixie/config.hpp>
#include <pixie/error.hpp>
#include <pixie/fw.hpp>
#include <pixie/log.hpp>
#include <pixie/os_compat.h>
#include <pixie/stats.hpp>
#include <pixie/utils/io.hpp>
#include <pixie/utils/numerics.hpp>
#include <pixie/utils/path.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/crate-view.hpp>
#include <pixie/pixie16/legacy.hpp>
#include <pixie/pixie16/run.hpp>
#include <pixie/pixie16/sim.hpp>

/*
 * Local types for convenience.
 */
using xia_error = xia::pixie::error::error;

/*
 * Boot patterns
 */
#define BOOTPATTERN_COMFPGA_BIT 0
#define BOOTPATTERN_SPFPGA_BIT 2
#define BOOTPATTERN_DSPCODE_BIT 3
#define BOOTPATTERN_DSPPAR_BIT 4
#define BOOTPATTERN_PROGFIPPI_BIT 5
#define BOOTPATTERN_SETDACS_BIT 6

/*
 * Legacy API stats data exported to users as opaque data
 */
struct stats_legacy {
    static const unsigned int mark_1 = 0x20010928;
    static const unsigned int mark_2 = 0x19650829;

    unsigned int marker_1;
    size_t num_channels;
    xia::pixie::stats::module module;
    xia::pixie::stats::channel channels[xia::pixie::hw::max_channels];
    unsigned int marker_2;

    explicit stats_legacy(const xia::pixie::hw::configs& configs);

    void validate() const;
};

using stats_legacy_ptr = stats_legacy*;

/*
 * API Crate
 *
 * The API crate multiplexes the type of crate the user has
 * selected. We support access to the crate, the hardware, and an
 * offline mode via simulation for users who wish to run tests
 * configurations.
 *
 * Note, the simulated crate is an on going efforts and the level of
 *       simulation support it provides varies between releases.
 */
struct api_crate {
    using module_defs = std::vector<std::string>;

    /*
     * Physical crates, uses slots.
     */
    xia::pixie::crate::crate crate_hw;
    xia::pixie::sim::crate crate_sim;

    /*
     * Module crate handles logical mapping unique the to legacy API.
     */
    xia::pixie::crate::view::module modules;

    std::atomic<bool> run_check_override;

    api_crate();

    void set_simulation();
    unsigned short num_simulation_modules();

    xia::pixie::crate::crate* operator->();
    operator xia::pixie::crate::crate&();
    operator xia::pixie::crate::view::module&();

    /*
     * Simulation module definitions. Currently this is hard coded.
     */
    static const module_defs sim_module_defs;

    /*
     * System control buffer stream and reader.
     */
    xia::util::io::bufferstream sysctl;
    xia::util::io::bufferstream::reader sysctl_reader;
};

api_crate::api_crate()
    : modules(crate_hw), run_check_override(false), sysctl_reader(sysctl) {
}

void api_crate::set_simulation() {
    modules.set(crate_sim);
    for (auto& mod : sim_module_defs) {
        xia::pixie::sim::add_module_def(mod, ' ');
    }
}

unsigned short api_crate::num_simulation_modules() {
    return static_cast<unsigned short>(sim_module_defs.size());
}

xia::pixie::crate::crate* api_crate::operator->() {
    return modules;
}

api_crate::operator xia::pixie::crate::crate&() {
    return *modules;
}

api_crate::operator xia::pixie::crate::view::module&() {
    return modules;
}

const api_crate::module_defs api_crate::sim_module_defs = {
    "device-number=0 slot=2 revision=15 eeprom-format=1 " \
    "serial-num=250 num-channels=16 adc-msps=250 adc-bits=16 adc-clk-div=2",
    "device-number=1 slot=3 revision=15 eeprom-format=1 " \
    "serial-num=1000 num-channels=16 adc-msps=500 adc-bits=14 adc-clk-div=5",
    "device-number=2 slot=4 revision=15 eeprom-format=1 " \
    "serial-num=1001 num-channels=16 adc-msps=250 adc-bits=14 adc-clk-div=2",
    "device-number=3 slot=5 revision=17 eeprom-format=2 " \
    "serial-num=1002 num-channels=32 adc-msps=250 adc-bits=14 adc-clk-div=2 " \
    "db_pid=4 db_count=4"};

/*
 * The crate. We only handle a single crate with the legacy API.
 */
static api_crate crate;

stats_legacy::stats_legacy(const xia::pixie::hw::configs& configs)
    : marker_1(mark_1), marker_2(mark_2) {
    for (size_t channel = 0; channel < configs.size(); ++channel) {
        channels[channel].config = configs[channel];
    }
}

void stats_legacy::validate() const {
    if (marker_1 != mark_1 || marker_2 != mark_2) {
        throw xia_error(xia_error::code::invalid_value, "statistics data corrupt");
    }
}

static int not_supported() {
    return xia::pixie::error::return_code_not_supported();
}

template<class T>
T set_bit(const std::string& name, unsigned short& bit, T value, const bool& bit_status) {
    T local_val = value;
    try {
        auto value_bits = std::bitset<std::numeric_limits<T>::digits>(value);
        value_bits.set(bit, bit_status);
        return (T) value_bits.to_ulong();
    } catch (std::out_of_range& out_of_range) {
        xia_log(xia::log::warning) << name << ": bit out of range - " << out_of_range.what();
    }
    return local_val;
}

template<class T>
T test_bit(const std::string& name, unsigned short& bit, T value) {
    try {
        return std::bitset<std::numeric_limits<T>::digits>(value).test(bit);
    } catch (std::out_of_range& out_of_range) {
        xia_log(xia::log::warning) << name << ": bit out of range - " << out_of_range.what();
    }
    return false;
}

static int err_handler(std::function<int()> run) {
    try {
        return run();
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::invalid_argument& e) {
        xia_log(xia::log::error) << "invalid argument: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::runtime_error& e) {
        xia_log(xia::log::error) << "runtime: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }
}

PIXIE_EXPORT double PIXIE_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber) {
    return double(xia::util::numerics::ieee_float(IEEEFloatingNumber));
}

PIXIE_EXPORT unsigned int PIXIE_API Decimal2IEEEFloating(double DecimalNumber) {
    return (unsigned int) (xia::util::numerics::ieee_float(DecimalNumber));
}

PIXIE_EXPORT unsigned int PIXIE_API Pixie16GetStatisticsSize(void) {
    return sizeof(stats_legacy);
}

PIXIE_EXPORT int PIXIE_API PixieGetHistogramLength(const unsigned short mod_num,
                                                   const unsigned short chan_num,
                                                   unsigned int* hist_length) {
    auto call = [&mod_num, &chan_num, &hist_length]() {
        if (hist_length == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "hist_length is null");
        }
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, mod_num);
        module->channel_check(chan_num);
        *hist_length = static_cast<unsigned int>(
            module->channels[chan_num].fixture->config.max_histogram_length);
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieGetTraceLength(const unsigned short mod_num,
                                               const unsigned short chan_num,
                                               unsigned int* trace_length) {
    auto call = [&mod_num, &chan_num, &trace_length]() {
        if (trace_length == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "trace_length is null");
        }
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, mod_num);
        module->channel_check(chan_num);
        *trace_length = static_cast<unsigned int>(
            module->channels[chan_num].fixture->config.max_adc_trace_length);
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieGetMaxNumBaselines(const unsigned short mod_num,
                                                   const unsigned short chan_num,
                                                   unsigned int* max_num_baselines) {
    auto call = [&mod_num, &chan_num, &max_num_baselines]() {
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, mod_num);
        module->channel_check(chan_num);
        auto mxl = module->channels[chan_num].fixture->config.max_num_baselines;
        *max_num_baselines = static_cast<unsigned int>(mxl);
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieGetReturnCodeText(int return_code, char* buf,
                                                  unsigned int buf_size) {
    std::string msg = xia::pixie::error::api_result_text(return_code);
    if (buf_size < msg.size()) {
        xia::pixie::error::error error(xia::pixie::error::code::invalid_value,
                                       "buffer size not large enough");
        return error.return_code();
    }
    strncpy(buf, msg.c_str(), msg.size());
    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16AcquireADCTrace(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16AcquireADCTrace: ModNum=" << ModNum;

    auto call = [&ModNum]() {
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        if (!crate.run_check_override) {
            module->run_check();
        }
        module->get_traces();
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16AcquireBaselines(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16AcquireBaselines: ModNum=" << ModNum;

    auto call = [&ModNum]() {
        crate->ready();
        if (ModNum == crate.modules.num_modules) {
            if (!crate.run_check_override) {
                crate->check_active_run();
            }
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; mod_num++) {
                xia::pixie::crate::view::module_handle module(crate, mod_num);
                if (*module == xia::pixie::hw::rev_H) {
                    return not_supported();
                }
                module->acquire_baselines();
            }
        } else {
            xia::pixie::crate::view::module_handle module(crate, ModNum);
            if (!crate.run_check_override) {
                module->run_check();
            }
            if (*module == xia::pixie::hw::rev_H) {
                return not_supported();
            }
            module->acquire_baselines();
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16AdjustOffsets(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16AdjustOffsets: ModNum=" << ModNum;

    auto call = [&ModNum]() {
        crate->ready();
        if (ModNum == crate.modules.num_modules) {
            if (!crate.run_check_override) {
                crate->check_active_run();
            }
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; mod_num++) {
                xia::pixie::crate::view::module_handle module(crate, mod_num);
                module->adjust_offsets();
            }
        } else {
            xia::pixie::crate::view::module_handle module(crate, ModNum);
            if (!crate.run_check_override) {
                module->run_check();
            }
            module->adjust_offsets();
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16BLcutFinder(unsigned short ModNum, unsigned short ChanNum,
                                              unsigned int* BLcut) {
    xia_log(xia::log::debug) << "Pixie16BLcutFinder: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum;

    auto call = [&ModNum, &ChanNum, &BLcut]() {
        if (BLcut == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "BLcut is null");
        }
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        if (!crate.run_check_override) {
            module->run_check();
        }
        module->channel_check(ChanNum);

        if (*module == xia::pixie::hw::rev_H) {
            return not_supported();
        }

        xia::pixie::channel::range channels = {size_t(ChanNum)};
        xia::pixie::param::values values;
        module->bl_find_cut(channels, values);
        *BLcut = values[0];
        return 0;
    };

    return err_handler(call);
}

static void PixieBootModule(xia::pixie::module::module& module, const char* ComFPGAConfigFile,
                            const char* SPFPGAConfigFile, const char* DSPCodeFile,
                            const char* DSPParFile, const char* DSPVarFile,
                            unsigned short BootPattern) {
    using firmware_set = xia::pixie::firmware::firmware_set;
    using device_detail = xia::pixie::firmware::device_detail;
    using firmware = xia::pixie::firmware::firmware;
    using hw_config = xia::pixie::hw::config;

    hw_config& config = module.eeprom.configs[0];

    firmware_set user_fw;

    device_detail device;
    device.mod_revision = module.revision;
    device.mod_adc_msps = config.adc_msps;
    device.mod_adc_bits = config.adc_bits;

    device.name = "sys";
    firmware comm_fw(device, "n/a", 1);
    device.name = "fippi";
    firmware fippi_fw(device, "n/a", 0xF);
    device.name = "dsp";
    firmware dsp_fw(device, "n/a", 1);
    device.name = "var";
    firmware dsp_var(device, "n/a", 1);

    user_fw.slot.push_back(module.slot);

    comm_fw.filename = ComFPGAConfigFile;
    user_fw.add(comm_fw);

    fippi_fw.filename = SPFPGAConfigFile;
    user_fw.add(fippi_fw);

    dsp_fw.filename = DSPCodeFile;
    user_fw.add(dsp_fw);

    dsp_var.filename = DSPVarFile;
    user_fw.add(dsp_var);

    xia::pixie::firmware::add(crate->firmware, user_fw);

    const auto num_bits = std::numeric_limits<unsigned short>::digits;
    auto pattern = std::bitset<num_bits>(BootPattern);

    xia::pixie::module::module::boot_params boot_params;
    boot_params.boot_comms = pattern.test(BOOTPATTERN_COMFPGA_BIT);
    boot_params.boot_fippi = pattern.test(BOOTPATTERN_SPFPGA_BIT);
    boot_params.boot_dsp = pattern.test(BOOTPATTERN_DSPCODE_BIT);

    firmware_set fw_set;
    xia::pixie::firmware::find_filter filter(module.get_fw_tag(), module.slot);
    xia::pixie::firmware::find(fw_set, crate->firmware, filter);

    module.probe(fw_set);
    module.boot(boot_params, fw_set);

    if (pattern.test(BOOTPATTERN_DSPPAR_BIT)) {
        xia::pixie::config::import_settings_file(DSPParFile, module);
    }

    module.sync_hw(pattern.test(BOOTPATTERN_PROGFIPPI_BIT), pattern.test(BOOTPATTERN_SETDACS_BIT));
}

static void PixieBootModule(xia::pixie::module::module& module,
                            const char* DSPParFile, unsigned short BootPattern) {
    using firmware_set = xia::pixie::firmware::firmware_set;

    const auto num_bits = std::numeric_limits<unsigned short>::digits;
    auto pattern = std::bitset<num_bits>(BootPattern);

    xia::pixie::module::module::boot_params boot_params;
    boot_params.boot_comms = pattern.test(BOOTPATTERN_COMFPGA_BIT);
    boot_params.boot_fippi = pattern.test(BOOTPATTERN_SPFPGA_BIT);
    boot_params.boot_dsp = pattern.test(BOOTPATTERN_DSPCODE_BIT);

    firmware_set fw_set;
    xia::pixie::firmware::find_filter filter(module.get_fw_tag(), module.slot);
    xia::pixie::firmware::find(fw_set, crate->firmware, filter);

    module.probe(fw_set);
    module.boot(boot_params, fw_set);

    if (pattern.test(BOOTPATTERN_DSPPAR_BIT)) {
        xia::pixie::config::import_settings_file(DSPParFile, module);
    }

    module.sync_hw(
      pattern.test(BOOTPATTERN_PROGFIPPI_BIT), pattern.test(BOOTPATTERN_SETDACS_BIT));
}

PIXIE_EXPORT int PIXIE_API Pixie16BootModule(const char* ComFPGAConfigFile,
                                             const char* SPFPGAConfigFile, const char*,
                                             const char* DSPCodeFile, const char* DSPParFile,
                                             const char* DSPVarFile, unsigned short ModNum,
                                             unsigned short BootPattern) {
    auto call = [&ComFPGAConfigFile, &SPFPGAConfigFile, &DSPCodeFile, &DSPParFile,
                 &DSPVarFile, &ModNum, &BootPattern]() {
        if (ComFPGAConfigFile == nullptr) {
            throw xia_error(xia_error::code::invalid_value,
                            "ComFPGAConfigFile is null");
        }
        if (SPFPGAConfigFile == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "SPFPGAConfigFile is null");
        }
        if (DSPCodeFile == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "DSPCodeFile is null");
        }
        if (DSPParFile == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "DSPParFile is null");
        }
        if (DSPVarFile == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "DSPVarFile is null");
        }

        xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum << std::hex
                                << " BootPattern=0x" << BootPattern;
        xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum
                                << " ComFPGAConfigFile=" << ComFPGAConfigFile;
        xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum
                                << " SPFPGAConfigFile=" << SPFPGAConfigFile;
        xia_log(xia::log::info)
            << "Pixie16BootModule: ModNum=" << ModNum << " DSPCodeFile=" << DSPCodeFile;
        xia_log(xia::log::info)
            << "Pixie16BootModule: ModNum=" << ModNum << " DSPParFile=" << DSPParFile;
        xia_log(xia::log::info)
            << "Pixie16BootModule: ModNum=" << ModNum << " DSPVarFile=" << DSPVarFile;

        const unsigned short device_boot_mask = (1 << BOOTPATTERN_COMFPGA_BIT) |
                                                (1 << BOOTPATTERN_SPFPGA_BIT) |
                                                (1 << BOOTPATTERN_DSPCODE_BIT);
        const auto devices_boot = BootPattern & device_boot_mask;

        if (devices_boot != 0 && devices_boot != device_boot_mask) {
            throw xia_error(xia_error::code::invalid_value,
                            "must boot COMM, FPGA and DSP together");
        }

        xia::pixie::crate::crate::user user(crate);
        crate->ready();

        if (ModNum == crate.modules.num_modules) {
            if (!crate.run_check_override) {
                crate->check_active_run();
            }
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; ++mod_num) {
                PixieBootModule(crate.modules[mod_num], ComFPGAConfigFile, SPFPGAConfigFile, DSPCodeFile,
                                DSPParFile, DSPVarFile, BootPattern);
            }
        } else {
            if (!crate.run_check_override) {
                crate.modules[ModNum].run_check();
            }
            PixieBootModule(crate.modules[ModNum], ComFPGAConfigFile, SPFPGAConfigFile,
                            DSPCodeFile, DSPParFile, DSPVarFile, BootPattern);
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16BootModuleFirmware(const char* DSPParFile, unsigned short ModNum,
                                             unsigned short BootPattern) {
    auto call = [&DSPParFile, &ModNum, &BootPattern]() {
        if (DSPParFile == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "DSPParFile is null");
        }
        xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum << std::hex
                                << " BootPattern=0x" << BootPattern;
        xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum
                                << " DSPParFile=" << DSPParFile;

        const unsigned short device_boot_mask = (1 << BOOTPATTERN_COMFPGA_BIT) |
                                                (1 << BOOTPATTERN_SPFPGA_BIT) |
                                                (1 << BOOTPATTERN_DSPCODE_BIT);
        const auto devices_boot = BootPattern & device_boot_mask;

        if (devices_boot != 0 && devices_boot != device_boot_mask) {
            throw xia_error(xia_error::code::invalid_value,
                            "invalid value: must boot COMM, FPGA and DSP together");
        }

        crate->ready();

        if (ModNum == crate.modules.num_modules) {
            if (!crate.run_check_override) {
                crate->check_active_run();
            }
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; ++mod_num) {
                PixieBootModule(crate.modules[mod_num], DSPParFile, BootPattern);
            }
        } else {
            if (!crate.run_check_override) {
                crate.modules[ModNum].run_check();
            }
            PixieBootModule(crate.modules[ModNum], DSPParFile, BootPattern);
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16CheckExternalFIFOStatus(unsigned int* nFIFOWords,
                                                          unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16CheckExternalFIFOStatus: ModNum=" << ModNum;

    auto call = [&nFIFOWords, &ModNum]() {
        if (nFIFOWords == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "nFIFOWords is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        *nFIFOWords = static_cast<unsigned int>(module->read_list_mode_level());
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16CheckRunStatus(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16CheckRunStatus: ModNum=" << ModNum;

    auto call = [&ModNum]() {
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        if (module->run_active()) {
            return 1;
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeInputCountRate(unsigned int* Statistics,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeInputCountRate: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum;

    double result = 0;

    auto call = [&Statistics, &ChanNum, &result]() {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics is null");
        }
        stats_legacy_ptr stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].input_count_rate();
        return 0;
    };

    auto rc = err_handler(call);
    if (rc < 0) {
        return rc;
    }
    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeLiveTime(unsigned int* Statistics,
                                                     unsigned short ModNum,
                                                     unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeLiveTime: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum;

    double result = 0;

    auto call = [&Statistics, &ChanNum, &result]() {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics is null");
        }
        auto stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].live_time();
        return 0;
    };

    auto rc = err_handler(call);
    if (rc < 0) {
        return rc;
    }
    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeOutputCountRate(unsigned int* Statistics,
                                                            unsigned short ModNum,
                                                            unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeOutputCountRate: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum;

    double result = 0;

    auto call = [&Statistics, &ChanNum, &result]() {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics is null");
        }
        stats_legacy_ptr stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].output_count_rate();
        return 0;
    };

    auto rc = err_handler(call);
    if (rc < 0) {
        return rc;
    }
    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeRawInputCount(unsigned int* Statistics,
                                                          unsigned short ModNum,
                                                          unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeRawInputCount: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum;

    double result = 0;

    auto call = [&Statistics, &ChanNum, &result]() {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics is null");
        }
        auto stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].input_counts();
        return 0;
    };

    auto rc = err_handler(call);
    if (rc < 0) {
        return rc;
    }
    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeRawOutputCount(unsigned int* Statistics,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeRawOutputCount: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum;

    double result = 0;

    auto call = [&Statistics, &ChanNum, &result]() {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics is null");
        }
        auto stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].output_counts();
        return 0;
    };

    auto rc = err_handler(call);
    if (rc < 0) {
        return rc;
    }
    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeRealTime(unsigned int* Statistics,
                                                     unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeRealTime: ModNum=" << ModNum;

    double result = 0;

    auto call = [&Statistics, &result]() {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics is null");
        }
        auto stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        result = double(stats->module.real_time());
        return 0;
    };

    auto rc = err_handler(call);
    if (rc < 0) {
        return rc;
    }
    return result;
}

PIXIE_EXPORT int PIXIE_API Pixie16CopyDSPParameters(unsigned short BitMask,
                                                    unsigned short SourceModule,
                                                    unsigned short SourceChannel,
                                                    unsigned short* DestinationMask) {
    auto call = [&BitMask, &SourceModule, &SourceChannel, &DestinationMask]() {
        if (DestinationMask == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "DestinationMask is null");
        }

        xia_log(xia::log::debug) << "Pixie16CopyDSPParameters: Source Module=" << SourceModule
                                 << " Source Channel=" << SourceChannel
                                 << " Destination Mask=" << DestinationMask
                                 << " Bit Mask=" << BitMask;

        crate->ready();
        xia::pixie::crate::view::module_handle source(crate, SourceModule);

        if (SourceChannel > source->num_channels) {
            throw xia_error(xia_error::code::invalid_value, "SourceChannel too large");
        }

        if (!crate.run_check_override) {
            crate->check_active_run();
        }

        size_t offset = 0;
        for (size_t dest_mod = 0; dest_mod < crate.modules.num_modules; dest_mod++) {
            xia::pixie::crate::view::module_handle dest_handle(crate, dest_mod);

            for (size_t dest_chan = 0; dest_chan < dest_handle->num_channels; dest_chan++) {
                if (DestinationMask[offset + dest_chan] == 0) {
                    continue;
                }
                xia::pixie::param::copy_parameters(BitMask, source->channels[SourceChannel].vars,
                                                   dest_handle->channels[dest_chan].vars,
                                                   source->module_vars, dest_handle->module_vars);
            }
            offset += dest_handle->num_channels;
            dest_handle->sync_vars();
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16LoadDSPParametersFromFile(const char* FileName) {
    xia_log(xia::log::debug) << "Pixie16LoadDSPParametersFromFile: FileName=" << FileName;

    auto call = [&FileName]() {
        crate->ready();
        if (!crate.run_check_override) {
            crate->check_active_run();
        }
        xia::pixie::crate::crate::user user(crate);
        for (size_t mod_num = 0; mod_num < crate.modules.num_modules; ++mod_num) {
            xia::pixie::crate::view::module_handle module(crate, mod_num);
            xia::pixie::config::import_settings_file(FileName, *module);
            xia::pixie::hw::run::control(
              *module, xia::pixie::hw::run::control_task::program_fippi);
            module->set_dacs();
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16EndRun(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16EndRun: ModNum=" << ModNum;

    auto call = [&ModNum]() {
        crate->ready();
        if (ModNum == crate.modules.num_modules) {
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; mod_num++) {
                xia::pixie::crate::view::module_handle module(crate, mod_num);
                module->run_end();
            }
        } else {
            xia::pixie::crate::view::module_handle module(crate, ModNum);
            module->run_end();
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16ExitSystem(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ExitSystem: ModNum=" << ModNum;

    auto call = [&ModNum]() {
        crate->ready();
        if (ModNum == crate.modules.num_modules) {
            if (!crate.run_check_override) {
                crate->check_active_run();
            }
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; mod_num++) {
                xia::pixie::crate::view::module_handle
                  module(crate, mod_num, xia::pixie::module::check::open);
                module->close();
            }
        } else {
            xia::pixie::crate::view::module_handle
              module(crate, ModNum, xia::pixie::module::check::open);
            module->run_check();
            module->close();
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16InitSystem(
    unsigned short NumModules, unsigned short* PXISlotMap,
    unsigned short OfflineMode) {
    /*
     * Create a log file. The environment can change the level of logging.
     */
    xia::log::level log_level = xia::log::info;
    const char* env_log_level = std::getenv("PIXIE16_LOG_LEVEL");
    if (env_log_level != nullptr) {
        std::string ell = env_log_level;
        if (ell == "DEBUG" || ell == "debug") {
            log_level = xia::log::debug;
        } else if (ell == "INFO" || ell == "info") {
            log_level = xia::log::info;
        } else if (ell == "WARNING" || ell == "warning" || ell == "WARN" || ell == "warn") {
            log_level = xia::log::warning;
        } else if (ell == "ERROR" || ell == "error") {
            log_level = xia::log::error;
        }
    }
    xia::logging::start("log", "Pixie16Msg.log", true);
    xia::logging::set_level(log_level);

    auto call = [&NumModules, &PXISlotMap, &OfflineMode]() {
        if (NumModules > xia::pixie::hw::max_slots) {
            throw xia_error(xia::pixie::error::code::module_total_invalid,
                            "requested more modules than slots");
        }

        if (NumModules == 0) {
            throw xia_error(xia::pixie::error::code::module_total_invalid,
                            "no modules requested");
        }

        if (OfflineMode > 1) {
            xia_log(xia::log::warning) << "Pixie16InitSystem: OfflineMode can only be 0 or 1";
            OfflineMode = 1;
        }

        if (OfflineMode == 1) {
            crate.set_simulation();
            if (NumModules > crate.num_simulation_modules())
                throw xia_error(xia::pixie::error::code::module_total_invalid,
                                "more modules requested than available");
            xia::pixie::sim::load_firmware_sets(crate->firmware);
        }

        xia_log(xia::log::info) << "Pixie16InitSystem: NumModules=" << NumModules
                                << " PXISlotMap=" << PXISlotMap << " OfflineMode=" << OfflineMode;

        /*
         * Load the system firmware into the crate
         */
        xia::pixie::firmware::load_system_firmwares(crate->firmware);

        crate->initialize();

        if (crate.modules.num_modules == 0) {
            crate->shutdown();
            throw xia_error(xia::pixie::error::code::module_total_invalid,
                            "Crate did not initialize with any modules.");
        }

        /*
         * If the number of modules requested is greater than the number
         * of modules in the crate, then it is an error.
         */
        if (NumModules > crate.modules.num_modules) {
            crate->shutdown();
            throw xia_error(xia_error::code::module_total_invalid,
                            "module count does not match user supplied "
                            "number of modules");
        }

        /*
         * Only handle the index to slot assignment if the user supplied the
         * number of modules (ie the length of the array) and the array.
         */
        if (NumModules > 0 && PXISlotMap != nullptr) {
            xia::pixie::module::number_slots numbers;
            for (int i = 0; i < int(NumModules); ++i) {
                using number_slot = xia::pixie::module::number_slot;
                xia_log(xia::log::info)
                    << "Pixie16InitSystem: slot map: " << PXISlotMap[i] << " => " << i;
                numbers.push_back(number_slot(i, PXISlotMap[i]));
            }
            crate.modules.assign(numbers);
        }
        return 0;
    };

    auto rc = err_handler(call);
    if (rc < 0) {
        try {
            crate->shutdown();
        } catch (...) {
        }
    }
    return rc;
}

PIXIE_EXPORT int PIXIE_API Pixie16LoadModuleFirmware(const char* SearchPath) {
    auto call = [&SearchPath]() {
        if (SearchPath == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "SearchPath is null");
        }
        crate->ready();
        if (!crate.run_check_override) {
            crate->check_active_run();
        }
        xia::pixie::firmware::load_firmwares(crate->firmware, SearchPath);
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16SetModuleFirmware(const char* FwFile, unsigned int ModSlot, const char* Device) {
    auto call = [&FwFile, &ModSlot, &Device]() {
        if (FwFile == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "FwFile is null");
        }
        if (Device == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "Device is null");
        }

        using firmware = xia::pixie::firmware::firmware;
        using firmware_set = xia::pixie::firmware::firmware_set;
        using device_detail = xia::pixie::firmware::device_detail;
        using hw_config = xia::pixie::hw::config;
        crate->ready();
        if (!crate.run_check_override) {
            crate->check_active_run();
        }
        xia::pixie::module::module_ptr module = crate->find(ModSlot);

        if (!module->present() || !module) {
            throw xia_error(xia_error::code::module_not_found, "invalid slot");
        }

        hw_config config = module->eeprom.configs[0];
        device_detail dev;
        dev.name = Device;
        dev.mod_revision = module->revision;
        dev.mod_adc_msps = config.adc_msps;
        dev.mod_adc_bits = config.adc_bits;
        firmware fw(dev, "unknown", 1);
        fw.filename = FwFile;
        firmware_set fw_set;
        fw_set.add(fw);
        fw_set.slot.push_back(ModSlot);
        xia::pixie::firmware::add(crate->firmware, fw_set);
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data,
                                                           unsigned int nFIFOWords,
                                                           unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ReadDataFromExternalFIFO: ModNum=" << ModNum
                             << " nFIFOWords=" << nFIFOWords;

    auto call = [&ExtFIFO_Data, &nFIFOWords, &ModNum]() {
        if (ExtFIFO_Data == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "nFIFOWords is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);

        xia::pixie::hw::words data(nFIFOWords);
        auto copied = module->read_list_mode(data);
        if (copied != nFIFOWords) {
            xia_log(xia::log::error)
                << "Failed to read FIFO words, requested nFIFOWords (" << nFIFOWords
                << "), copied " << copied
                << " for Module " << ModNum << ". Remaining values filled with zero.";
        }
        for (size_t i = 0; i < copied; i++) {
            ExtFIFO_Data[i] = data[i];
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadHistogramFromModule(unsigned int* Histogram,
                                                          unsigned int NumWords,
                                                          unsigned short ModNum,
                                                          unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ReadHistogramFromModule: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum << " NumWords=" << NumWords;

    auto call = [&Histogram, &NumWords, &ModNum, &ChanNum]() {
        if (Histogram == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "Histogram is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        module->channel_check(ChanNum);
        auto& chan = module->channels[ChanNum];
        auto read_words = NumWords;
        if (read_words > chan.fixture->config.max_histogram_length) {
            xia_log(xia::log::warning)
                << "NumWords (" << NumWords << ") greater than the max_histogram_length ("
                << chan.fixture->config.max_histogram_length << ") for Module " << ModNum
                << " Channel " << ChanNum
                << ". Truncating to the maximum length and filling with max bin values.";
            read_words = (unsigned int) (chan.fixture->config.max_histogram_length);

            for (unsigned int i = read_words; i < NumWords; i++)
                Histogram[i] = std::numeric_limits<unsigned int>::max();
        } else {
            xia_log(xia::log::warning)
                << "NumWords (" << NumWords << ") less than the max_histogram_length ("
                << chan.fixture->config.max_histogram_length << ") for Module " << ModNum
                << " Channel " << ChanNum << ". You may not be capturing all the data.";
        }
        module->read_histogram(ChanNum, Histogram, read_words);
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadModuleInfo(unsigned short ModNum, unsigned short* ModRev,
                                                 unsigned int* ModSerNum,
                                                 unsigned short* ModADCBits,
                                                 unsigned short* ModADCMSPS) {
    xia_log(xia::log::debug) << "Pixie16ReadModuleInfo: ModNum=" << ModNum;

    auto call = [&ModNum, &ModRev, &ModSerNum, &ModADCBits, &ModADCMSPS]() {
        if (ModRev == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ModRev is null");
        }
        if (ModSerNum == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ModSerNum is null");
        }
        if (ModADCBits == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ModADCBits is null");
        }
        if (ModADCMSPS == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ModADCMSPS is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum,
                                                xia::pixie::module::check::open);
        if (ModRev)
            *ModRev = module->revision;
        if (ModSerNum)
            *ModSerNum = module->serial_num;
        if (ModADCBits)
            *ModADCBits = module->eeprom.configs[0].adc_bits;
        if (ModADCMSPS)
            *ModADCMSPS = module->eeprom.configs[0].adc_msps;
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieGetModuleInfo(unsigned short mod_num, module_config* cfg) {
    xia_log(xia::log::debug) << "PixieReadModuleInfo: ModNum=" << mod_num;

    auto call = [&mod_num, &cfg]() {
        if (cfg == nullptr) {
            throw xia::pixie::error::error(
                xia::pixie::error::code::module_info_failure, "cfg is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, mod_num,
            xia::pixie::module::check::open);

        std::memset(cfg, 0, sizeof(*cfg));

        cfg->adc_bit_resolution = module->eeprom.configs[0].adc_bits;
        cfg->adc_sampling_frequency = module->eeprom.configs[0].adc_msps;
        cfg->number = module->number;
        cfg->number_of_channels = (unsigned short) (module->num_channels);
        cfg->revision = module->revision;
        cfg->serial_number = module->serial_num;
        cfg->slot = static_cast<unsigned short>(module->slot);

        if (module->online()) {
            using firmware_set = xia::pixie::firmware::firmware_set;

            firmware_set fw_set;
            module->firmware_get(fw_set, crate->firmware);

            if (fw_set.device_count() > PIXIE16_API_MOD_CONFIG_MAX_DEVICES) {
                throw xia_error(xia_error::code::internal_failure,
                                "Too many devices in firmware set");
            }

            std::ostringstream oss;
            oss << fw_set.release;
            std::strncpy(cfg->fw_revision, oss.str().c_str(), sizeof(cfg->fw_revision) - 1);
            std::strncpy(cfg->fw_tag, fw_set.tag().c_str(), sizeof(cfg->fw_tag) - 1);
            std::strncpy(cfg->fw_type, xia::pixie::firmware::set_type_label(fw_set.type()),
                         sizeof(cfg->fw_type) - 1);
            for (size_t dev = 0; dev < fw_set.device_count(); ++dev) {
                auto& dev_name = fw_set.get_devices()[dev];
                auto fw = fw_set.get(dev_name);
                std::strncpy(cfg->fw_device[dev], dev_name.c_str(),
                             sizeof(cfg->fw_device[dev]) - 1);
                std::strncpy(cfg->fw_device_file[dev], fw->filename.c_str(),
                             sizeof(cfg->fw_device_file[dev]) - 1);
            }
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanADCTrace(unsigned short* Trace_Buffer,
                                                      unsigned int Trace_Length,
                                                      unsigned short ModNum,
                                                      unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ReadSglChanADCTrace: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum << " Trace_Length=" << Trace_Length;

    auto call = [&Trace_Buffer, &Trace_Length, &ModNum, &ChanNum]() {
        if (Trace_Buffer == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "Trace_Buffer is null");
        }
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        if (!crate.run_check_override) {
            module->run_check();
        }
        module->channel_check(ChanNum);
        module->read_adc(ChanNum, Trace_Buffer, Trace_Length, false);
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanBaselines(double* Baselines, double* TimeStamps,
                                                       unsigned short NumBases,
                                                       unsigned short ModNum,
                                                       unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ReadSglChanBaselines: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum << " NumBases=" << NumBases;

    auto call = [&Baselines, &TimeStamps, &NumBases, &ModNum, &ChanNum]() {
        if (Baselines == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "Baselines is null");
        }
        if (TimeStamps == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "TimeStamps is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        if (!crate.run_check_override) {
            module->run_check();
        }
        module->channel_check(ChanNum);

        xia::pixie::channel::range channels = {size_t(ChanNum)};
        xia::pixie::channel::baseline::channels_values values(1);
        module->bl_get(channels, values, false);
        xia::pixie::channel::baseline::values& cv = values[0];
        for (size_t v = 0; v < size_t(NumBases); ++v) {
            TimeStamps[v] = std::get<0>(cv[v]);
            Baselines[v] = std::get<1>(cv[v]);
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanPar(const char* ChanParName, double* ChanParData,
                                                 unsigned short ModNum, unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ReadSglChanPar: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum
                             << " ChanParName=" << ChanParName;

    auto call = [&ChanParName, &ChanParData, &ModNum, &ChanNum]() {
        if (ChanParName == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ChanParName is null");
        }
        if (ChanParData == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ChanParData is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        if (!crate.run_check_override) {
            module->run_check();
        }
        module->channel_check(ChanNum);

        *ChanParData = module->read(ChanParName, ChanNum);
        xia_log(xia::log::debug) << "Pixie16ReadSglChanPar: ModNum=" << ModNum
                                 << " ChanNum=" << ChanNum << " ChanParName=" << ChanParName
                                 << " ChanParData=" << *ChanParData;
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglModPar(const char* ModParName, unsigned int* ModParData,
                                                unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ReadSglModPar: ModNum=" << ModNum
                             << " ModParName=" << ModParName;

    auto call = [&ModParName, &ModParData, &ModNum]() {
        if (ModParName == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ModParName is null");
        }
        if (ModParData == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ModParData is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        if (!crate.run_check_override) {
            module->run_check();
        }
        *ModParData = module->read(ModParName);
        xia_log(xia::log::debug) << "Pixie16ReadSglModPar: ModNum=" << ModNum
                                 << " ModParName=" << ModParName
                                 << " ModParData=" << *ModParData;
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadStatisticsFromModule(unsigned int* Statistics,
                                                           unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ReadStatisticsFromModule: ModNum=" << ModNum;

    auto call = [&Statistics, &ModNum]() {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics is null");
        }
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        stats_legacy_ptr legacy_stats = new (Statistics) stats_legacy(module->eeprom.configs);
        legacy_stats->validate();
        xia::pixie::stats::stats stats(*module);
        module->read_stats(stats);
        legacy_stats->num_channels = module->num_channels;
        legacy_stats->module = stats.mod;
        for (size_t channel = 0; channel < module->num_channels; ++channel) {
            legacy_stats->channels[channel] = stats.chans[channel];
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16SaveDSPParametersToFile(const char* FileName) {
    xia_log(xia::log::debug) << "Pixie16SaveDSPParametersToFile: FileName=" << FileName;

    auto call = [&FileName]() {
        if (!crate.run_check_override) {
            crate->check_active_run();
        }
        xia::pixie::config::export_settings_file(FileName, crate);
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16SetDACs(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16SetDACs: ModNum=" << ModNum;

    auto call = [&ModNum]() {
        crate->ready();
        if (ModNum == crate.modules.num_modules) {
            if (!crate.run_check_override) {
                crate->check_active_run();
            }
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; mod_num++) {
                xia::pixie::crate::view::module_handle module(crate, mod_num);
                module->set_dacs();
            }
        } else {
            xia::pixie::crate::view::module_handle module(crate, ModNum);
            if (!crate.run_check_override) {
                module->run_check();
            }
            module->set_dacs();
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16StartHistogramRun(unsigned short ModNum, unsigned short mode) {
    xia_log(xia::log::debug) << "Pixie16StartHistogramRun: ModNum=" << ModNum
                             << " mode=" << mode;

    auto call = [&ModNum, &mode]() {
        xia::pixie::hw::run::run_mode run_mode;
        switch (mode) {
            case 0:
                run_mode = xia::pixie::hw::run::run_mode::resume;
                break;
            case 1:
                run_mode = xia::pixie::hw::run::run_mode::new_run;
                break;
            default:
                throw xia_error(
                  xia_error::code::invalid_value, "invalid histogram start run mode");
        }
        crate->ready();
        if (ModNum == crate.modules.num_modules) {
            if (!crate.run_check_override) {
                crate->check_active_run();
            }
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; mod_num++) {
                xia::pixie::crate::view::module_handle module(crate, mod_num);
                module->start_histograms(run_mode);
            }
        } else {
            xia::pixie::crate::view::module_handle module(crate, ModNum);
            if (!crate.run_check_override) {
                module->run_check();
            }
            module->start_histograms(run_mode);
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16StartListModeRun(unsigned short ModNum, unsigned short RunType,
                                                   unsigned short mode) {
    xia_log(xia::log::debug) << "Pixie16StartListModeRun: ModNum=" << ModNum
                             << " RunType=" << RunType << " mode=" << mode;

    auto call = [&ModNum, &RunType, &mode]() {
        if (RunType != (unsigned short) (xia::pixie::hw::run::run_task::list_mode)) {
            throw xia_error(xia_error::code::invalid_value,
                            "invalid list-mode start run type (must be 0x100)");
        }
        xia::pixie::hw::run::run_mode run_mode;
        switch (mode) {
            case 0:
                run_mode = xia::pixie::hw::run::run_mode::resume;
                break;
            case 1:
                run_mode = xia::pixie::hw::run::run_mode::new_run;
                break;
            default:
                throw xia_error(
                  xia_error::code::invalid_value, "invalid list-mode start run mode");
        }

        crate->ready();
        if (ModNum == crate.modules.num_modules) {
            if (!crate.run_check_override) {
                crate->check_active_run();
            }
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; mod_num++) {
                xia::pixie::crate::view::module_handle module(crate, mod_num);
                module->start_listmode(run_mode);
            }
        } else {
            xia::pixie::crate::view::module_handle module(crate, ModNum);
            if (!crate.run_check_override) {
                module->run_check();
            }
            module->start_listmode(run_mode);
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16TauFinder(unsigned short ModNum, double* Tau) {
    xia_log(xia::log::debug) << "Pixie16TauFinder: ModNum=" << ModNum;

    auto call = [&ModNum, &Tau]() {
        if (Tau == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "Tau is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        if (!crate.run_check_override) {
            module->run_check();
        }
        if (*module == xia::pixie::hw::rev_H) {
            return not_supported();
        }
        module->tau_finder();
        xia::pixie::hw::doubles taus;
        module->read_autotau(taus);
        std::copy(taus.begin(), taus.end(), Tau);
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16WriteSglChanPar(const char* ChanParName, double ChanParData,
                                                  unsigned short ModNum, unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16WriteSglChanPar: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum << " ChanParName=" << ChanParName
                             << " ChanParData=" << ChanParData;

    auto call = [&ChanParName, &ChanParData, &ModNum, &ChanNum]() {
        if (ChanParName == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ChanParName is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum);
        if (!crate.run_check_override) {
            module->run_check();
        }
        module->channel_check(ChanNum);
        module->write(ChanParName, ChanNum, ChanParData);
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API Pixie16WriteSglModPar(const char* ModParName, unsigned int ModParData,
                                                 unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16WriteSglModPar: ModNum=" << ModNum
                             << " ModParName=" << ModParName << " ModParData=" << ModParData;

    auto call = [&ModParName, &ModParData, &ModNum]() {
        if (ModParName == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "ModParName is null");
        }

        crate->ready();
        bool bcast;
        if (ModNum == crate.modules.num_modules) {
            bcast = true;
        } else {
            xia::pixie::crate::view::module_handle module(crate, ModNum);
            if (!crate.run_check_override) {
                module->run_check();
            }
            bcast = module->write(ModParName, ModParData);
        }
        if (bcast) {
            if (!crate.run_check_override) {
                crate->check_active_run();
            }
            xia::pixie::crate::crate::user user(crate);
            for (size_t mod_num = 0; mod_num < crate.modules.num_modules; ++mod_num) {
                if (ModNum != mod_num) {
                    xia::pixie::crate::view::module_handle module(crate, mod_num);
                    if (module->online()) {
                        module->write(ModParName, ModParData);
                    }
                }
            }
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieBootCrate(const char* settings_file,
                                          const PIXIE_BOOT_MODE boot_mode) {
    xia_log(xia::log::debug) << "PixieBootCrate: settings_file=" << settings_file
                             << " boot-mode=" << boot_mode;

    auto call = [&settings_file, &boot_mode]() {
        crate->ready();
        if (!crate.run_check_override) {
            crate->check_active_run();
        }

        bool import_settings;
        bool boot;
        bool force;
        bool probe = crate->probe();

        switch (boot_mode) {
            case PIXIE_BOOT_SETTINGS_LOAD:
                if (settings_file == nullptr || strlen(settings_file) == 0) {
                    throw xia_error(xia_error::code::invalid_value,
                                    "settings file is null or empty");
                }
                import_settings = true;
                boot = false;
                force = false;
                if (!probe) {
                    throw xia_error(xia_error::code::module_offline,
                                    "fast boot not available with offline modules");
                }
                break;
            case PIXIE_BOOT_RESET_LOAD:
                import_settings = settings_file != nullptr && strlen(settings_file) > 0;
                boot = true;
                force = true;
                break;
            default:
                import_settings = false;
                boot = !probe;
                force = false;
                break;
        }

        if (boot) {
            xia::pixie::crate::crate::boot_params boot_params;
            boot_params.force = force;
            crate->boot(boot_params);
        }

        if (import_settings) {
            xia::pixie::module::number_slots loaded_slots;
            crate->import_config(settings_file, loaded_slots);
            crate->initialize_afe();
        }
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieGetFifoConfiguration(const unsigned short mod_num,
                                                     module_fifo_config* fifo_config) {
    xia_log(xia::log::debug) << "PixieGetFifoConfiguration: Module=" << mod_num;

    auto call = [&mod_num, &fifo_config]() {
        if (fifo_config == nullptr) {
            throw xia::pixie::error::error(
                xia::pixie::error::code::module_info_failure, "fifo_config is null");
        }
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, mod_num,
                                                xia::pixie::module::check::open);
        fifo_config->bandwidth_mb_per_sec = module->fifo_bandwidth;
        fifo_config->buffers = module->fifo_buffers;
        fifo_config->dma_trigger_level_bytes = module->fifo_dma_trigger_level;
        fifo_config->hold_usecs = module->fifo_hold_usecs;
        fifo_config->idle_wait_usecs = module->fifo_idle_wait_usecs;
        fifo_config->run_wait_usecs = module->fifo_run_wait_usecs;
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieRegisterCrateFirmware(const unsigned int version, const int revision,
                                                      const int adc_msps, const int adc_bits,
                                                      const char* device, const char* path) {
    using firmware_set = xia::pixie::firmware::firmware_set;
    using firmware = xia::pixie::firmware::firmware;
    using device_detail = xia::pixie::firmware::device_detail;

    auto call = [&version, &revision, &adc_msps, &adc_bits, &device, &path]() {
        if (device == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "device is null");
        }
        if (path == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "path is null");
        }

        xia_log(xia::log::debug)
            << "Pixie16RegisterFirmware: version=" << version
            << " revision=" << revision << " adc_msps=" << adc_msps
            << " adc_bits=" << adc_bits << " device=" << device << " path=" << path;

        crate->ready();

        if (!crate.run_check_override) {
            crate->check_active_run();
        }
        std::string ver_s = std::to_string(version);
        firmware_set partial_fw;
        device_detail dev;
        dev.name = device;
        dev.mod_revision = revision;
        dev.mod_adc_msps = adc_msps;
        dev.mod_adc_bits = adc_bits;
        firmware fw(dev, ver_s, 1);
        fw.filename = path;
        partial_fw.add(fw);
        xia::pixie::firmware::add(crate->firmware, partial_fw);
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieRegisterFirmware(const unsigned int version, const int revision,
                                                 const int adc_msps, const int adc_bits,
                                                 const char* device, const char* path,
                                                 unsigned short ModNum) {
    using firmware_set = xia::pixie::firmware::firmware_set;
    using firmware = xia::pixie::firmware::firmware;
    using device_detail = xia::pixie::firmware::device_detail;

    auto call = [&version, &revision, &adc_msps, &adc_bits, &device, &path,
                 &ModNum]() {
        if (device == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "device is null");
        }
        if (path == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "path is null");
        }

        xia_log(xia::log::debug) << "PixieRegisterFirmware: version=" << version
                                 << " revision=" << revision << " adc_msps=" << adc_msps
                                 << " adc_bits=" << adc_bits << " device=" << device << " path=" << path
                                 << " ModNum=" << ModNum;

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, ModNum,
                                                xia::pixie::module::check::open);

        if (module->revision != revision) {
            xia_log(xia::log::error) << "requested " << revision
                                     << " != " << "expected " << module->revision;
            throw xia_error(
                xia_error::code::module_invalid_firmware,
                "can only register firmware to module with matching revision");
        }

        if (!crate.run_check_override) {
            module->run_check();
        }
        xia::pixie::hw::slot_type slot = module->slot;
        std::string ver_s = std::to_string(version);
        device_detail dev;
        dev.name = device;
        dev.mod_revision = revision;
        dev.mod_adc_msps = adc_msps;
        dev.mod_adc_bits = adc_bits;
        firmware fw(dev, ver_s, 1);
        fw.filename = path;
        firmware_set fw_set;
        fw_set.add(fw);
        fw_set.slot.push_back(slot);
        xia::pixie::firmware::add(crate->firmware, fw_set);
        return 0;
    };

    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSetFifoConfiguration(const unsigned short mod_num,
                                                     module_fifo_config* fifo_config) {
    xia_log(xia::log::debug) << "PixieSetFifoConfiguration: Module=" << mod_num;

    auto call = [&mod_num, &fifo_config]() {
        if (fifo_config == nullptr) {
            throw xia::pixie::error::error(
                xia::pixie::error::code::module_info_failure, "fifo_config is null");
        }
        crate->ready();
        xia::pixie::crate::view::module_handle module(crate, mod_num, xia::pixie::module::check::open);
        if (!crate.run_check_override) {
            module->run_check();
        }
        module->set_fifo_bandwidth(fifo_config->bandwidth_mb_per_sec);
        module->set_fifo_buffers(fifo_config->buffers);
        module->set_fifo_dma_trigger_level(fifo_config->dma_trigger_level_bytes);
        module->set_fifo_hold(fifo_config->hold_usecs);
        module->set_fifo_idle_wait(fifo_config->idle_wait_usecs);
        module->set_fifo_run_wait(fifo_config->run_wait_usecs);
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieReadRunFifoStats(unsigned short mod_num,
                                                 struct module_fifo_stats* fifo_stats) {
    xia_log(xia::log::debug) << "PixieReadModuleRunFifoStats: Module=" << mod_num;

    auto call = [&mod_num, &fifo_stats]() {
        if (fifo_stats == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "fifo_stats is null");
        }

        crate->ready();
        xia::pixie::crate::view::module_handle module(crate,
            mod_num, xia::pixie::module::check::open);
        xia::pixie::module::module::fifo_stats snapshot;
        snapshot = module->run_stats;
        fifo_stats->in = snapshot.get_in_bytes();
        fifo_stats->out = snapshot.get_out_bytes();
        fifo_stats->dma_in = snapshot.get_dma_in_bytes();
        fifo_stats->overflows = snapshot.overflows;
        fifo_stats->dropped = snapshot.dropped;
        fifo_stats->hw_overflows = snapshot.hw_overflows;
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT const char* PIXIE_API PixieGetInstallationPath(const enum PIXIE_INSTALL_PATH opt, ...) {
    switch (opt) {
        case PIXIE_PATH_FIRMWARE_DEFAULT:
            return xia::pixie::firmware::system_firmware_path;
        default:
            break;
    }
    return nullptr;
}

PIXIE_EXPORT int PIXIE_API PixieSysControlOpen(const char* path, const enum PIXIE_SYSCTL_FORMAT format) {
    auto call = [&path, &format]() {
        bool is_json;
        switch (format) {
            case PIXIE_SYSCTL_FORMAT_TEXT:
                is_json = false;
                break;
            case PIXIE_SYSCTL_FORMAT_JSON:
                is_json = true;
                break;
            default:
                throw xia_error(xia_error::code::invalid_value, "invalid system control format");
        }
        std::string spath;
        if (path != nullptr) {
            spath = path;
        }
        crate.sysctl.create();
        crate.sysctl_reader.seek(0);
        std::regex mib_match(spath);
        xia::pixie::format::json json_out;
        xia::mib::mib_walk_func walker =
            [&spath, is_json, &mib_match, &json_out](xia::mib::node& nod) {
                auto name = nod.name();
                if (spath.empty() || std::regex_search(name, mib_match)) {
                    if (is_json) {
                        xia::mib::mib_to_json(json_out, nod.str(), name);
                    } else {
                        std::stringstream oss;
                        oss << name << " = " << nod.str();
                        auto s = oss.str();
                        crate.sysctl.push(s);
                    }
                }
            };
        xia::mib::walk(walker);
        if (is_json) {
            auto s = json_out.dump();
            crate.sysctl.push(s);
        }
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlRead(char* buffer, size_t* size) {
    auto call = [&buffer, &size]() {
        if (size == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "size is null");
        }

        *size = crate.sysctl_reader.read(buffer, *size);
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlSize(size_t* size) {
    auto call = [&size]() {
        *size = crate.sysctl_reader.size();
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlClose(void) {
    auto call = []() {
        crate.sysctl.destroy();
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlGet(
    const char* path, char* value, size_t size) {
    auto call = [&path, &value, &size]() {
        if (path == nullptr) {
            throw xia_error(
                xia_error::code::invalid_value, "system control path is null");
        }

        if (value == nullptr) {
            throw xia_error(
                xia_error::code::invalid_value, "value is null");
        }

        if (size < 2) {
            throw xia_error(
                xia_error::code::invalid_value, "size too small");
        }

        auto node = xia::mib::find(path);
        node.set_hint(xia::mib::hint::fmt_no_quotes);
        auto s = node.str();
        size_t len = s.length();
        if (len >= size) {
            len = size - 1;
        }
        std::memcpy(value, s.c_str(), len);
        value[len] = '\0';
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlGetInt(const char* path, int* value) {
    auto call = [&path, &value]() {
        if (path == nullptr) {
            throw xia_error(
                xia_error::code::invalid_value, "system control path is null");
        }
        auto node = xia::mib::find(path);
        *value = node.get<int>();
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlGetDouble(const char* path, double* value) {
    auto call = [&path, &value]() {
        if (path == nullptr) {
            throw xia_error(
                xia_error::code::invalid_value, "system control path is null");
        }
        auto node = xia::mib::find(path);
        *value = node.get<double>();
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlSet(const char* path, const char* value) {
    auto call = [&path, &value]() {
        if (path == nullptr) {
            throw xia_error(
                xia_error::code::invalid_value, "system control path is null");
        }
        auto node = xia::mib::find(path);
        node.set_value(value);
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlSetInt(const char* path, const int value) {
    auto call = [&path, &value]() {
        if (path == nullptr) {
            throw xia_error(
                xia_error::code::invalid_value, "system control path is null");
        }
        auto node = xia::mib::find(path);
        node = value;
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlSetDouble(const char* path, const double value) {
    auto call = [&path, value]() {
        if (path == nullptr) {
            throw xia_error(
                xia_error::code::invalid_value, "system control path is null");
        }
        auto node = xia::mib::find(path);
        node = value;
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlSetValues(const char* values,
                                                    const PIXIE_SYSCTL_FORMAT format) {
    auto call = [&values, &format]() {
        if (values == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "values is null");
        }

        xia::pixie::format::json json_in;
        std::vector<xia::util::string::strings> values_in;
        bool is_json;
        switch (format) {
            case PIXIE_SYSCTL_FORMAT_TEXT:
                is_json = false;
                {
                    /*
                     * Using \n should be compatible on Unix and Windows
                     */
                    xia::util::string::strings vals;
                    xia::util::string::split(vals, values, '\n');
                    for (auto& val : vals) {
                        xia::util::string::strings key_value;
                        xia::util::string::split(key_value, val, '=', 2);
                        values_in.push_back(key_value);
                    }
                }
                break;
            case PIXIE_SYSCTL_FORMAT_JSON:
                is_json = true;
                try {
                    json_in = xia::pixie::format::json::parse(values);
                } catch (xia::pixie::format::json::exception& e) {
                    throw xia_error(
                        xia::pixie::error::code::config_json_error,
                        std::string("system control set values: ") + e.what());
                }
                break;
            default:
                throw xia_error(
                    xia_error::code::invalid_value, "invalid system control format");
        }
        xia::mib::mib_walk_func walker =
            [is_json, &values_in, &json_in](xia::mib::node& nod) {
                auto name = nod.name();
                if (is_json) {
                    xia::util::string::strings path_pieces;
                    xia::util::string::split(path_pieces, name, xia::mib::mibsep);
                    auto jvalue = json_in;
                    bool found = true;
                    for (auto& piece : path_pieces) {
                        if (!jvalue.contains(piece)) {
                            found = false;
                            break;
                        }
                        jvalue = jvalue[piece];
                    }
                    if (found) {
                        nod.set_value(jvalue);
                    }
                } else {
                    for (auto& key_val : values_in) {
                        if (name == key_val[0]) {
                            nod.set_value(key_val[1]);
                        }
                    }
                }
            };
        xia::mib::walk(walker);
        return 0;
    };
    return err_handler(call);
}

PIXIE_EXPORT int PIXIE_API PixieSysControlSetFileValues(const char* filename,
                                                        const PIXIE_SYSCTL_FORMAT format) {
    auto call = [&filename, &format]() {
        if (filename == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "filename is null");
        }

        switch (format) {
            case PIXIE_SYSCTL_FORMAT_TEXT:
            case PIXIE_SYSCTL_FORMAT_JSON:
                break;
            default:
                throw xia_error(xia_error::code::invalid_value, "invalid format");
        }

        xia::util::io::file file;
        file.open(filename, file.flag::ro);
        struct stat sb;
        auto r = ::fstat(file.handle, &sb);
        if (r != 0) {
            throw xia_error(
                xia_error::code::file_size_invalid, "cannot get file size");
        }
        auto size = size_t(sb.st_size) + 1;
        std::vector<char> data(size);
        file.read(data, size);
        file.close();
        data[size - 1] = '\0';
        return PixieSysControlSetValues(&data[0], format);
    };
    return err_handler(call);
}
