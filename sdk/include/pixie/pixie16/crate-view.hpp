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

/** @file crate-views.hpp
 * @brief Defines functions and data structures related to handling a view of
 *        the Pixie-16 crate.
 */

#ifndef PIXIE_CRATE_VIEW_H
#define PIXIE_CRATE_VIEW_H

#include <atomic>
#include <functional>

#include <pixie/error.hpp>
#include <pixie/fw.hpp>

#include <pixie/pixie16/backplane.hpp>
#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace crate {
/**
 * @brief Defines functions and data structures related to handling a view of
 *        the Pixie-16 crate.
 */
namespace view {

/**
 * @brief Module Crate is a logical mapping of module numbers to slots
 *
 * This object is concurrent for its access. If you use a module
 * then avoid direct access to the physical crate, reference it via
 * this object. This gates access to the physical crate if module
 * numbers are being assigned or modules are being taken offline or
 * brought online.
 */
struct module {
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
    module();

    /**
     * Construct a module crate for the provided physical crate.
     */
    module(crate& phys_crate);

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
    template<typename T> pixie::module::module& operator[](T number);

    /**
     * @brief Return a module indexed by a slot.
     * @tparam T The template for various ways to determine the slot.
     * @param slot The physical slot in the crate that we'd like to access.
     * @return A pointer to the module object associated with the physical slot.
     */
    template<typename T> pixie::module::module_ptr find(T slot);

    /**
     * @brief Assign numbers to the modules by slot.
     *
     * Known modules not in the map are closed. Modules in
     * map that are not know are opened.
     *
     * @param numbers A list of slots that should be assigned.
     */
    void assign(const pixie::module::number_slots& numbers);

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

template<typename T> void module::modules_to_slots(T& numbers) {
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i] >= modules.size()) {
            throw error(pixie::error::code::module_number_invalid,
                        "module number out of range: " + std::to_string(numbers[i]));
        }
        numbers[i] = modules[numbers[i]];
    }
}

template<typename T> pixie::module::module& module::operator[](T number) {
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

template<typename T> pixie::module::module_ptr module::find(T slot) {
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
 * @brief Slot Module Crate is a view of a physical crate allowing
 *        either slot or logically mapping.
 *
 * This object is concurrent for its access. If you use a module
 * then avoid direct access to the physical crate, reference it via
 * this object. This gates access to the physical crate if module
 * numbers are being assigned or modules are being taken offline or
 * brought online.
 */
struct slot_module {
    using lock_type = std::recursive_mutex;
    using lock_guard = std::lock_guard<lock_type>;
    using module_num = hw::slot_type;
    using module_map = hw::slot_numbers;
    using module_ptr = module*;

protected:
    /**
     * The module crate the slot_module crate logically maps to.
     */
    module_ptr mod_crate;

    /**
     * Check is there is a module. Raise an error if not set.
     */
    void check() {
        if (mod_crate == nullptr) {
            throw error(pixie::error::code::crate_not_ready,
                        "no module crate set");
        }
    }

    /**
     * Set the module pointer
     */
    void set_module(module& wrapped_module);

    /**
     * The omnitool is using slots
     */
    bool using_slots;

public:

    /**
     * Construct a slot_module with no module.
     */
    slot_module();

    /**
     * Construct a slot_module for the provided module. Set if
     * it is logically or slot mapped.
     */
    slot_module(module& wrapped_module, bool logical_map);

    /**
      * Returns the modules as slots in slot mode otherwise as a logically mapping.
     */
    module_map get_modules(void);

    /**
     * Returns num_modules
     */
    size_t get_num_modules(void);

    /**
     * Set the crate pointer of the module
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
        return (*mod_crate);
    }

    /**
     * @brief Validity checking operator that returns the physical crate pointer.
     * @return A pointer to the physical crate.
     */
    operator xia::pixie::crate::crate*() {
        lock_guard guard(lock_);
        check();
        return (*mod_crate);
    }

    /**
     * @brief Validity checking operator that returns a reference to
     *        the physical crate.
     * @return A reference to the physical crate.
     */
    operator xia::pixie::crate::crate&() {
        lock_guard guard(lock_);
        check();
        return *(*mod_crate);
    }

    /**
     * @brief Range checking operator to index modules based on index types.
     * @tparam T The templated index type
     * @param number The module or slot number from the index
     * @return A pointer to the module object found at the requested index.
     */
    template<typename T> pixie::module::module& operator[](T number);

    /**
     * @brief Return a module indexed by a slot.
     * @tparam T The template for various ways to determine the slot.
     * @param slot The physical slot in the crate that we'd like to access.
     * @return A pointer to the module object associated with the physical slot.
     */
    template<typename T> pixie::module::module_ptr find(T slot);

    /**
     * @brief Assign numbers to the modules by slot.
     *
     * Known modules not in the map are closed. Modules in
     * map that are not know are opened.
     *
     * @param numbers A list of slots that should be assigned.
     */
    void assign(const pixie::module::number_slots& numbers);

    /*
     * Format the module to slot map
     */
    void format_module_map(std::string& str);
    std::string format_module_map();


private:
    /*
     * Module crate lock
     */
    lock_type lock_;
};

template<typename T> void slot_module::modules_to_slots(T& numbers) {
    if (!using_slots) {
        mod_crate->modules_to_slots(numbers);
    }
}

template<typename T> pixie::module::module& slot_module::operator[](T number) {
    size_t number_ = static_cast<size_t>(number);
    lock_guard guard(lock_);
    check();
    if (!using_slots) {
        if (number_ >= get_num_modules()) {
            throw error(pixie::error::code::module_number_invalid,
                        "module number out of range: " + std::to_string(number));
        }
        number_ = mod_crate->modules[number_];
    } else {
        auto slot_num = std::find(begin(mod_crate->modules),
                        end(mod_crate->modules), number_);
        if (slot_num == end(mod_crate->modules)) {
            throw error(pixie::error::code::module_number_invalid,
                        "module number out of range: " + std::to_string(number));
        }
    }
    crate& pcrate = *mod_crate;
    return pcrate[number_];
}

template<typename T> pixie::module::module_ptr slot_module::find(T slot) {
    return mod_crate->find(slot);
}

/**
 * @brief A slot handle abstracts access to the module instance.
 *
 * This allows you to access a module and operate on it. The crate
 * registers this user and the module is locked while this object
 * exists.
 */
struct slot_handle {
    template<typename T> slot_handle(crate& crate_, T slot,
        pixie::module::check check = pixie::module::check::online);
    ~slot_handle() = default;

    pixie::module::module& operator*() {
        return handle;
    }
    pixie::module::module* operator->() {
        return &handle;
    }

private:
    pixie::module::module& handle;
    crate::user user;
    pixie::module::module::guard guard;
};

/**
 * Support a range of slot indexing types.
 */
template<typename T>
slot_handle::slot_handle(crate& crate_, T slot, pixie::module::check check)
    : handle(crate_[slot]), user(crate_), guard(handle) {
    crate_.ready();
    pixie::module::check_module(handle, check);
}
/**
 * @brief A module handle abstracts access to the module instance.
 *
 * This allows you to access a module and operate on it. The crate
 * registers this user and the module is locked while this object
 * exists.
 */
struct module_handle {
    template<typename T> module_handle(module& crate_, T num,
        pixie::module::check check = pixie::module::check::online);
    ~module_handle() = default;

    pixie::module::module& operator*() {
        return handle;
    }
    pixie::module::module* operator->() {
        return &handle;
    }

private:
    pixie::module::module& handle;
    crate::user user;
    pixie::module::module::guard guard;
};

/**
 * Support a range of slot indexing types.
 */
template<typename T>
module_handle::module_handle(module& crate_, T num, pixie::module::check check)
    : handle(crate_[num]), user(*crate_), guard(handle) {
    crate_->ready();
    pixie::module::check_module(handle, check);
}
} // namespace views
} // namespace crate
} // namespace pixie
} // namespace xia
#endif  // PIXIE_CRATE_VIEW_H
