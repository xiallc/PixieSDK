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

/** @file module.cpp
 * @brief Implements functions and data structures related to module reporting
 */

#include <pixie/reports/reports.hpp>
#include <pixie/utils/io.hpp>

namespace xia {
namespace reports {

void report(pixie::module::module& module, std::ostream& out) {
    if (module.present()) {
        util::io::ostream_guard flags(out);

        out << std::fixed << std::setprecision(3) << std::boolalpha;

        std::ostringstream title;
        title << "Module " << module.number << " (slot " << module.slot << ')';

        out << title.str() << std::endl
            << std::string(title.str().length(), '=') << std::endl
            << std::endl
            << "Serial Number   : " << module.serial_num << std::endl
            << "Revision        : " << module.revision_label() << " (" << module.revision << ')' << std::endl
            << "Major Revision  : " << module.major_revision << std::endl
            << "Minor Revision  : " << module.minor_revision << std::endl
            << "Crate Revision  : " << module.crate_revision << std::endl
            << "Board Revision  : " << module.board_revision << std::endl
            << std::endl
            << "PCI Bus         : " << module.pci_bus() << std::endl
            << "PCI_Slot        : " << module.pci_slot() << std::endl
            << std::endl;
        out << "Num Channels    : " << module.num_channels << std::endl
            << "Max Channels    : " << module.max_channels << std::endl
            << std::endl
            << "EEPROM Format   : " << module.eeprom_format << std::endl
            << std::endl
            << "FIFO Buffers    : " << module.fifo_buffers << std::endl
            << "FIFO Run wait   : " << module.fifo_run_wait_usecs << " usecs" << std::endl
            << "FIFO Idle wait  : " << module.fifo_idle_wait_usecs << " usecs" << std::endl
            << "FIFO Hold       : " << module.fifo_hold_usecs << " usecs" << std::endl
            << "FIFO DMA Trig   : " << module.fifo_dma_trigger_level << " words" << std::endl
            << "FIFO Bandwidth  : ";
        if (module.fifo_bandwidth == 0) {
            out << "unlimited";
        } else {
            out << module.fifo_bandwidth << " Mbytes/sec";
        }
        out << std::endl
            << std::endl
            << "I2C read period : " << module.i2c_read_period << " usecs" << std::endl
            << "IO CPLD old     : " << module.io_cpld_version_old << std::endl
            << std::endl;

        if (module.online()) {
            out << "Address Map" << std::endl << "-----------" << std::endl;
            module.param_addresses.output(out, true);
            out << std::endl;

            out << "Module Variables" << std::endl << "----------------" << std::endl << std::endl;
            var_report(module.module_vars, out);

            out << "Channels" << std::endl << "--------" << std::endl << std::endl;
            subreport(module.channels, out, module.num_channels);
        }
    }
}

}
}