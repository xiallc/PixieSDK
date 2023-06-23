/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2022 XIA LLC, All rights reserved.
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

/** @file backplane.hpp
 * @brief Defines functions and data structures related to handling a Pixie-16 crate's backplane.
 */

#ifndef PIXIE_BACKPLANE_H
#define PIXIE_BACKPLANE_H

#include <atomic>
#include <limits>
#include <vector>

#include <pixie/error.hpp>
#include <pixie/param.hpp>

namespace xia {
namespace pixie {
namespace module {
class module;
}
/**
 * @brief Defines functions and data structures related to handling a Pixie-16 crate.
 */
namespace backplane {
/*
 * Crate errors
 */
using error = pixie::error::error;

/**
 * @brief A backplane connects the modules in a crate together.
 */
struct backplane {
    using waiters = std::vector<bool>;

    struct role {
        /**
         * Leader is released when this value.
         */
        static const size_t released = std::numeric_limits<size_t>::max();

        /**
         * The role's label
         */
        std::string label;

        /**
         * The role leader variable indicates if there is a leader of
         * the role leader and which module it is. If the role leader
         * value is -1 there is no active role leader and the
         * leadership can be reserved.
         */
        std::atomic_size_t leader;

        /**
         * Request leader role.
         * @retval true The request is successful or module is the
         *              role leader
         */
        bool request(const module::module& mod);

        /**
         * Release leader role if the leader
         * @retval true The leader role has been released
         */
        bool release(const module::module& mod);

        /**
         * Is this module the role leader (or not)?
         */
        bool operator==(const module::module& mod) const;
        bool operator!=(const module::module& mod) const;

        /**
         * Leader slot?
         */
        size_t slot() const { return leader.load(); }

        /**
         * Has leader?
         */
        bool has_leader() const { return slot() != released; }

        /**
         * Not leader?
         *
         * A module is not the leader if there is a leader and the
         * module is not it.
         *
         * @note Testing if a module is the leader does not work
         * because it may not be the leader if there is no leader or
         * it is not the leader. You cannot tell which it is and
         * inverting the test does when ending a run.
         */
        bool not_leader(const module::module& mod);

        /**
         * Cannot be copied
         */
        role& operator-(const role& other) = delete;

        role(const std::string& label);
    };

    /**
     * If there is a role leader the wired-OR trigger lines are
     * connected to a pullup. Can only be set by a single module.
     */
    role wired_or_triggers_pullup;

    /**
     * The run leader starts and stops a run after all modules have
     * asserted their sync run signal.
     */
    role run;

    /**
     * The director module.
     */
    role director;

    /**
     * Slots present
     */
    std::atomic_size_t num_slots_present;

    /**
     * Sync waits must be 0 or the number of slots in a crate.
     */
    std::atomic_size_t sync_waits;
    waiters sync_waiters;

    backplane();

    /**
     * Update Sync wait
     */
    void sync_wait(module::module& mod, const param::value_type synch_wait);
    void sync_wait(module::module& mod);

    /**
     * Is synch_wait valid?
     *
     * For synch_wait to be valid there can be no synch waiters or all
     * modules must be synch waiters.
     */
    void sync_wait_valid() const;

    /**
     * Initialize the sync waiters to the number of slots in the crate
     * and initialise to false. Set the number of slots with the
     * modules present.
     */
    void init(const size_t num_slots, const size_t num_present);

    /**
     * Initialize the sync waiter state to false and set the number
     * of slots with the modules present.
     */
    void init(const size_t num_present);

    /**
     * Reinitialize. Use a template as a container forward decal.
     */
    template<typename T> void reinit(T& slots, const size_t num_slots);

    /**
     * A module is taken offline. Update the backplane.
     */
    void offline(const module::module& module);
};

template<typename T> void backplane::reinit(T& slots, const size_t num_slots) {
    init(num_slots);
    for (auto& mod : slots) {
        sync_wait(*mod);
    }
}
}  // namespace backplane
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_BACKPLANE_H
