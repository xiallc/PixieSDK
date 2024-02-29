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

#include <omnitool-threads.hpp>

namespace xia {
namespace omnitool {
namespace thread {
module_worker::module_worker()
    : number(-1), slot(xia::pixie::hw::slot_invalid), revision(-1),
      pci_bus(-1), pci_slot(-1), running(false), has_error(false),
      total(0), last_total(0) {}

module_worker::module_worker(const module_worker& orig)
    : number(orig.number), slot(orig.slot), revision(orig.revision),
      pci_bus(orig.pci_bus), pci_slot(orig.pci_slot), running(orig.running.load()),
      has_error(orig.has_error), period(orig.period),
      total(orig.total), last_total(orig.last_total) {}

module_worker::module_worker(module_worker&& orig)
    : number(orig.number), slot(orig.slot), revision(orig.revision),
      pci_bus(orig.pci_bus), pci_slot(orig.pci_slot), running(orig.running.load()),
      has_error(orig.has_error), period(orig.period),
      total(orig.total), last_total(orig.last_total) {}
} // namespace thread
} // namespace omnitool
} // namespace xia
