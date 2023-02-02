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

/** @file crate.cpp
 * @brief Implements functions and data structures related to handling a Pixie-16 crate.
 */

#include <algorithm>
#include <future>
#include <iomanip>
#include <numeric>
#include <sstream>

#include <pixie/config.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/backplane.hpp>
#include <pixie/pixie16/crate.hpp>

namespace xia {
namespace pixie {
namespace crate {
crate::guard::guard(crate& crate_) : lock_(crate_.lock_), guard_(lock_) {}

crate::user::user(crate& crate__) : crate_(crate__) {
    ++crate_.users_;
}

crate::user::~user() {
    --crate_.users_;
}

crate::boot_params::boot_params()
    : force(true), boot_comms(true), boot_fippi(true), boot_dsp(true) {
}

crate::crate() :
    revision(-1), num_slots(0), num_present(0), num_online(0), num_offline(0),
    ready_(false), users_(0) {
    event_funcs_clear();
}

crate::~crate() {}

void crate::ready() {
    if (!ready_.load()) {
        throw error(pixie::error::code::crate_not_ready, "crate is not ready");
    }
}

bool crate::busy() const {
    return users_.load() > 0;
}

int crate::users() const {
    return users_.load();
}

void crate::initialize(bool reg_trace) {
    xia_log(log::info) << "crate: initialise";

    /*
     * Set ready to true and if there is an issue return it to false.
     */
    if (ready_.exchange(true)) {
        throw error(pixie::error::code::crate_already_open, "create already initialised");
    }

    lock_guard guard(lock_);

    crate_event_call(crate_event::initialize_begin);

    create_module_slots();

    try {
        /*
         * The device probe collects the modules present and
         * visiable. We move the modules to their slot when the scan
         * has completed.
         */
        for (size_t device_number = 0; device_number < hw::max_slots; ++device_number) {
            module::module& module = *slots[device_number];

            try {
                module.module_var_descriptors =
                    param::module_var_descs(param::get_module_var_descriptors());
                module.channel_var_descriptors =
                    param::channel_var_descs(param::get_channel_var_descriptors());
                module.reg_trace = reg_trace;
                module.open(device_number);
            } catch (pixie::error::error& e) {
                xia_log(log::error) << "module: device " << device_number << ": error: " << e.what();
            }

            /*
             * Have all modules been found?
             */
            if (!module.present()) {
                break;
            }

            ++num_present;
        }

        move_modules_to_slots();

        check_revision();
        check_slots();

        for (size_t slot = 0; slot < num_slots; ++slot) {
            module::module& module = *slots[slot];
            if (module.present()) {
                slot_event_call(slot, slot_event::present);
                if (module.opened()) {
                    slot_event_call(slot, slot_event::open);
                    if (module.online()) {
                        xia_log(log::info) << "slot: device " << module.get_bus_device_number()
                                           << ": slot:" << module.slot
                                           << " serial-number:" << module.serial_num
                                           << " version:" << module.version_label();
                        ++num_online;
                        slot_event_call(slot, slot_event::online);
                    } else {
                        xia_log(log::info) << "slot: offline: device "
                                           << module.get_bus_device_number()
                                           << ": slot: " << module.slot;
                        ++num_offline;
                        slot_event_call(slot, slot_event::offline);
                    }
                } else {
                    slot_event_call(slot, slot_event::close);
                }
            } else {
                slot_event_call(slot, slot_event::not_present);
            }
        }

        backplane.init(hw::max_slots, num_present);
    } catch (...) {
        ready_ = false;
        throw;
    }

    crate_event_call(crate_event::initialize_end);
}

void crate::shutdown() {
    xia_log(log::info) << "crate: shutdown";
    lock_guard guard(lock_);
    error::code first_error = error::code::success;
    crate_event_call(crate_event::shutdown_begin);
    for (auto& module : slots) {
        try {
            module->close();
            if (num_online > 0) {
                --num_online;
            }
            ++num_offline;
        } catch (error& e) {
            first_error = e.type;
        }
    }
    crate_event_call(crate_event::shutdown_end);
    if (num_online > 0) {
        throw error(error::code::module_total_invalid, "crate shutdown online count not 0");
    }
    if (num_offline != num_present) {
        throw error(
            error::code::module_total_invalid, "crate shutdown offline count not present count");
    }
    ready_ = false;
    if (first_error != error::code::success) {
        throw error(first_error, "crate shutdown error; see log");
    }
}

void crate::set_online(const hw::slot_type slot) {
    lock_guard guard(lock_);
    (*this)[slot];
    set_online(*slots[slot]);
}

void crate::set_online(module::module& module) {
    if (!module.online()) {
        xia_log(log::info) << "crate: set online: slot=" << module.slot;
        lock_guard guard(lock_);
        module.force_online();
        ++num_online;
        --num_offline;
        slot_event_call(module.slot, slot_event::online);
        return;
    }
    throw error(error::code::module_not_found, "module not seen as offline");
}

void crate::set_offline(const hw::slot_type slot) {
    lock_guard guard(lock_);
    (*this)[slot];
    set_offline(*slots[slot]);
}

void crate::set_offline(module::module& module) {
    if (module.online()) {
        xia_log(log::info) << "crate: set offline: slot=" << module.slot;
        lock_guard guard(lock_);
        module.force_offline();
        if (num_online > 0) {
            --num_online;
        }
        ++num_offline;
        slot_event_call(module.slot, slot_event::offline);
        return;
    }
    throw error(error::code::module_not_found, "module not seen as online");
}

bool crate::probe() {
    xia_log(log::info) << "crate: probe: " << slots.size();
    ready();
    lock_guard guard(lock_);
    num_present = 0;
    num_online = 0;
    num_offline = 0;
    crate_event_call(crate_event::probe_begin);
    for (auto& module : slots) {
        xia_log(log::debug) << "crate: probe: " << *module;
        if (module->present()) {
            ++num_present;
            slot_event_call(module->slot, slot_event::present);
            if (module->opened()) {
                module->probe();
                if (module->online()) {
                    ++num_online;
                    slot_event_call(module->slot, slot_event::online);
                } else {
                    ++num_offline;
                    slot_event_call(module->slot, slot_event::offline);
                }
            }
        }
    }
    backplane_reinit();
    crate_event_call(crate_event::probe_end);
    return num_online == num_present;
}

int crate::count_present() {
    int num = 0;
    for (auto& module : slots) {
        if (module->present()) {
            ++num;
        }
    }
    return num;
}

void crate::boot(const crate::boot_params& params) {
    xia_log(log::info) << "crate: boot: force=" << std::boolalpha << params.force
                       << " comms=" << params.boot_comms << " fippi=" << params.boot_fippi
                       << " dsp=" << params.boot_dsp;

    crate::boot_params::range slot_nums;

    if (params.slots.empty()) {
        slot_nums.resize(num_slots);
        std::iota(slot_nums.begin(), slot_nums.end(), 0);
    } else {
        for (auto slot_num : params.slots) {
            if (slot_num >= num_slots) {
                throw error(
                    error::code::module_number_invalid, "invalid boot slot number");
            }
        }
        slot_nums = params.slots;
    }

    ready();
    lock_guard guard(lock_);

    module::module_states state;
    capture_state(*this, state);

    crate_event_call(crate_event::boot_begin);

    typedef std::promise<error::code> promise_error;
    typedef std::future<error::code> future_error;

    std::vector<promise_error> promises(num_slots);
    std::vector<future_error> futures;
    std::vector<std::thread> threads;

    for (auto slot : slot_nums) {
        auto module = slots[slot];
        if (!module->opened() || module->revision == 0 ||
            (!params.force && module->online())) {
            continue;
        }
        slot_event_call(module->slot, slot_event::boot);
        futures.push_back(future_error(promises[slot].get_future()));
        threads.push_back(std::thread([slot, &params, &promises, module] {
            try {
                module->boot(params.boot_comms, params.boot_fippi, params.boot_dsp);
                promises[slot].set_value(error::code::success);
            } catch (pixie::error::error& e) {
                promises[slot].set_value(e.type);
            } catch (...) {
                try {
                    promises[slot].set_exception(std::current_exception());
                } catch (...) {
                    /* nothing */
                }
            }
        }));
    }

    error::code first_error = error::code::success;

    for (size_t t = 0; t < threads.size(); ++t) {
        error::code e = futures[t].get();
        if (first_error == error::code::success) {
            first_error = e;
        }
        threads[t].join();
    }

    backplane_reinit();

    for (size_t slot = 0; slot < num_slots; ++slot) {
        auto& module = slots[slot];
        if (state[slot].present && module->online() != state[slot].online) {
            if (state[slot].online) {
                slot_event_call(slot, slot_event::offline);
                ++num_offline;
                --num_online;
            } else {
                slot_event_call(slot, slot_event::online);
                ++num_online;
                --num_offline;
            }
        }
    }

    crate_event_call(crate_event::boot_end);

    if (first_error != error::code::success) {
        throw error(first_error, "crate boot error; see log");
    }
}

void crate::set_firmware() {
    xia_log(log::info) << "crate: set firmware";
    ready();
    lock_guard guard(lock_);
    for (auto& module : slots) {
        if (module->opened()) {
            for (auto& config : module->eeprom.configs) {
                std::string tag = firmware::tag(module->revision, config.adc_msps, config.adc_bits);
                auto mod_fw = firmware.find(tag);
                if (mod_fw != firmware.end()) {
                    module->add(firmware[tag]);
                } else {
                    if (firmware::revision_tag_check(tag, module->revision, config, firmware)) {;
                        module->add(firmware[tag]);
                    } else {
                        xia_log(log::warning) << module::module_label(*module)
                                              << "crate: module firmware not found: " << tag;
                    }
                }
            }
            if (module->firmware.size() == 0) {
                xia_log(log::warning) << module::module_label(*module) << "no firmware set";
            }
        }
    }
}

void crate::import_config(const std::string json_file, module::number_slots& loaded) {
    xia_log(log::info) << "crate: import configuration";
    ready();
    lock_guard guard(lock_);
    crate_event_call(crate_event::import_config_begin);
    loaded.clear();
    config::import_json(json_file, *this, loaded);
    for (auto& module : slots) {
        if (module->online()) {
            module->sync_vars();
        }
    }
    backplane_reinit();
    crate_event_call(crate_event::import_config_end);
}

void crate::initialize_afe() {
    xia_log(log::info) << "crate: initializing analog front-end";

    ready();
    lock_guard guard(lock_);

    typedef std::promise<error::code> promise_error;
    typedef std::future<error::code> future_error;

    std::vector<promise_error> promises(num_slots);
    std::vector<future_error> futures;
    std::vector<std::thread> threads;

    for (size_t slot = 0; slot < num_slots; ++slot) {
        auto module = slots[slot];
        if (!module->online()) {
            continue;
        }
        futures.push_back(future_error(promises[slot].get_future()));
        threads.push_back(std::thread([slot, &promises, module] {
            try {
                module->sync_hw();
                promises[slot].set_value(error::code::success);
            } catch (pixie::error::error& e) {
                promises[slot].set_value(e.type);
            } catch (...) {
                try {
                    promises[slot].set_exception(std::current_exception());
                } catch (...) {
                    /* nothing */
                }
            }
        }));
    }

    error::code first_error = error::code::success;

    for (size_t t = 0; t < threads.size(); ++t) {
        error::code e = futures[t].get();
        if (first_error == error::code::success) {
            first_error = e;
        }
        threads[t].join();
    }

    if (first_error != error::code::success) {
        throw error(first_error, "crate AFE intialize error; see log");
    }
}

void crate::backplane_reinit() {
    backplane.reinit(slots, num_present);
}

void crate::export_config(const std::string json_file) {
    xia_log(log::info) << "crate: export configuration";
    lock_guard guard(lock_);
    crate_event_call(crate_event::export_config_begin);
    config::export_json(json_file, *this);
    crate_event_call(crate_event::export_config_end);
}

void crate::output(std::ostream& out) const {
    if (!ready_.load()) {
        out << "not initialized";
        return;
    }
    out << "fw: tags: " << firmware.size() << std::endl;
    int c = 0;
    for (auto fw_rev : firmware) {
        for (auto& fw : std::get<1>(fw_rev)) {
            out << ' ' << std::setw(3) << ++c << ". " << ' ' << *fw << std::endl;
        }
    }
    out << "slots:" << num_slots << " present: " << num_present
        << " online: " << num_online << " offline: " << num_offline  << std::endl;
    bool first = true;
    for (auto& mod : slots) {
        if (mod->present()) {
            if (first) {
                first = false;
            } else {
                out << std::endl;
            }
            out << ' ' << *mod;
        }
    }
}

void crate::report(std::ostream& out) const {
    for (auto& mod : slots) {
        if (mod->present()) {
            mod->report(out);
            out << std::endl;
        }
    }
}

void crate::slot_event_func_set(slot_event_func& event_func) {
    lock_guard guard(lock_);
    slot_event_call = event_func;
}

void crate::crate_event_func_set(crate_event_func& event_func) {
    lock_guard guard(lock_);
    crate_event_call = event_func;
}

void crate::event_funcs_clear() {
    lock_guard guard(lock_);
    crate_event_call = [self = this](crate_event event) {
        xia_log(log::debug) << "crate: event: crate-event="
                            << crate_event_label(event);
    };
    slot_event_call = [self = this](hw::slot_type slot, slot_event event) {
        xia_log(log::debug) << "crate: event: slot=" << slot << " slot-event="
                            << slot_event_label(event);
    };
}

void crate::create_module_slots() {
    num_slots = hw::max_slots;
    for (size_t s = 0; s < num_slots; ++s) {
        slots.push_back(std::make_shared<module::module>(backplane));
    }
}

void crate::move_modules_to_slots() {
    /*
     * Move the modules to their respective slot in the slots table
     */
    for (hw::slot_type slot = 0; slot < num_slots; ++slot) {
        bool found = false;
        /*
         * Check the remaining slots to see if the slot number can be
         * found. If found and it is not the current slot swap it.
         */
        for (hw::slot_type check_slot = slot; check_slot < num_slots; ++check_slot) {
            if (slots[check_slot]->slot == slot) {
                if (slot != check_slot) {
                    std::swap(slots[slot], slots[check_slot]);
                }
                found = true;
                break;
            }
        }
        /*
         * If not found check if the module in the slot is present and
         * if present swap it for a module that is not present.
         */
        if (!found) {
            if (slots[slot]->slot_valid()) {
                for (hw::slot_type check_slot = slot + 1;
                     check_slot < num_slots;
                     ++check_slot) {
                    if (!slots[check_slot]->slot_valid()) {
                        std::swap(slots[slot], slots[check_slot]);
                        break;
                    }
                }
            }
        }
    }
}

void crate::check_slots() {
    using duplicate = std::pair<module::module_ptr, module::module_ptr>;
    using duplicates = std::vector<duplicate>;

    struct find_dups {
        duplicates dups;

        void check(module::module_ptr& mod, module::modules& mods) {
            if (mod->opened()) {
                for (auto compare : mods) {
                    if (compare->opened() && mod != compare) {
                        if (mod->slot == compare->slot) {
                            auto di = std::find_if(dups.begin(), dups.end(),
                                                   [&mod](const duplicate& dup) {
                                                       auto dmod = std::get<0>(dup);
                                                       return mod->slot == dmod->slot;
                                                   });
                            if (di == dups.end()) {
                                dups.push_back(duplicate(mod, compare));
                            }
                        }
                    }
                }
            }
        }
    };

    find_dups dups;

    for (auto module : slots) {
        dups.check(module, slots);
    }

    for (auto& dup : dups.dups) {
        auto mod1 = std::get<0>(dup);
        auto mod2 = std::get<1>(dup);
        if (revision == 2) {
            if (mod1->slot == 5 && mod1->pci_bus() == mod2->pci_bus()) {
                if (mod1->pci_slot() == 13) {
                    mod1->slot = 4;
                } else if (mod2->pci_slot() == 13) {
                    mod2->slot = 4;
                }
            }
        }
        if (mod1->slot == mod2->slot) {
            xia_log(log::error) << "crate: duplicate slot: " << mod1->slot
                                << " 1:pci=" << mod1->pci_bus() << ':' << mod1->pci_slot()
                                << " 2:pci=" << mod2->pci_bus() << ':' << mod2->pci_slot();
        }
    }
}

void crate::check_revision() {
    revision = -1;
    for (auto module : slots) {
        if (module->opened()) {
            if (revision < 0) {
                revision = module->crate_revision;
                xia_log(log::info) << "crate: crate revision: " << revision;
            } else if (revision != module->crate_revision) {
                xia_log(log::warning) << "crate: crate revision mismatch: " << module->crate_revision
                                      << " module slot=" << module->slot;
            }
        }
    }
}

module_crate::module_crate() : phys_crate(nullptr), num_modules(0) {
}

module_crate::module_crate(crate& phys_crate_) : phys_crate(nullptr), num_modules(0) {
    set(phys_crate_);
}

void module_crate::set(crate& phys_crate_) {
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

void module_crate::assign(const module::number_slots& numbers) {
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
            module.probe();
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

void module_crate::crate_event(crate::crate_event event) {
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

void module_crate::slot_event(hw::slot_type slot, crate::slot_event event) {
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

void module_crate::module_discovered(hw::slot_type slot) {
    xia_log(log::error) << "module-crate: add module: " << slot;
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

void module_crate::format_module_map(std::string& str) {
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

std::string module_crate::format_module_map() {
    std::string str;
    format_module_map(str);
    return str;
}

void capture_state(crate& crate, module::module_states& states) {
    states.clear();
    states.reserve(crate.num_slots);
    for (hw::slot_type slot = 0; slot < crate.num_slots; ++slot) {
        states.emplace_back(module::module_state(crate[slot]));
    }
}

const char* crate_event_label(crate::crate_event event) {
    static const char* label[] = {
        "initialize::begin",
        "initialize::end",
        "shutdown::begin",
        "shutdown::end",
        "probe::begin",
        "probe::end",
        "boot::begin",
        "boot::end",
        "import-config::begin",
        "import-config::end",
        "export-config::begin",
        "export-config::end"
    };
    static const size_t labels = sizeof(label) / sizeof(label[0]);
    const size_t event_num = static_cast<size_t>(event);
    if (event_num < labels) {
        return label[event_num];
    }
    return "invalid crate event";
}

const char* slot_event_label(crate::slot_event event) {
    static const char* label[] = {
        "present",
        "not-present",
        "open",
        "close",
        "online",
        "offline",
        "boot"
    };
    static const size_t labels = sizeof(label) / sizeof(label[0]);
    const size_t event_num = static_cast<size_t>(event);
    if (event_num < labels) {
        return label[event_num];
    }
    return "invalid module event";
}
};  // namespace crate
};  // namespace pixie
};  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::crate::crate& crate) {
    crate.output(out);
    return out;
}
