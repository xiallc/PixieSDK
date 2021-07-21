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

/** @file crate.hpp
 * @brief Defines functions and data structures related to handling a Pixie-16 crate.
 */

#ifndef PIXIE_CRATE_H
#define PIXIE_CRATE_H

#include <atomic>

#include <pixie/error.hpp>
#include <pixie/fw.hpp>
#include <pixie/os_compat.hpp>

#include <pixie/pixie16/fpga.hpp>
#include <pixie/pixie16/module.hpp>


namespace xia {
namespace pixie {
namespace crate {
/*
 * Crate errors
 */
typedef pixie::error::error error;

/*
 * Crate
 *
 * A crate is a series of slots that contain modules.
 */
struct crate {
private:
    /*
     * Module lock
     */
    using lock_type = std::recursive_mutex;
    using lock_guard = std::lock_guard<lock_type>;

public:
    /*
     * Module lock guard
     */
    class guard {
        lock_type& lock_;
        lock_guard guard_;

    public:
        guard(crate& crate_);
        ~guard() = default;
    };

    /*
     * Hold an instance while using a module.
     */
    class user {
        crate& crate_;

    public:
        user(crate& crate_);
        ~user();
    };

    /*
     * Number of modules present in the crate.
     */
    size_t num_modules;

    /*
     * Crate revision
     */
    int revision;

    /*
     * A crate contains a number of modules in slots. These modules are
     * online and ready for use.
     */
    module::modules modules;

    /*
     * The offline modules have been detected by are not online. This lets
     * a user check a crate and determine the state of all detected
     * modules.
     */
    module::modules offline;

    /*
     * Firmware for the crate. Check the modules for the ones they have
     * loaded.
     */
    firmware::crate firmware;

    crate();
    virtual ~crate();

    /*
     * Check the crate has been intialised and ready for use. Throws an
     * error if not ready.
     */
    void ready();

    /*
     * Check if the crate is busy?
     */
    bool busy() const;

    /*
     * How many active users are in the crate when this call is made?
     */
    int users() const;

    /*
     * Range checking operator to index modules based on various index
     * types.
     */
    template<typename T>
    module::module& operator[](T number) {
        size_t number_ = static_cast<size_t>(number);
        if (number_ >= num_modules) {
            throw error(pixie::error::code::module_number_invalid,
                        "module number out of range: " + std::to_string(number));
        }
        return *(modules[number_]);
    }

    /*
     * Return a module indexed by a slot.
     */
    template<typename T>
    module::module_ptr find(T slot) {
        int slot_ = static_cast<int>(slot);
        auto mod = std::find_if(modules.begin(), modules.end(),
                                [slot_](const module::module_ptr m) { return m->slot == slot_; });
        if (mod == modules.end()) {
            mod = std::find_if(offline.begin(), offline.end(),
                               [slot_](const module::module_ptr m) { return m->slot == slot_; });
            if (mod == offline.end()) {
                throw error(pixie::error::code::module_number_invalid, "module slot not found");
            }
        }
        return *mod;
    }

    /*
     * Initialise the crate and get it ready.
     */
    void initialize(bool reg_trace = false);

    /*
     * Mark a module as offline and move to the offline module list. This
     * invalidates any iterators you may hold to the modules and offline
     * containers.
     */
    void set_offline(module::module_ptr module);

    /*
     * Probe the modules.
     */
    void probe();

    /*
     * Boot all modules.
     */
    void boot();

    /*
     * Assign numbers to the modules by slot. Modules not in the map are
     * forced offline. You can optionally stop this happening but the
     * number for the modules not in the map will be invalid.
     */
    void assign(const module::number_slots& numbers, bool force_offline = true);

    /*
     * Set the firmwares into the modules in the crate.
     */
    void set_firmware();

    /*
     * Import a configuration. Returning a list of loaded modules.
     */
    void import_config(const std::string json_file, module::number_slots& loaded);

    /*
     * Export the configuration
     */
    void export_config(const std::string json_file);

    /*
     * Move offline modules from the online list to offline.
     */
    void move_offlines();

    /*
     * Output the crate details.
     */
    void output(std::ostream& out) const;

protected:
    virtual void add_module();

private:
    /*
     * Check the module slots.
     */
    void check_slots();

    /*
     * Check the crate revisions all match.
     */
    void check_revision();

    /*
     * Crate lock
     */
    lock_type lock_;

    /*
     * Crate ready.
     */
    std::atomic_bool ready_;

    /*
     * Number of active users in the
     */
    std::atomic_int users_;
};

/*
 * Module handle. This allows you to access a module during while operating
 * on it. The crate as a user register and the module is locked while this
 * object exists.
 */
struct module_handle {
    module_handle(crate& crate_, size_t number);
    module_handle(crate& crate_, unsigned short number);
    ~module_handle() = default;

    module::module& operator*() {
        return handle;
    }
    module::module* operator->() {
        return &handle;
    }

private:
    module::module& handle;
    crate::user user;
    module::module::guard guard;
};
}  // namespace crate
}  // namespace pixie
}  // namespace xia

/*
 * Output stream operator.
 */
std::ostream& operator<<(std::ostream& out, const xia::pixie::crate::crate& crate);

#endif  // PIXIE_CRATE_H
