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

/** @file fixture.hpp
 * @brief Defines a hardware channel fixture
 */

#ifndef PIXIE_FIXTURE_H
#define PIXIE_FIXTURE_H

#include <functional>

#include <pixie/log.hpp>
#include <pixie/param.hpp>
#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace channel {
struct channel;
};
namespace module {
class module;
};
/**
 * @brief Collects Pixie-16 specific fixtures information.
 */
namespace fixture {
/**
 * Output handler for actions and tests
 */
template <typename T>
using output = std::function<void(const std::string&, T arg)>;

/**
 * Log level output
 */
using log_output = output<log::level>;

/**
 * @brief An assembly is an assemblyd part of a system
 *
 * An assembly is a PWA or printed wiring assembly that forms part of
 * a  system. An assembly may be formed by subassemblys. A module owns
 * all assembly fixures and is the root assembly.
 *
 * The things you can do in each event handler will depend on the
 * state of the assembly. For example you cannot use the DSP services
 * in the @ref dsp_loaded handler because the module is still not
 * formally online.
 */
struct assembly;
using assembly_ptr = std::shared_ptr<assembly>;
using assembly_ptrs = std::vector<assembly_ptr>;
struct assembly {
    /**
     * Label of the assembly
     */
    std::string label;

    /**
     * The module that has the assemblies.
     */
    pixie::module::module& module_;

    /**
     * The assemblys this module hosts
     */
    assembly_ptrs subassemblies;

    assembly(pixie::module::module& module_, std::string label = "integrated");
    virtual ~assembly();

    /**
     * Get the MIB path for this assembly
     */
    virtual std::string get_mib_base();

    /**
     * Open the assembly. Called once the module is opened. Do not
     * touch the hardware. There are no channels and no variables.
     */
    virtual void open();

    /**
     * Close the assembly. Called before the module is forced offline and
     * closed.
     */
    virtual void close();

    /**
     * Initialize the module.
     */
    virtual void initialize();

    /**
     * The module is online.
     */
    virtual void online();

    /**
     * The module has been forced online.
     */
    virtual void forced_online();

    /**
     * The module has been forced offline.
     */
    virtual void forced_offline();

    /**
     * The COMMS FPGA has been loaded. Only this FPGA is loaded.
     */
    virtual void fpga_comms_loaded();

    /**
     * The FIPPI FPGAs have been loaded. Only assume the COMMS and FIPPI FPGAs
     * are loaded.
     */
    virtual void fpga_fippi_loaded();

    /**
     * The DSP has been loaded and can be accessed. The COMMS and FIPPI FPGAs
     * are also loaded.
     */
    virtual void dsp_loaded();

    /**
     * The module have been booted and is online.
     */
    virtual void boot();

    /**
     * The module values will be erased
     */
    virtual void erase_values();

    /**
     * The module values have been initialized
     */
    virtual void init_values();

    /**
     * The module channels will be erased
     */
    virtual void erase_channels();

    /**
     * The module has been initialised, initialise the subassemblies.
     */
    virtual void init_assemblies();

    /**
     * The module channels have been initialized
     */
    virtual void init_channels();

    /**
     * Hardware synchronisation
     */
    virtual void sync_hw();

    /**
     * Variable sychronisation
     */
    virtual void sync_vars(const param::sync_mode sync_mode);

    /*
     * Fixture control task support
     */

    /**
     * Set DACs.
     */
    virtual void set_dacs();

    /**
     * Get ADC traces.
     */
    virtual void get_traces();

    /**
     * Adjust offsets
     */
    virtual void adjust_offsets();

    /**
     * Tau Finder
     */
    virtual void tau_finder();

    /*
     * Tests
     */
    virtual bool has_test(const std::string test);
    virtual void run_test(
        const std::string test, bool& result, log_output& out);
};

/**
 * @brief The object representing the channel fixture that is part of
 *        an assembly
 *
 * Channels can be part of the motherboard of a module or they can be on a
 * separate PCB and the module is an assembly of boards or fixtures.
 *
 * The nature of the supported fixtures can vary.
 */
struct channel {
    /**
     * The module's channel that exists on this type of fixture.
     */
    pixie::channel::channel& module_channel;

    /**
     * The hardware configuration.
     */
    assembly& assembly_;

    /**
     * The hardware configuration.
     */
    hw::config config;

    /**
     * The channel's label
     */
    std::string label;

    channel(
        pixie::channel::channel& module_channel, assembly& assembly_,
        const hw::config& config);
    virtual ~channel();

    /**
     * Open the channel fixture. Configure and set up.
     */
    virtual void open();

    /**
     * Close the channel fixture. Called by the default destructor.
     */
    virtual void close();

    /**
     * The channel is online.
     */
    virtual void online();

    /**
     * The channel has been forced online.
     */
    virtual void forced_online();

    /**
     * The channel has been forced offline.
     */
    virtual void forced_offline();

    /**
     * Per channel DAC set up.
     */
    virtual void set_dac(param::value_type value);

    /**
     * Per channel offsetdac adjust
     */
    virtual void adjust_offsetdac();

    /**
     * Per channel ADC acquire.
     */
    virtual void acquire_adc();

    /**
     * Per channel ADC acquire.
     */
    virtual void read_adc(hw::adc_word* buffer, size_t size);

    /**
     * An event
     *
     * Overload if required
     */
    virtual void event(const std::string name);

    /**
     * Set a value.
     *
     * Overload the types required. Integers are all unsigned and the
     * template provides conversion for fixture users only.
     */
    virtual void set(const std::string item, bool value);
    virtual void set(const std::string item, unsigned int value);
    virtual void set(const std::string item, double value);

    /**
     * Get a value.
     *
     * Overload the types required. Integers are all unsigned and the
     * template provides conversion for fixture users only.
     */
    virtual void get(const std::string item, bool& value);
    virtual void get(const std::string item, unsigned int& value);
    virtual void get(const std::string item, double& value);

    /**
     * Integter types
     */
    template<typename T,
             std::enable_if_t<!std::is_same_v<T, bool> &&
                              std::is_integral_v<T> &&
                              std::is_arithmetic_v<T>, bool> = true>
    void set(const std::string item, T value) {
        set(item, static_cast<unsigned int>(value));
    }
    template<typename T,
             std::enable_if_t<!std::is_same_v<T, bool> &&
                              std::is_integral_v<T> &&
                              std::is_arithmetic_v<T>, bool> = true>
    void get(const std::string item, T& value) {
        unsigned int uvalue;
        get(item, uvalue);
        value = static_cast<T>(uvalue);
    }

    /**
     * Get the module.
     */
    module::module& get_module();

    /**
     * Get a persistent key for the channel fixture
     */
    const std::string persistent_key(const std::string& item) const;
};

using channel_ptr = std::shared_ptr<channel>;

/**
 * Make a channel fixture given a hardware configuration
 */
channel_ptr make(pixie::channel::channel& module_channel, const hw::config& config);

/**
 * Make a module fixtures given a hardware configuration
 */
assembly_ptr make(pixie::module::module& module_);

}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_FIXTURE_H
