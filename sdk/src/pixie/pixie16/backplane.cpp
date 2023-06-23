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

/** @file crate.cpp
 * @brief Implements functions and data structures related to handling a Pixie-16 crate's backplane.
 */

#include <pixie/log.hpp>
#include <pixie/param.hpp>

#include <pixie/pixie16/backplane.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace backplane {
backplane::role::role(const std::string& label_) : label(label_), leader(-1) {}

bool backplane::role::request(const module::module& mod) {
    if (mod.opened()) {
        auto expected = released;
        auto desired = mod.slot;
        const bool requested = leader.compare_exchange_strong(
            expected, desired, std::memory_order_release, std::memory_order_relaxed);
        if (requested) {
            xia_log(log::info) << "backplane: " << label << ": leader: slot=" << mod.slot;
        } else {
            if (leader.load() == desired) {
                return true;
            }
        }
        return requested;
    }
    return false;
}

bool backplane::role::release(const module::module& mod) {
    if (mod.opened()) {
        auto expected = mod.slot;
        auto desired = released;
        const bool released_ = leader.compare_exchange_strong(
            expected, desired, std::memory_order_release, std::memory_order_relaxed);
        if (released_) {
            xia_log(log::info) << "backplane: " << label << ": released: slot=" << mod.slot;
        }
        return released_;
    }
    return false;
}

bool backplane::role::operator==(const module::module& mod) const {
    return slot() == mod.slot;
}

bool backplane::role::operator!=(const module::module& mod) const {
    return slot() != mod.slot;
}

bool backplane::role::not_leader(const module::module& mod) {
    return has_leader() && *this != mod;
}

backplane::backplane()
    : wired_or_triggers_pullup("wired-or-triggers"), run("run"), director("director"),
      num_slots_present(0), sync_waits(0), sync_waiters({false}) {}

void backplane::sync_wait(module::module& mod, const param::value_type synch_wait) {
    bool synch_wait_active = synch_wait == 1;
    if (mod.online()) {
        if (synch_wait_active != sync_waiters[mod.slot]) {
            if (synch_wait_active) {
                ++sync_waits;
            } else {
                /*
                 * P16-556
                 *  The default setting for the sync_waiters is false so
                 *  if a module sets the synch_wait with 0 and there is no
                 *  leader the sync_waits will underroll.
                 */
                if (sync_waits.load() > 0) {
                    --sync_waits;
                }
            }
            sync_waiters[mod.slot] = synch_wait_active;
            /*
             * Range check. The check is not for the module count in the
             * crate because a module does not know about other modules so
             * check against the size of the waiters which is the maximum
             * number of slots a crate has. Out of range is bug.
             */
            size_t sw = sync_waits.load();
            if (sw > num_slots_present) {
                throw error(error::code::internal_failure,
                            "slot: " + std::to_string(mod.slot) +
                            ": invalid backplane sync_wait value: " + std::to_string(sw));
            }
        }
    }
}

void backplane::sync_wait(module::module& mod) {
    if (mod.online()) {
        sync_wait(mod, mod.read(param::module_param::synch_wait));
    }
}

void backplane::sync_wait_valid() const {
    size_t waits = sync_waits.load();
    if (waits != 0 && waits != num_slots_present) {
        throw error(error::code::module_invalid_operation,
                    "sync wait mode enabled and not all slots in the sync wait state: " +
                    std::to_string(waits) + " of " + std::to_string(num_slots_present) +
                    " waiting");
    } else if (waits != 0 && !run.has_leader()) {
        throw error(error::code::module_invalid_operation,
                    "sync wait mode enabled but no run leader slot is assigned");
    }
}

void backplane::init(const size_t num_present) {
    init(sync_waiters.size(), num_present);
}

void backplane::init(const size_t num_slots, const size_t num_present) {
    sync_waits = 0;
    num_slots_present = num_present;
    sync_waiters.resize(num_slots, false);
    std::fill(sync_waiters.begin(), sync_waiters.end(), false);
}

void backplane::offline(const module::module& module) {
    if (module.slot_valid() && module.slot < sync_waiters.size()) {
        if (sync_waiters[module.slot]) {
            --sync_waits;
        }
        sync_waiters[module.slot] = false;
    }
}

};  // namespace backplane
};  // namespace pixie
};  // namespace xia
