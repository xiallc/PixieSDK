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

void report(pixie::module::module& module, pixie::format::json& out) {
    if (module.present()) {
        out["product"] = "Pixie-16";
        out["model"] = module.revision_label() +
                          (" (" + std::to_string(module.revision) + ")");
        out["serial_number"] = std::to_string(module.serial_num);
        out["major_revision"] = module.major_revision;
        out["minor_revision"] = module.minor_revision;
        out["crate_revision"] = module.crate_revision;
        out["board_revision"] = module.board_revision;
        out["pci_bus"] = module.pci_bus();
        out["pci_slot"] = module.pci_slot();
        out["crate_slot"] = module.slot;
        out["adc_res"] = module.channels[0].fixture->config.adc_bits;
        out["adc_freq"] = module.channels[0].fixture->config.adc_msps;
        out["num_channels"] = module.num_channels;
        out["max_channels"] = module.max_channels;
        out["eeprom_version"] = module.eeprom_format;
        out["worker"]["buffers"] = module.fifo_buffers;
        out["worker"]["run_wait"] = module.fifo_run_wait_usecs.load();
        out["worker"]["idle_wait"] = module.fifo_idle_wait_usecs.load();
        out["worker"]["hold"] = module.fifo_hold_usecs.load();
        out["worker"]["dma_trig"] = module.fifo_dma_trigger_level.load();
        out["worker"]["bandwidth"] = module.fifo_bandwidth.load();
        out["i2c_read_period"] = module.i2c_read_period;
        out["old_io_cpld_version"] = module.io_cpld_version_old;
        out["online"] = module.online();
        out["running"] = module.run_active();
    }
}

}
}
