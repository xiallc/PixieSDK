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
 * @brief The object representing the channel fixture.
 *
 * Channels can be part of the motherboard of a module or thet can be on a
 * separate PCB and the module is an assemble of boards or fixtures.
 *
 * The nature of the supported fixtures can vary.
 */
struct channel {
    /**
     * The module's channel that exists on this type of fixture.
     */
    pixie::channel::channel& module_channel;

    std::string label;

    /**
     * The hardware configuration.
     */
    hw::config config;

    channel(pixie::channel::channel& module_channel, const hw::config& config);
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
     * Per channel DAC set up.
     */
    virtual void set_dac(param::value_type value);

    /**
     * Per channel ADC acquire.
     */
    virtual void acquire_adc();

    /**
     * Report the fixture's details
     */
    virtual void report(std::ostream& out) const;

    /**
     * Get the module.
     */
    module::module& get_module();
};

using channel_ptr = std::shared_ptr<channel>;

/**
 * Make a channel fixture given a hardware configuration
 */
channel_ptr make(pixie::channel::channel& module_channel, const hw::config& config);

}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_FIXTURE_H
