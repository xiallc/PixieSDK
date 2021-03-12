/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2020, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>
#include <sstream>

#include <pixie_module.hpp>

#include <hw/i2cm24c64.hpp>
#include <hw/pcf8574.hpp>

#include <pixie16sys_defs.h>

#include <PlxApi.h>

namespace xia
{
namespace pixie
{
namespace module
{
    error::error(const std::string& what)
        : runtime_error(what) {
    }

    error::error(const char* what)
        : runtime_error(what) {
    }

    /*
     * PLX PCI vendor and device id
     */
    const int vendor_id = 0x10b5;
    const int device_id = 0x9054;

    struct pci_bus_handle {
        int device_number;
        PLX_DEVICE_OBJECT handle;
        PLX_DEVICE_KEY key;
        pci_bus_handle();
    };

    pci_bus_handle::pci_bus_handle()
        : device_number(-1)
    {
        ::memset(&key, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));
        key.VendorId = vendor_id;
        key.DeviceId = device_id;
    }

    int
    pci_bus(const bus_handle& device)
    {
        if (device) {
            return device->key.bus;
        }
        return -1;
    }

    int
    pci_slot(const bus_handle& device)
    {
        if (device) {
            return device->key.slot;
        }
        return -1;
    }

    module::module()
        : device(std::make_unique<pci_bus_handle>()),
          slot(0),
          serial_num(0),
          revision(0),
          index(-1),
          vmaddr(nullptr),
          module_var_descriptors(param::get_module_var_descriptors()),
          channel_var_descriptors(param::get_channel_var_descriptors()),
          reg_trace(false)
    {
    }

    module::module(module&& m)
        : device(std::move(m.device)),
          slot(m.slot),
          serial_num(m.serial_num),
          revision(m.revision),
          index(m.index),
          vmaddr(m.vmaddr),
          module_var_descriptors(std::move(m.module_var_descriptors)),
          channel_var_descriptors(std::move(m.channel_var_descriptors)),
          reg_trace(m.reg_trace)
    {
        m.slot = 0;
        m.serial_num = 0;
        m.revision = 0;
        m.index = -1;
        m.vmaddr = nullptr;
        m.reg_trace = false;
    }

    module::~module()
    {
        /*
         * Catch an error here and log.
         */
        try {
            close();
        } catch(error& e) {
            std::cout << "error: " << e.what() << std::endl;
        }
    }

    void
    module::open(size_t device_number)
    {
        PLX_STATUS ps;

        ps = ::PlxPci_DeviceFind(&device->key, device_number);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "Pixie PCI find: device: " << device->device_number
                << " : " << ps;
            throw error(oss.str());
        }

        device->device_number = device_number;

        ps = ::PlxPci_DeviceOpen(&device->key, &device->handle);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "Pixie PCI open: device: " << device->device_number
                << " : " << ps;
            throw error(oss.str());
        }

        /*
         * For PLX 9054, Space 0 is at PCI BAR 2.
         */
        ps = PlxPci_PciBarMap(&device->handle, 2, (VOID**) &vmaddr);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "Pixie PCI BAR map: device: " << device->device_number
                << " : " << ps;
            throw error(oss.str());
        }

        hw::i2c::pcf8574 pio(*this, PCF8574_ADDR, 1 << 0, 1 << 1, 1 << 2);
        slot = pio.read_a_byte() & 0xf8 >> 3;

        hw::i2c::i2cm24c64 eeprom(*this, I2CM24C64_ADDR, 1 << 0, 1 << 1, 1 << 2);
        std::vector<uint8_t> data;

        eeprom.sequential_read(0, 3, data);

        if (data.size() != 3) {
            std::ostringstream oss;
            oss << "Pixie EEPROM read: device: " << device->device_number
                << " : invalid data length:" << data.size();
            throw error(oss.str());
        }

        /*
         *  Starting with serial number 256, serial number is stored in the
         *  first two bytes of EEPROM, followed by revision number, which is at
         *  least 11 (i.e. Rev-B)
         */
        revision = data[2];

        if (revision >= 11) {
            serial_num = ((int) data[1] << 8) | (int) data[0];
        } else {
            serial_num = (unsigned int) data[0];
        }

        std::cout << "module: slot:" << slot
                  << " serial-number:" << serial_num
                  << " revision:" << revision
                  << "  PCI: device=" << device->device_number
                  << std::hex
                  << " device-id=0x" << device->key.DeviceId
                  << " vendor-id=0x" << device->key.VendorId
                  << " pci-bus=0x" << (unsigned int) device->key.bus
                  << " pci-slot=0x" << (unsigned int) device->key.slot
                  << std::dec << std::endl;
    }

    void
    module::close()
    {
        if (device) {
            bus_handle device_ = std::move(device);
            PLX_STATUS ps_unmap_bar = PLX_STATUS_OK;
            PLX_STATUS ps_close;

            if (vmaddr != nullptr) {
                ps_unmap_bar = ::PlxPci_PciBarUnmap(&device_->handle, &vmaddr);
            }

            ps_close = ::PlxPci_DeviceClose(&device_->handle);

            /*
             * A single error for both operations and the device is always
             * closed..
             */
            if (ps_unmap_bar != PLX_STATUS_OK || ps_close != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "Pixie PCI ";
                if (ps_unmap_bar != PLX_STATUS_OK)
                    oss << "BAR unmap";
                if (ps_unmap_bar != PLX_STATUS_OK && ps_close != PLX_STATUS_OK)
                    oss << " and ";
                if (ps_close != PLX_STATUS_OK)
                    oss << "close";
                oss << ": device: " << device->device_number
                    << " : " << ps_unmap_bar << ", " << ps_close;
                throw error(oss.str());
            }
        }
    }

    void
    module::initialize(const std::string varsdef_)
    {
        varsdef = varsdef_;
        param::load(varsdef,
                    module_var_descriptors,
                    channel_var_descriptors);
    }
};
};
};
