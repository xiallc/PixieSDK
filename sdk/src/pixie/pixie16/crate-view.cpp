
/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2023 XIA LLC, All rights reserved.
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

/** @file crate-views.cpp
 * @brief Implements functions and data structures related to handling a view of
 *        the Pixie-16 crate.
 */

#include <sstream>

#include <pixie/log.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/crate-view.hpp>

namespace xia {
namespace pixie {
namespace crate {
namespace view {

module::module() : phys_crate(nullptr), num_modules(0) {
}

module::module(crate& phys_crate_) : phys_crate(nullptr), num_modules(0) {
    set(phys_crate_);
}

void module::set(crate& phys_crate_) {
    lock_guard guard(lock_);
    crate::crate_event_func crate_call =
        [self = this](crate::crate_event event) {
            self->crate_event(event);
        };
    crate::slot_event_func slot_call =
        [self = this](hw::slot_type slot, crate::slot_event event) {
            self->slot_event(slot, event);
        };
    if (phys_crate != nullptr) {
        phys_crate->event_funcs_clear();
    }
    phys_crate = &phys_crate_;
    phys_crate->crate_event_func_set(crate_call);
    phys_crate->slot_event_func_set(slot_call);
}

void module::assign(const pixie::module::number_slots& numbers) {
    lock_guard guard(lock_);
    check();
    crate& pcrate = *phys_crate;
    std::ostringstream oss;
    for (auto& number_slot : numbers) {
        oss << number_slot.second << "->" << number_slot.first << ' ';
    }
    xia_log(log::info) << "assign slot: requested map: " << oss.str();
    /*
     * Sort the modules table based on the slot mapping provided. For modules in
     * the number table provided by the user:
     *
     *  - Move modules that exist in the modules table to a new slot
     *    provided in numbers
     *  - Modules in the modules table but not in the numbers table are closed.
     *  - Modules in the number table and not in the modules table are open.
     *
     * Note: Referenced slots need to contain a module. A module that
     *       is present and does not have a valid slot number may not
     *       have been opened and opening it may cause it to move
     *       slots. We attempt to open it and then check once the
     *       module is open. The case of a slot being empty may be
     *       caused by a module not have been opened.
     */
    for (auto& number_slot : numbers) {
        if (number_slot.first > numbers.size()) {
            xia_log(log::error) << "module-crate: assign: module numbers must be continuous: "
                                << number_slot.first;
            throw error(error::code::module_number_invalid,
                        "module asignment numbers are not continuous");
        }
        if (number_slot.second > phys_crate->num_slots) {
            xia_log(log::error) << "module-crate: assign: invalid slot: "
                                << number_slot.second;
            throw error(error::code::module_number_invalid,
                        "module asignment has invalid slot numbers");
        }
        if (!pcrate[number_slot.second].present()) {
            xia_log(log::error) << "module-crate: assign: slot empty: "
                                << number_slot.second;
            throw error(error::code::module_number_invalid,
                        "module asignment refences empty slot: " +
                        std::to_string(number_slot.second));
        }
    }
    hw::slot_numbers current_modules = modules;
    modules.clear();
    modules.resize(numbers.size());
    num_modules = 0;
    for (auto& number_slot : numbers) {
        modules[number_slot.first] = number_slot.second;
        auto& module = pcrate[number_slot.second];
        module.number = int(num_modules);
        num_modules++;
    }
    for (auto slot : modules) {
        auto mod = std::find_if(current_modules.begin(), current_modules.end(),
                                [&slot](const hw::slot_type s) {
                                    return s == slot; });
        /*
         * If the slot is not currently held open it.
         */
        if (mod == current_modules.end()) {
            auto& module = pcrate[slot];
            if (!module.opened()) {
                module.open();
                if (module.slot != slot) {
                    xia_log(log::error) << "module-crate: assign: slot moved on open: "
                                        << slot;
                    throw error(error::code::module_number_invalid,
                                "module asignment moved slot with open: " +
                                std::to_string(slot));
                }
            }
            auto fw_tag = module.get_fw_tag();
            if (!firmware::check(pcrate.firmware, fw_tag)) {
                throw error(error::code::module_invalid_firmware, "firmware not found: " + fw_tag);
            }
            firmware::firmware_set fw_set;
            firmware::find_filter filter(fw_tag, module.slot);
            firmware::find(fw_set, pcrate.firmware, filter);
            module.probe(fw_set);
        }
    }
    for (auto slot : current_modules) {
        auto mod = std::find_if(modules.begin(), modules.end(),
                                [&slot](const hw::slot_type s) {
                                    return s == slot; });
        /*
         * If the slot is not currently held open it.
         */
        if (mod == modules.end()) {
            auto& module = pcrate[slot];
            if (module.opened()) {
                module.close();
            }
        }
    }
    pcrate.backplane_reinit();
}

void module::crate_event(crate::crate_event event) {
    xia_log(log::debug) << "module-crate: event: crate-event="
                        << crate_event_label(event);
    lock_guard guard(lock_);
    check();
    switch (event) {
    case crate::crate_event::initialize_begin:
    case crate::crate_event::initialize_end:
    case crate::crate_event::probe_begin:
    case crate::crate_event::probe_end:
    case crate::crate_event::shutdown_begin:
    case crate::crate_event::shutdown_end:
    case crate::crate_event::boot_begin:
    case crate::crate_event::boot_end:
    case crate::crate_event::import_config_begin:
    case crate::crate_event::import_config_end:
    case crate::crate_event::export_config_begin:
    case crate::crate_event::export_config_end:
        break;
    }
}

void module::slot_event(hw::slot_type slot, crate::slot_event event) {
    xia_log(log::debug) << "module-crate: event: slot=" << slot
                        << " slot-event=" << slot_event_label(event);
    lock_guard guard(lock_);
    check();
    switch (event) {
    case crate::slot_event::online:
    case crate::slot_event::offline:
        module_discovered(slot);
        break;
    case crate::slot_event::present:
    case crate::slot_event::not_present:
    case crate::slot_event::boot:
    default:
        break;
    }
}

void module::module_discovered(hw::slot_type slot) {
    xia_log(log::info) << "module-crate: add module: " << slot;
    lock_guard guard(lock_);
    check();
    auto mi = std::find_if(modules.begin(), modules.end(),
                           [slot](const module_num& mod_slot) {
                               return slot == mod_slot;
                           });
    if (mi == modules.end()) {
        modules.push_back(slot);
        crate& pcrate = *phys_crate;
        auto& module = pcrate[slot];
        module.number = int(num_modules);
        num_modules++;
    }
}

void module::format_module_map(std::string& str) {
    if (modules.empty()) {
        str = "no modules assigned";
    } else {
        std::ostringstream oss;
        for (size_t m = 0; m < modules.size(); ++m) {
            oss << modules[m] << "->" << m << ' ';
        }
        str = oss.str();
    }
}

std::string module::format_module_map() {
    std::string str;
    format_module_map(str);
    return str;
}

slot_module::slot_module() : mod_crate(nullptr), using_slots(true) {
}

slot_module::slot_module(module& wrapped_module, bool logical_map) :
    mod_crate(nullptr), using_slots(true) {
    using_slots = !logical_map;
    set_module(wrapped_module);
}

void slot_module::set_module(module& wrapped_module) {
    lock_guard guard(lock_);
    mod_crate = &wrapped_module;
}

size_t slot_module::get_num_modules(void) {
    return mod_crate->num_modules;
}

slot_module::module_map slot_module::get_modules(void) {
    if (using_slots) {
        return mod_crate->modules;
    } else {
        slot_module::module_map map;
        for (size_t i = 0; i < get_num_modules(); i++) {
            map.push_back(i);
        }
        return map;
    }
}

void slot_module::set(crate& phys_crate) {
    mod_crate->set(phys_crate);
}

void slot_module::assign(const pixie::module::number_slots& numbers) {
    mod_crate->assign(numbers);
}

void slot_module::format_module_map(std::string& str) {
    mod_crate->format_module_map(str);
}

std::string slot_module::format_module_map() {
    return mod_crate->format_module_map();
}
} // namespace views
} // namespace crate
} // namespace pixie
} // namespace xia
