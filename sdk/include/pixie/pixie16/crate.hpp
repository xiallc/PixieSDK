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
#include <functional>

#include <pixie/error.hpp>
#include <pixie/fw.hpp>
#include <pixie/os_compat.hpp>

#include <pixie/pixie16/backplane.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Defines functions and data structures related to handling a Pixie-16 crate.
 */
namespace crate {
/*
 * Crate errors
 */
using error =  pixie::error::error;

/**
 * @brief A crate is a series of slots that contain modules.
 *
 * The crate object is a physical model of the crate hardware. It is
 * constructed with a module slot for each physical slot the crate
 * hardware has even if a module cannot reside in that slot.
 *
 * The physical model is mapped by slot id only.
 */
struct crate {
private:
    /**
     * Crate lock
     */
    using lock_type = std::recursive_mutex;
    using lock_guard = std::lock_guard<lock_type>;

public:
    /**
     * Crate events
     */
    enum struct crate_event {
        initialize_begin,
        initialize_end,
        shutdown_begin,
        shutdown_end,
        probe_begin,
        probe_end,
        boot_begin,
        boot_end,
        import_config_begin,
        import_config_end,
        export_config_begin,
        export_config_end
    };

    /**
     * Called when crate events happen. The crate lock maybe be held
     * so you could deadlock. The slots can be accessed.
     */
    using crate_event_func = std::function<void(crate_event event)>;

    /**
     * Slot events
     */
    enum struct slot_event {
        present,
        not_present,
        open,
        close,
        online,
        offline,
        boot
    };

    /**
     * Called when slot events happen. The crate lock maybe be held so
     * you could deadlock. The slots can be accessed.
     */
    using slot_event_func = std::function<void(hw::slot_type slot, slot_event event)>;

    /**
     * @brief Crate lock guard to prevent concurrent access to a slot.
     */
    class guard {
        lock_type& lock_;
        lock_guard guard_;

    public:
        guard(crate& crate_);
        ~guard() = default;
    };

    /**
     * @brief Hold an instance while a slot to in use.
     */
    class user {
        crate& crate_;

    public:
        user(crate& crate_);
        ~user();
    };

    /**
     * Boot settings.
     *
     * Defaults to all true and all slots.
     */
    struct boot_params {
        using range_type = size_t;
        using range = std::vector<range_type>;

        /**
         * If true (the default) boot all the slots else only boot the
         * offline slots.
         */
        bool force;
        bool boot_comms; /** Clear and load the COMM FPGA */
        bool boot_fippi; /** Clear and load the FIPPI FPGA */
        bool boot_dsp; /** Reset and load the DSP */
        range slots; /** Range of slots to boot, empty is all */

        boot_params();
    };

    /**
     * Crate revision
     */
    int revision;

    /**
     * Number of slots the crate has.
     */
    size_t num_slots;

    /**
     * Number of modules present in the crate.
     */
    size_t num_present;

    /**
     * Number of modules online in the crate.
     */
    size_t num_online;

    /**
     * Number of modules offline in the crate.
     */
    size_t num_offline;

    /**
     * The backplane
     */
    backplane::backplane backplane;

public:
    /**
     * Firmware for the crate.
     */
    firmware::crate firmware;

    crate();
    virtual ~crate();

    /**
     * Check the crate has been initialised and ready for use.
     * @throws pixie::error::code::crate_not_ready
     */
    void ready();

    /**
     * @brief Check if the crate is busy?
     * @return True if the crate is busy with another operation.
     */
    bool busy() const;

    /**
     * @brief Checks how many users are using the crate object.
     * @return The number of active users are in the crate when this call is made.
     */
    int users() const;

    /**
     * @brief Return a module indexed by a slot.
     * @tparam T The template for various ways to determine the slot.
     * @param slot The physical slot in the crate that we'd like to access.
     * @return A pointer to the module object associated with the physical slot.
     */
    template<typename T>
        module::module_ptr find(T slot) {
        hw::slot_type slot_ = static_cast<hw::slot_type>(slot);
        if (slot_ >= num_slots) {
            throw error(
                pixie::error::code::module_number_invalid, "module slot not found");
        }
        return slots[slot_];
    }

    /**
     * @brief Return a module indexed by a slot.
     * @tparam T The template for various ways to determine the slot.
     * @param slot The physical slot in the crate that we'd like to access.
     * @return A pointer to the module object associated with the physical slot.
     */
    template<typename T>
    module::module& operator[](T slot) {
        return *find(slot);
    }

    /**
     * @brief Initialise the crate and get it ready.
     * @param reg_trace When true enables enhanced diagnostic information to be output.
     */
    void initialize(bool reg_trace = false);

    /**
     * @brief Shutdown the crate and close all open modules.
     */
    void shutdown();

    /**
     * @brief Mark a slot as online.
     *
     * @param slot Slot number.
     */
    void set_online(const hw::slot_type slot);

    /**
     * @brief Mark a slot as offline.
     *
     * @param slot Slot number.
     */
    void set_offline(const hw::slot_type slot);

    /**
     * @brief Mark a module as online.
     *
     * @param module The module to be set online.
     */
    void set_online(module::module& module);

    /**
     * @brief Mark a module as offline.
     *
     * @param module The module to be set offline.
     */
    void set_offline(module::module& module);

    /**
     * @brief Checks if all the modules are online.
     *
     * @return True if all modules are online else false if a module is not online
     */
    bool probe();

    /**
     * @brief Boot all modules by loading the firmware onto the hardware.
     *
     * @param params The crate boot parameters
     */
    void boot(const boot_params& params = boot_params());

    /**
     * @brief Associates the firmware with modules in the crate.
     */
    void set_firmware();

    /**
     * @brief Import a configuration. Returning a list of loaded modules.
     * @param[in] json_file The path to the JSON configuration file to load.
     * @param[out] loaded The list of modules that received configurations.
     */
    void import_config(const std::string json_file, module::number_slots& loaded);

    /**
     * @brief Initializes the module's analog front end after importing a configuration.
     * @see xia::pixie::module::sync_hw
     * For each online module, the function executes the xia::pixie::module::sync_hw function with
     * the default values.
     */
    void initialize_afe();

    /**
     * @bief Reiunitialise the backplane. Call if the slots have
     *       changed state.
     */
    void backplane_reinit();

    /**
     * @brief Export the active module configurations to a file.
     * @param json_file Path to the file that will hold the configurations.
     */
    void export_config(const std::string json_file);

    /**
     * @brief Output the crate details.
     */
    void output(std::ostream& out) const;

    /**
     * @brief Set the crate event function.
     */
    void crate_event_func_set(crate_event_func& event_func);

    /**
     * @brief Set the slot event function.
     */
    void slot_event_func_set(slot_event_func& event_func);

    /**
     * @brief Clear the event functions.
     */
    void event_funcs_clear();

    int count_present();

protected:
    /**
     * A crate is a series of slots that contain modules. A module
     * maybe not present, online of offline.
     */
    module::modules slots;

    /**
     * Crate event function
     */
    crate_event_func crate_event_call;

    /**
     * Slot event function
     */
    slot_event_func slot_event_call;

    /**
     * Populate the slots with a specific tyoe of module.
     */
    virtual void create_module_slots();

    /**
     * Move the modules in the slot table to their slot based
     * on the module's slot number.
     */
    virtual void move_modules_to_slots();

private:
    /*
     * Check the slots.
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

/**
 * @brief A slot handle abstracts access to the module instance.
 *
 * This allows you to access a module and operate on it. The crate
 * registers this user and the module is locked while this object
 * exists.
 */
struct slot_handle {
    template<typename T> slot_handle(crate& crate_, T slot,
                                     module::check check = module::check::online);
    ~slot_handle() = default;

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

/**
 * Support a range of slot indexing types.
 */
template<typename T>
slot_handle::slot_handle(crate& crate_, T slot, module::check check)
    : handle(crate_[slot]), user(crate_), guard(handle) {
    crate_.ready();
    module::check_module(handle, check);
}

/**
 * @brief Module Crate is a logical mapping of module numbers to slots
 *
 * This object is concurrent for its access. If you use a module_crate
 * then avoid direct access to the physical crate, reference it via
 * this object. This gates access to the physical crate if module
 * numbers are being assigned or modules are being taken offline or
 * brought online.
 */
struct module_crate {
    using lock_type = std::recursive_mutex;
    using lock_guard = std::lock_guard<lock_type>;
    using module_num = hw::slot_type;
    using module_map = hw::slot_numbers;
    using crate_ptr = crate*;

protected:
    /**
     * The physical crate the module crate logically maps to.
     */
    crate_ptr phys_crate;

    /**
     * Check is there is a physical crate. Raise an error if not set.
     */
    void check() {
        if (phys_crate == nullptr) {
            throw error(pixie::error::code::crate_not_ready,
                        "no physical crate set");
        }
    }

public:
    /**
     * Module slot map.
     */
    module_map modules;

    /**
     * Number of modules mapped to crate slots.
     */
    size_t num_modules;

    /**
     * Construct a module crate with no physical crate.
     */
    module_crate();

    /**
     * Construct a module crate for the provided physical crate.
     */
    module_crate(crate& phys_crate);

    /**
     * Set the crate pointer
     */
    void set(crate& phys_crate);

    /**
     * Map the container of module numbers to slot numbers.
     */
    template<typename T> void modules_to_slots(T& numbers);

    /**
     * @brief Validity checking operator that returns the physical crate pointer.
     * @return A pointer to the physical crate.
     */
    xia::pixie::crate::crate* operator->() {
        lock_guard guard(lock_);
        check();
        return phys_crate;
    }

    /**
     * @brief Validity checking operator that returns the physical crate pointer.
     * @return A pointer to the physical crate.
     */
    operator xia::pixie::crate::crate*() {
        lock_guard guard(lock_);
        check();
        return phys_crate;
    }

    /**
     * @brief Validity checking operator that returns a reference to
     *        the physical crate.
     * @return A reference to the physical crate.
     */
    operator xia::pixie::crate::crate&() {
        lock_guard guard(lock_);
        check();
        return *phys_crate;
    }

    /**
     * @brief Range checking operator to index modules based on index types.
     * @tparam T The templated index type
     * @param number The module number from the index
     * @return A pointer to the module object found at the requested index.
     */
    template<typename T> module::module& operator[](T number);

    /**
     * @brief Return a module indexed by a slot.
     * @tparam T The template for various ways to determine the slot.
     * @param slot The physical slot in the crate that we'd like to access.
     * @return A pointer to the module object associated with the physical slot.
     */
    template<typename T> module::module_ptr find(T slot);

    /**
     * @brief Assign numbers to the modules by slot.
     *
     * Known modules not in the map are closed. Modules in
     * map that are not know are opened.
     *
     * @param numbers A list of slots that should be assigned.
     */
    void assign(const module::number_slots& numbers);

    /*
     * Format the module to slot map
     */
    void format_module_map(std::string& str);
    std::string format_module_map();

protected:
    /*
     * Crate event handler
     */
    void crate_event(crate::crate_event event);

    /*
     * Slot event handler
     */
    void slot_event(hw::slot_type slot, crate::slot_event event);

    /*
     * Discovered module.
     */
    void module_discovered(hw::slot_type slot);

private:
    /*
     * Module crate lock
     */
    lock_type lock_;
};

/**
 * Capture the module states for the crate.
 */
void capture_state(crate& crate, module::module_states& states);

/**
 * State labels
 */
const char* crate_event_label(crate::crate_event event);
const char* slot_event_label(crate::slot_event event);

template<typename T> void module_crate::modules_to_slots(T& numbers) {
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i] >= modules.size()) {
            throw error(pixie::error::code::module_number_invalid,
                        "module number out of range: " + std::to_string(numbers[i]));
        }
        numbers[i] = modules[numbers[i]];
    }
}

template<typename T> module::module& module_crate::operator[](T number) {
    size_t number_ = static_cast<size_t>(number);
    lock_guard guard(lock_);
    check();
    if (number_ >= num_modules) {
        throw error(pixie::error::code::module_number_invalid,
                    "module number out of range: " + std::to_string(number));
    }
    crate& pcrate = *phys_crate;
    return pcrate[modules[number_]];
}

template<typename T> module::module_ptr module_crate::find(T slot) {
    hw::slot_type slot_ = static_cast<hw::slot_type>(slot);
    lock_guard guard(lock_);
    check();
    auto mod = std::find_if(modules.begin(), modules.end(),
                            [slot_](const hw::slot_type s) {
                                return s == slot_; });
    if (mod == modules.end()) {
        throw error(
            pixie::error::code::module_number_invalid, "module slot not found");
    }
    return phys_crate->find(*mod);
}

/**
 * @brief A module handle abstracts access to the module instance.
 *
 * This allows you to access a module and operate on it. The crate
 * registers this user and the module is locked while this object
 * exists.
 */
struct module_handle {
    template<typename T> module_handle(module_crate& crate_, T num,
                                       module::check check = module::check::online);
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

/**
 * Support a range of slot indexing types.
 */
template<typename T>
module_handle::module_handle(module_crate& crate_, T num, module::check check)
    : handle(crate_[num]), user(*crate_), guard(handle) {
    crate_->ready();
    module::check_module(handle, check);
}
}  // namespace crate
}  // namespace pixie
}  // namespace xia

/**
 * @brief Output stream operator for crates
 */
std::ostream& operator<<(std::ostream& out, const xia::pixie::crate::crate& crate);

#endif  // PIXIE_CRATE_H
