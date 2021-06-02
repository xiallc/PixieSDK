/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
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

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <pixie_channel.hpp>
#include <pixie_module.hpp>
#include <pixie_log.hpp>
#include <pixie_util.hpp>

#include <hw/csr.hpp>
#include <hw/dsp.hpp>
#include <hw/fpga_comms.hpp>
#include <hw/fpga_fippi.hpp>
#include <hw/memory.hpp>
#include <hw/pcf8574.hpp>
#include <hw/run.hpp>

/*
 * @todo move the few defines out of these at some point
 */
#include <pixie16sys_defs.h>
#include <pixie16app_defs.h>

#include <PlxApi.h>

namespace xia
{
namespace pixie
{
namespace module
{
    static std::string
    module_label(const int num, const int slot, const char* label = "module")
    {
        std::ostringstream oss;
        oss << label << ": num=" << num << ",slot=" << slot << ": ";
        return oss.str();
    }

    std::string
    module_label(const module& mod, const char* label)
    {
        return module_label(mod.number, mod.slot, label);
    }

    error::error(const int num, const int slot,
                 const code type, const std::ostringstream& what)
        : pixie::error::error(type, make_what(num, slot, what.str().c_str()))
    {
    }

    error::error(const int num, const int slot,
                 const code type, const std::string& what)
        : pixie::error::error(type, make_what(num, slot, what.c_str()))
    {
    }

    error::error(const int num, const int slot,
                 const code type, const char* what)
        : pixie::error::error(type, make_what(num, slot, what))
    {
    }

    void
    error::output(std::ostream& out)
    {
        util::ostream_guard flags(out);
        out << std::setfill(' ')
            << "error: code=" << std::setw(2) << result()
            << ' ' << what();
    }

    std::string
    error::make_what(const int num, const int slot, const char* what_)
    {
        std::ostringstream oss;
        oss << module_label(num, slot) << what_;
        return oss.str();
    }

    /*
     * PLX PCI vendor and device id
     */
    const int vendor_id = 0x10b5;
    const int device_id = 0x9054;

    static const char* pci_error_labels[] = {
        "PLX_STATUS_OK",
        "PLX_STATUS_FAILED",
        "PLX_STATUS_NULL_PARAM",
        "PLX_STATUS_UNSUPPORTED",
        "PLX_STATUS_NO_DRIVER",
        "PLX_STATUS_INVALID_OBJECT",
        "PLX_STATUS_VER_MISMATCH",
        "PLX_STATUS_INVALID_OFFSET",
        "PLX_STATUS_INVALID_DATA",
        "PLX_STATUS_INVALID_SIZE",
        "PLX_STATUS_INVALID_ADDR",
        "PLX_STATUS_INVALID_ACCESS",
        "PLX_STATUS_INSUFFICIENT_RES",
        "PLX_STATUS_TIMEOUT",
        "PLX_STATUS_CANCELED",
        "PLX_STATUS_COMPLETE",
        "PLX_STATUS_PAUSED",
        "PLX_STATUS_IN_PROGRESS",
        "PLX_STATUS_PAGE_GET_ERROR",
        "PLX_STATUS_PAGE_LOCK_ERROR",
        "PLX_STATUS_LOW_POWER",
        "PLX_STATUS_IN_USE",
        "PLX_STATUS_DISABLED",
        "PLX_STATUS_PENDING",
        "PLX_STATUS_NOT_FOUND",
        "PLX_STATUS_INVALID_STATE",
        "PLX_STATUS_BUFF_TOO_SMALL"
    };

    struct pci_bus_handle {
        int device_number;
        PLX_DEVICE_OBJECT handle;
        PLX_DEVICE_KEY key;
        PLX_DMA_PROP dma;
        pci_bus_handle();
    };

    pci_bus_handle::pci_bus_handle()
        : device_number(-1)
    {
        ::memset(&key, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));
        ::memset(&dma, 0, sizeof(PLX_DMA_PROP));
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

    std::string
    pci_error_text(PLX_STATUS ps)
    {
        std::ostringstream oss;
        oss << "PLX (" << int(ps) << ") ";
        if (ps >= PLX_STATUS_OK && ps < PLX_STATUS_RSVD_LAST_ERROR) {
            oss << pci_error_labels[ps - PLX_STATUS_OK];
        } else {
            oss << "unknown error code";
        }
        return oss.str();
    }

    /*
     * EEPROM module configuration handles the various format versions.
     */
    struct module_eeprom_format
    {
        std::pair<int, int> revisions;
        int version;
    };

    static const std::vector<module_eeprom_format> module_eeprom_formats =
    {
        { { 0x0a, 0x0f }, 0 },
        { { 0x0f, 0x0f }, 1 },
        { { 0x48, 0xff }, 2 }
    };

    /*
     * The early Pixie module EEPROMs do not contain the ADC data. Provide a
     * table of configuration data.
     */
    struct module_config
    {
        std::pair<int, int> serial_num;
        int adc_bits;
        int adc_msps;
        int num_channels;
        int eeprom_format;
        std::vector<int> revisions;
    };

    static const std::vector<module_config> module_configs =
    {
        { {    0,  255 }, 12, 100, 16, 0, { 0xA } },
        { {  256,  274 }, 12, 100, 16, 1, { 0xB, 0xC, 0xD } },
        { { 1000, 1034 }, 12, 250, 16, 1, { 0xF } }
    };

    module::guard::guard(module& mod)
        : lock_(mod.lock_),
          guard_(lock_)
    {
    }

    void
    module::guard::lock()
    {
        lock_.lock();
    }

    void
    module::guard::unlock()
    {
        lock_.unlock();
    }

    module::bus_guard::bus_guard(module& mod)
        : lock_(mod.bus_lock_),
          guard_(lock_)
    {
    }

    void
    module::bus_guard::lock()
    {
        lock_.lock();
    }

    void
    module::bus_guard::unlock()
    {
        lock_.unlock();
    }

    module::module()
        : slot(0),
          number(-1),
          serial_num(0),
          revision(0),
          num_channels(0),
          vmaddr(nullptr),
          eeprom_format(-1),
          run_task(hw::run::run_task::nop),
          control_task(hw::run::control_task::nop),
          fifo_buffers(default_fifo_buffers),
          fifo_run_wait_usecs(default_fifo_run_wait_usec),
          fifo_idle_wait_usecs(default_fifo_idle_wait_usec),
          fifo_hold_usecs(default_fifo_hold_usec),
          reg_trace(false),
          fifo_worker_running(false),
          fifo_worker_finished(false),
          in_use(0),
          present_(false),
          online_(false),
          comms_fpga(false),
          fippi_fpga(false),
          have_hardware(false),
          device(std::make_unique<pci_bus_handle>())
    {
    }

    module::module(module&& m)
        : slot(m.slot),
          number(m.number),
          serial_num(m.serial_num),
          revision(m.revision),
          num_channels(m.num_channels),
          vmaddr(m.vmaddr),
          configs(m.configs),
          eeprom(m.eeprom),
          eeprom_format(m.eeprom_format),
          module_var_descriptors(std::move(m.module_var_descriptors)),
          module_vars(std::move(m.module_vars)),
          channel_var_descriptors(std::move(m.channel_var_descriptors)),
          channels(std::move(m.channels)),
          firmware(std::move(m.firmware)),
          run_task(m.run_task.load()),
          control_task(m.control_task.load()),
          fifo_buffers(m.fifo_buffers),
          fifo_run_wait_usecs(m.fifo_run_wait_usecs.load()),
          fifo_idle_wait_usecs(m.fifo_idle_wait_usecs.load()),
          fifo_hold_usecs(m.fifo_hold_usecs.load()),
          reg_trace(m.reg_trace),
          fifo_worker_running(false),
          fifo_worker_finished(false),
          in_use(0),
          present_(m.present_.load()),
          online_(m.online_.load()),
          comms_fpga(m.comms_fpga),
          fippi_fpga(m.fippi_fpga),
          have_hardware(false),
          device(std::move(m.device))
    {
        m.slot = 0;
        m.number = -1;
        m.serial_num = 0;
        m.revision = 0;
        m.num_channels = 0;
        m.vmaddr = nullptr;
        m.configs.clear();
        m.eeprom.clear();
        m.eeprom_format = -1;
        m.module_var_descriptors.clear();
        m.module_vars.clear();
        m.channel_var_descriptors.clear();
        m.channels.clear();
        m.run_task = hw::run::run_task::nop;
        m.control_task = hw::run::control_task::nop;
        m.fifo_buffers = default_fifo_buffers;
        m.fifo_run_wait_usecs = default_fifo_run_wait_usec;
        m.fifo_idle_wait_usecs = default_fifo_idle_wait_usec;
        m.fifo_hold_usecs = default_fifo_hold_usec;
        m.reg_trace = false;
        m.present_ = false;
        m.online_ = false;
        m.comms_fpga = false;
        m.fippi_fpga = false;
        m.have_hardware = false;
    }

    module::~module()
    {
        /*
         * Catch an error here and log.
         */
        try {
            close();
        } catch(error& e) {
            log(log::error) << e;
        }
        device.release();
    }

    module&
    module::operator=(module&& m)
    {
        lock_guard guard(lock_);
        lock_guard guard_m(m.lock_);

        if (in_use != 0 || m.in_use != 0) {
            throw error(number, slot, error::code::internal_failure,
                        "cannot move modules when in use");
        }

        slot = m.slot;
        number = m.number;
        serial_num = m.serial_num;
        revision = m.revision;
        num_channels = m.num_channels;
        vmaddr = m.vmaddr;
        configs = m.configs;
        eeprom = std::move(m.eeprom);
        eeprom_format = m.eeprom_format;
        module_var_descriptors = std::move(m.module_var_descriptors);
        channel_var_descriptors = std::move(m.channel_var_descriptors);
        module_vars = std::move(m.module_vars);
        channels = std::move(m.channels);
        run_task = m.run_task.load();
        control_task = m.control_task.load();
        fifo_buffers = m.fifo_buffers;
        fifo_run_wait_usecs = m.fifo_run_wait_usecs.load();
        fifo_idle_wait_usecs = m.fifo_idle_wait_usecs.load();
        fifo_hold_usecs = m.fifo_hold_usecs.load();
        reg_trace = m.reg_trace;
        present_ = m.present_.load();
        online_ = m.online_.load();
        comms_fpga = m.comms_fpga;
        fippi_fpga = m.fippi_fpga;
        have_hardware = m.have_hardware;

        device = std::move(m.device);

        m.slot = 0;
        m.number = -1;
        m.serial_num = 0;
        m.revision = 0;
        m.num_channels = 0;
        m.vmaddr = nullptr;
        m.configs.clear();
        m.eeprom.clear();
        m.eeprom_format = -1;
        m.run_task = hw::run::run_task::nop;
        m.control_task = hw::run::control_task::nop;
        m.fifo_buffers = default_fifo_buffers;
        m.fifo_run_wait_usecs = default_fifo_run_wait_usec;
        m.fifo_idle_wait_usecs = default_fifo_idle_wait_usec;
        m.fifo_hold_usecs = default_fifo_hold_usec;
        m.reg_trace = false;
        m.present_ = false;
        m.online_ = false;
        m.comms_fpga = false;
        m.fippi_fpga = false;
        m.have_hardware = false;

        return *this;
    }

    bool
    module::present() const
    {
        return present_.load();
    }

    bool
    module::online() const
    {
        return online_.load();
    }

    void
    module::open(size_t device_number)
    {
        log(log::debug) << "module: open: device-number=" << device_number;

        lock_guard guard(lock_);

        if (module_var_descriptors.empty() ||
            channel_var_descriptors.empty()) {
            throw error(number, slot,
                        error::code::internal_failure,
                        "no module or channel variable descriptors");
        }

        if (online()) {
            throw error(number, slot,
                        error::code::module_already_open,
                        "module already open");
        }

        if (device->device_number < 0) {
            PLX_STATUS ps;

            ps = ::PlxPci_DeviceFind(&device->key, device_number);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "PCI find: device: " << device_number
                    << ": " << pci_error_text(ps);
                error::code ec = error::code::module_initialize_failure;
                if (ps == PLX_STATUS_NO_DRIVER ||
                    ps == PLX_STATUS_INVALID_OBJECT) {
                    ec = error::code::not_supported;
                }
                throw error(number, slot, ec, oss);
            }

            device->device_number = device_number;

            ps = ::PlxPci_DeviceOpen(&device->key, &device->handle);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "PCI open: device: " << device_number
                    << ": " << pci_error_text(ps);
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            oss);
            }

            U8 drv_major;
            U8 drv_minor;
            U8 drv_rev;

            ps = ::PlxPci_DriverVersion(&device->handle,
                                        &drv_major,
                                        &drv_minor,
                                        &drv_rev);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "driver version: device: " << device_number
                    << ": " << pci_error_text(ps);
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            oss);
            }

            log(log::info) << "PLX: driver: version=" << int(drv_major)
                           << '.' << int(drv_minor) << '.' << int(drv_rev);

            U16 chip_type;
            U8 chip_rev;
            ps = ::PlxPci_ChipTypeGet(&device->handle, &chip_type, &chip_rev);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "get chip type: device: " << device_number
                    << ": " << pci_error_text(ps);
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            oss);
            }

            log(log::info) << "PLX: device: type=" << std::hex << chip_type
                           << " rev=" << std::dec << int(chip_rev);

            /*
             * For PLX 9054, Space 0 is at PCI BAR 2.
             */
            ps = PlxPci_PciBarMap(&device->handle, 2, (VOID**) &vmaddr);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "PCI BAR map: device: " << device_number
                    << ": " << pci_error_text(ps);
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            oss);
            }

            /*
             * DMA channel for block transfers.
             */
            device->dma.ReadyInput = 1;
            device->dma.Burst = 1;
            device->dma.BurstInfinite = 1;
#if PLX_SDK_VERSION_MAJOR < 6
            device->dma.LocalAddrConst = 1;
#else
            device->dma.ConstAddrLocal = 1;
#endif
            device->dma.LocalBusWidth = 2;
            ps = ::PlxPci_DmaChannelOpen(&device->handle, 0, &device->dma);
            if (ps != PLX_STATUS_OK) {
                if (ps == PLX_STATUS_INVALID_ACCESS) {
                  throw error(number, slot,
                              error::code::module_already_open,
                              "module in use by another process");
                }
                std::ostringstream oss;
                oss << "PCI DMA: device: " << device_number
                    << ": " << pci_error_text(ps);
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            oss);
            }

            have_hardware = true;

            /*
             * We can only touch specific registers at this early stage of
             * handling a module. We can read the slot and EEPROM and then
             * check if the FPGAs are loaded. If the FPGAs are loaded the DSP
             * can be reset and the FIFO worker started. Do not access any
             * other registers until the FPGAs are loaded.
             */
            const uint32_t i2c_CTRL = 1 << 2;
            const uint32_t i2c_SCL = 1 << 1;
            const uint32_t i2c_SDA = 1 << 0;

            hw::i2c::pcf8574 pio(*this, PCF8574_ADDR,
                                 i2c_SDA, i2c_SCL, i2c_CTRL,
                                 reg_trace);
            uint8_t pio_value = pio.read_a_byte();

            if (pio_value == 0) {
                have_hardware = false;
                std::ostringstream oss;
                oss << "pio: device: " << device_number
                    << ": invalid value: 0";
                throw error(number, slot,
                            error::code::module_info_failure,
                            oss);
            }

            slot = (pio_value & 0xf8) >> 3;

            hw::i2c::i2cm24c64 i2cm24c64(*this, I2CM24C64_ADDR,
                                         i2c_SDA, i2c_SCL, i2c_CTRL);
            i2cm24c64.read(0, 128, eeprom);

            if (eeprom.size() != 128) {
                have_hardware = false;
                std::ostringstream oss;
                oss << "eeprom read: device: " << device_number
                    << ": invalid data length:" << eeprom.size();
                throw error(number, slot,
                            error::code::module_info_failure,
                            oss);
            }

            logging::memdump(log::debug, module_label(*this) + "EEPROM:",
                             eeprom.data(), eeprom.size());

            if (!eeprom_v2()) {
                /*
                 *  Starting with serial number 256, serial number is stored in
                 *  the first two bytes of EEPROM, followed by revision number,
                 *  which is at least 11 (i.e. Rev-B)
                 */
                revision = eeprom[2];

                if (revision >= rev_B) {
                    serial_num =
                        (static_cast<int>(eeprom[1]) << 8) |
                        static_cast<int>(eeprom[0]);
                } else {
                    serial_num = static_cast<int>(eeprom[0]);
                }

                config_settings();
            } else {
                have_hardware = false;
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            "EEPROM format 2 not supported");
            }

            present_ = true;

            start_fifo_services();
        }
    }

    void
    module::close()
    {
        lock_guard guard(lock_);

        if (device && device->device_number >= 0) {
            PLX_STATUS ps_dma;
            PLX_STATUS ps_unmap_bar = PLX_STATUS_OK;
            PLX_STATUS ps_close;

            log(log::debug) << module_label(*this)
                            << "close: device-number="
                            << device->device_number;

            /*
             * Stop the FIFO worker and destroy the buffer pool.
             */
            stop_fifo_services();

            /*
             * Close the DMA channel.
             */
            ps_dma = ::PlxPci_DmaChannelClose(&device->handle, 0);
            if (ps_dma != PLX_STATUS_OK) {
                log(log::debug) << module_label(*this)
                                << "DMA close: " << pci_error_text(ps_dma);
                if (ps_dma == PLX_STATUS_IN_PROGRESS) {
                    ::PlxPci_DeviceReset(&device->handle);
                    ::PlxPci_DmaChannelClose(&device->handle, 0);
                }
            }

            if (vmaddr != nullptr) {
                ps_unmap_bar = ::PlxPci_PciBarUnmap(&device->handle, &vmaddr);
                vmaddr = nullptr;
            }

            ps_close = ::PlxPci_DeviceClose(&device->handle);

            device->device_number = -1;
            online_ = false;
            have_hardware = false;
            present_ = false;

            /*
             * A single error for both operations and the device is always
             * closed..
             */
            if (ps_unmap_bar != PLX_STATUS_OK || ps_close != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "Pixie PCI ";
                if (ps_unmap_bar != PLX_STATUS_OK) {
                    oss << "BAR unmap";
                }
                if (ps_unmap_bar != PLX_STATUS_OK &&
                    ps_close != PLX_STATUS_OK) {
                    oss << " and ";
                }
                if (ps_close != PLX_STATUS_OK) {
                    oss << "close";
                }
                oss << ": device: " << device->device_number
                    << ": " << pci_error_text(ps_unmap_bar)
                    << ", " << pci_error_text(ps_close);
                throw error(number, slot,
                            error::code::module_close_failure,
                            oss.str());
            }
        }
    }

    template<typename T> void
    module::check_channel_num(T num)
    {
        if (num >= num_channels ||
            (std::is_signed<T>::value && num < 0)) {
            throw error(number, slot,
                        error::code::channel_number_invalid,
                        "channel number out of range");
        }
    }

    void
    module::probe()
    {
        lock_guard guard(lock_);

        online_ = dsp_online = fippi_fpga = comms_fpga = false;

        erase_values();

        hw::fpga::comms comms(*this);
        comms_fpga = comms.done();

        if (comms_fpga) {
            hw::fpga::fippi fippi(*this);
            fippi_fpga = fippi.done();

            if (fippi_fpga) {
                hw::dsp::dsp dsp(*this);
                dsp_online = dsp.init_done();
            }
        }

        log(log::info) << std::boolalpha
                       <<  module_label(*this)
                       << "probe: sys=" << comms_fpga
                       << " fippi=" << fippi_fpga
                       << " dsp=" << dsp_online;

        if (fippi_fpga) {
            init_values();
        }

        online_ = comms_fpga && fippi_fpga && dsp_online;
    }

    void
    module::boot(bool boot_comms, bool boot_fippi, bool boot_dsp)
    {
        lock_guard guard(lock_);

        if (online()) {
            log(log::warning) << "boot online module";
        }

        online_ = false;

        if (boot_comms) {
            if (comms_fpga) {
                log(log::info) << module_label(*this)
                               << "comms already loaded";
            }
            firmware::firmware_ref fw = get("sys");
            hw::fpga::comms comms(*this);
            comms_fpga = false;
            comms.boot(fw->data);
            comms_fpga = comms.done();
        }

        if (boot_fippi) {
            if (fippi_fpga) {
                log(log::info) << module_label(*this)
                               << "fippi already loaded";
            }
            if (!comms_fpga) {
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            "fippi boot needs comms booted");
            }
            firmware::firmware_ref fw = get("fippi");
            hw::fpga::fippi fippi(*this);
            fippi_fpga = false;
            fippi.boot(fw->data);
            fippi_fpga = fippi.done();
        }

        if (boot_dsp) {
            if (dsp_online) {
                log(log::info) << module_label(*this)
                               << "dsp already running";
            }
            if (!comms_fpga || !fippi_fpga) {
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            "dsp needs comms and fippi booted");
            }
            firmware::firmware_ref fw = get("dsp");
            hw::dsp::dsp dsp(*this);
            dsp_online = false;
            dsp.boot(fw->data);
            dsp_online = dsp.init_done();
        }

        if (fippi_fpga) {
            init_values();
        }

        start_fifo_services();

        log(log::info) << module_label(*this)
                       << std::boolalpha
                       << "boot: sys-fpga=" << comms_fpga
                       << " fippi-fpga=" << boot_fippi
                       << " dsp=" << dsp_online;

        online_ = comms_fpga && fippi_fpga && dsp_online;
    }

    void
    module::initialize()
    {
    }

    void
    module::config_settings()
    {
        if (serial_num == 0xFFFF) {
            throw error(number, slot,
                        error::code::module_initialize_failure,
                        "invalid serial number: blank EEPROM");
        }

        hw::config config;

        if (serial_num > 1034) {
            if (revision <= rev_F) {
                num_channels = 16;
            } else {
                num_channels = 16;
            }
            eeprom_format = 1;
            config.adc_bits = static_cast<int>(eeprom[99]);
            config.adc_msps =
                (static_cast<int>(eeprom[99 + 2]) << 8) |
                static_cast<int>(eeprom[99 + 1]);
        } else {
            for (const auto& mod_config : module_configs) {
                if (serial_num >= std::get<0>(mod_config.serial_num) &&
                    serial_num <= std::get<1>(mod_config.serial_num)) {
                    num_channels = mod_config.num_channels;
                    eeprom_format = mod_config.eeprom_format;
                    config.adc_bits = mod_config.adc_bits;
                    config.adc_msps = mod_config.adc_msps;
                    break;
                }
            }
        }

        if (config.adc_bits == 0) {
            std::ostringstream oss;
            oss << "unknown serial number to ADC config: "
                << serial_num;
            throw error(number, slot,
                        error::code::module_initialize_failure,
                        oss.str());
        }

        /*
         * Set the FPGA ADC clock divider and the FPGA clock frequency.
         */
        switch (config.adc_msps) {
        case 100:
            config.adc_clk_div = 1;
            break;
        case 250:
            config.adc_clk_div = 2;
            break;
        case 500:
            config.adc_clk_div = 5;
            break;
        default:
            throw error(number, slot,
                        error::code::module_initialize_failure,
                        "invalid ADC MSPS: " + std::to_string(config.adc_msps));
        };

        config.fpga_clk_mhz = config.adc_msps / config.adc_clk_div;

        configs.resize(num_channels, config);
    }

    void
    module::add(firmware::module& fw)
    {
        lock_guard guard(lock_);
        if (online()) {
            throw error(number, slot,
                        error::code::module_invalid_operation,
                        "module is online when setting firmware");
        }
        std::copy(fw.begin(), fw.end(),
                  std::back_inserter(firmware));
    }

    firmware::firmware_ref
    module::get(const std::string device)
    {
        lock_guard guard(lock_);
        /*
         * First check if a slot assigned firmware exists for this
         * device. If not see if a default is available.
         */
        for (auto& fwr : firmware) {
            if (fwr->device == device) {
                return fwr;
            }
        }
        std::ostringstream oss;
        oss << "firmware not found: slot=" << slot
            << ": device=" << device
            << " firmwares=" << firmware.size();
        throw error(number, slot,
                    error::code::module_invalid_firmware,
                    oss.str());
    }

    param::value_type
    module::read(const std::string& par)
    {
        log(log::info) << module_label(*this) << "write: par=" << par;
        return read(param::lookup_module_param(par));
    }

    param::value_type
    module::read(param::module_param par)
    {
        log(log::debug) << module_label(*this) << "write: par=" << int(par);
        online_check();
        const param::module_var var = param::map_module_param(par);
        size_t offset;
        if (var == param::module_var::TrigConfig) {
            offset = size_t(par) - size_t(param::module_param::trigconfig0);
        } else {
            offset = 0;
        }
        return read_var(var, offset);
    }

    double
    module::read(const std::string& par, size_t channel)
    {
        log(log::info) << module_label(*this) << "read: par=" << par;
        return read(param::lookup_channel_param(par), channel);
    }

    double
    module::read(param::channel_param par, size_t channel)
    {
        log(log::debug) << module_label(*this) << "read: par=" << int(par);
        online_check();
        channel_check(channel);
        lock_guard guard(lock_);
        double value;
        switch (par) {
        case param::channel_param::trigger_risetime:
            value = channels[channel].trigger_risetime();
            break;
        case param::channel_param::trigger_flattop:
            value = channels[channel].trigger_flattop();
            break;
        case param::channel_param::trigger_threshold:
            value = channels[channel].trigger_threshold();
            break;
        case param::channel_param::energy_risetime:
            value = channels[channel].energy_risetime();
            break;
        case param::channel_param::energy_flattop:
            value = channels[channel].energy_flattop();
            break;
        case param::channel_param::tau:
            value = channels[channel].tau();
            break;
        case param::channel_param::trace_length:
            value = channels[channel].trace_length();
            break;
        case param::channel_param::trace_delay:
            value = channels[channel].trace_delay();
            break;
        case param::channel_param::voffset:
            value = channels[channel].voffset();
            break;
        case param::channel_param::xdt:
            value = channels[channel].xdt();
            break;
        case param::channel_param::baseline_percent:
            value = channels[channel].baseline_percent();
            break;
        case param::channel_param::emin:
            value = channels[channel].emin();
            break;
        case param::channel_param::binfactor:
            value = channels[channel].binfactor();
            break;
        case param::channel_param::baseline_average:
            value = channels[channel].baseline_average();
            break;
        case param::channel_param::channel_csra:
            value = channels[channel].csra();
            break;
        case param::channel_param::channel_csrb:
            value = channels[channel].csrb();
            break;
        case param::channel_param::blcut:
            value = channels[channel].bl_cut();
            break;
        case param::channel_param::integrator:
            value = channels[channel].integrator();
            break;
        case param::channel_param::fasttrigbacklen:
            value = channels[channel].fast_trig_backlen();
            break;
        case param::channel_param::cfddelay:
            value = channels[channel].cfd_delay();
            break;
        case param::channel_param::cfdscale:
            value = channels[channel].cfd_scale();
            break;
        case param::channel_param::cfdthresh:
            value = channels[channel].cfd_thresh();
            break;
        case param::channel_param::qdclen0:
        case param::channel_param::qdclen1:
        case param::channel_param::qdclen2:
        case param::channel_param::qdclen3:
        case param::channel_param::qdclen4:
        case param::channel_param::qdclen5:
        case param::channel_param::qdclen6:
        case param::channel_param::qdclen7:
            value = channels[channel].qdc_len(par);
            break;
        case param::channel_param::exttrigstretch:
            value = channels[channel].ext_trig_stretch();
            break;
        case param::channel_param::vetostretch:
            value = channels[channel].veto_stretch();
            break;
        case param::channel_param::multiplicitymaskl:
            value = channels[channel].multiplicity_mask_l();
            break;
        case param::channel_param::multiplicitymaskh:
            value = channels[channel].multiplicity_mask_h();
            break;
        case param::channel_param::externdelaylen:
            value = channels[channel].extern_delay_len();
            break;
        case param::channel_param::ftrigoutdelay:
            value = channels[channel].ftrig_out_delay();
            break;
        case param::channel_param::chantrigstretch:
            value = channels[channel].chan_trig_stretch();
            break;
        default:
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "invalid read parameter");
        }
        return value;
    }

    bool
    module::write(const std::string& par, param::value_type value)
    {
        log(log::info) << module_label(*this)
                       << "write: par=" << par << " value=" << value;
        return write(param::lookup_module_param(par), value);
    }

    bool
    module::write(param::module_param par, param::value_type value)
    {
        log(log::debug) << module_label(*this)
                        << "write: par=" << int(par) << " value=" << value;
        online_check();
        std::ostringstream oss;
        size_t offset = 0;
        bool bcast = false;
        lock_guard guard(lock_);
        switch (par) {
        case param::module_param::module_csrb:
            module_csrb(value);
            break;
        case param::module_param::slow_filter_range:
            slow_filter_range(value);
            break;
        case param::module_param::fast_filter_range:
            fast_filter_range(value);
            break;
        case param::module_param::synch_wait:
        case param::module_param::in_synch:
        case param::module_param::host_rt_preset:
            bcast = true;
            /* fall through */
        case param::module_param::module_format:
        case param::module_param::max_events:
            write_var(param::map_module_param(par), value, offset);
            break;
        case param::module_param::trigconfig0:
        case param::module_param::trigconfig1:
        case param::module_param::trigconfig2:
        case param::module_param::trigconfig3:
            offset = size_t(par) - size_t(param::module_param::trigconfig0);
            /* fall through */
        case param::module_param::module_csra:
        case param::module_param::fasttrigbackplaneena:
        case param::module_param::crateid:
        case param::module_param::slotid:
        case param::module_param::modid:
            write_var(param::map_module_param(par), value, offset);
            hw::run::control(*this, hw::run::control_task::program_fippi);
            break;
        default:
            oss << "invalid module parameter: " << int(par);
            throw error(number, slot,
                        error::code::module_invalid_param, oss.str());
            break;
        }
        return bcast;
    }

    void
    module::write(const std::string& par, size_t channel, double value)
    {
        log(log::info) << module_label(*this)
                       << "write: par=" << par
                       << " channel=" << channel
                       << " value=" << value;
        write(param::lookup_channel_param(par), channel, value);
    }

    void
    module::write(param::channel_param par, size_t channel, double value)
    {
        log(log::debug) << module_label(*this)
                        << "write: par=" << int(par)
                        << " channel=" << channel
                        << " value=" << value;
        online_check();
        channel_check(channel);
        std::ostringstream oss;
        lock_guard guard(lock_);
        switch (par) {
        case param::channel_param::trigger_risetime:
            channels[channel].trigger_risetime(value);
            break;
        case param::channel_param::trigger_flattop:
            channels[channel].trigger_flattop(value);
            break;
        case param::channel_param::trigger_threshold:
            channels[channel].trigger_threshold(value);
            break;
        case param::channel_param::energy_risetime:
        case param::channel_param::energy_flattop:
            channels[channel].energy_risetime_flattop(par, value);
            break;
        case param::channel_param::tau:
            channels[channel].tau(value);
            break;
        case param::channel_param::trace_length:
            channels[channel].trace_length(value);
            break;
        case param::channel_param::trace_delay:
            channels[channel].trace_delay(value);
            break;
        case param::channel_param::voffset:
            channels[channel].voffset(value);
            break;
        case param::channel_param::xdt:
            channels[channel].xdt(value);
            break;
        case param::channel_param::baseline_percent:
            channels[channel].baseline_percent(value);
            break;
        case param::channel_param::emin:
            channels[channel].emin(value);
            break;
        case param::channel_param::binfactor:
            channels[channel].binfactor(value);
            break;
        case param::channel_param::baseline_average:
            channels[channel].baseline_average(value);
            break;
        case param::channel_param::channel_csra:
            channels[channel].csra(value);
            break;
        case param::channel_param::channel_csrb:
            channels[channel].csrb(value);
            break;
        case param::channel_param::blcut:
            channels[channel].bl_cut(value);
            break;
        case param::channel_param::integrator:
            channels[channel].integrator(value);
            break;
        case param::channel_param::fasttrigbacklen:
            channels[channel].fast_trig_backlen(value);
            break;
        case param::channel_param::cfddelay:
            channels[channel].cfd_delay(value);
            break;
        case param::channel_param::cfdscale:
            channels[channel].cfd_scale(value);
            break;
        case param::channel_param::cfdthresh:
            channels[channel].cfd_thresh(value);
            break;
        case param::channel_param::qdclen0:
        case param::channel_param::qdclen1:
        case param::channel_param::qdclen2:
        case param::channel_param::qdclen3:
        case param::channel_param::qdclen4:
        case param::channel_param::qdclen5:
        case param::channel_param::qdclen6:
        case param::channel_param::qdclen7:
            channels[channel].qdc_len(par, value);
            break;
        case param::channel_param::exttrigstretch:
            channels[channel].ext_trig_stretch(value);
            break;
        case param::channel_param::vetostretch:
            channels[channel].veto_stretch(value);
            break;
        case param::channel_param::multiplicitymaskl:
            channels[channel].multiplicity_mask_l(value);
            break;
        case param::channel_param::multiplicitymaskh:
            channels[channel].multiplicity_mask_h(value);
            break;
        case param::channel_param::externdelaylen:
            channels[channel].extern_delay_len(value);
            break;
        case param::channel_param::ftrigoutdelay:
            channels[channel].ftrig_out_delay(value);
            break;
        case param::channel_param::chantrigstretch:
            channels[channel].chan_trig_stretch(value);
            break;
        default:
            break;
        }
    }

    param::value_type
    module::read_var(const std::string& var,
                     size_t channel,
                     size_t offset,
                     bool io)
    {
        log(log::info) << module_label(*this)
                       << "read: var=" << var
                       << " channel=" << channel
                       << " offset=" << offset
                       << " io=" << io;
        try {
            return read_var(param::lookup_module_var(var), offset, io);
        } catch (pixie::error::error& e) {
            if (e.type != error::code::module_invalid_var) {
                throw;
            }
        }
        return read_var(param::lookup_channel_var(var), channel, offset, io);
    }

    param::value_type
    module::read_var(param::module_var var, size_t offset, bool io)
    {
        log(log::debug) << module_label(*this)
                        << "read: var=" << int(var)
                        << " offset=" << offset;
        online_check();
        const size_t index = static_cast<size_t>(var);
        if (index >= module_var_descriptors.size()) {
            std::ostringstream oss;
            oss << "invalid module variable: " << index;
            throw error(number, slot,
                        error::code::module_invalid_param, oss.str());
        }
        const auto& desc = module_var_descriptors[index];
        log(log::debug) << "module: read: " << desc.name;
        if (desc.state == param::disable) {
            throw error(number, slot,
                        error::code::module_param_disabled,
                        "module variable disabled: " + desc.name);
        }
        if (desc.mode == param::wr) {
            throw error(number, slot,
                        error::code::module_param_writeonly,
                        "module variable not readable: " + desc.name);
        }
        if (offset >= desc.size) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "invalid module variable offset: " + desc.name);
        }
        lock_guard guard(lock_);
        param::value_type value;
        if (have_hardware && io) {
            hw::memory::dsp dsp(*this);
            hw::word mem = dsp.read(desc.address);
            hw::convert(mem, value);
            module_vars[index].value[offset].value = value;
            module_vars[index].value[offset].dirty = false;
        } else {
            value = module_vars[index].value[offset].value;
        }
        return value;
    }

    param::value_type
    module::read_var(param::channel_var var,
                     size_t channel,
                     size_t offset,
                     bool io)
    {
        log(log::debug) << module_label(*this)
                        << "read: var=" << int(var)
                        << " channel=" << channel
                        << " offset=" << offset
                        << " io=" << io;
        online_check();
        channel_check(channel);
        const size_t index = static_cast<size_t>(var);
        if (index >= channel_var_descriptors.size()) {
            std::ostringstream oss;
            oss << "invalid channel variable: " << index;
            throw error(number, slot,
                        error::code::channel_invalid_param, oss.str());
        }
        const auto& desc = channel_var_descriptors[index];
        if (desc.state == param::disable) {
            throw error(number, slot,
                        error::code::channel_param_disabled,
                        "channel variable disabled: " + desc.name);
        }
        if (desc.mode == param::wr) {
            throw error(number, slot,
                        error::code::channel_param_writeonly,
                        "channel variable not readable: " + desc.name);
        }
        if (offset >= desc.size) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "invalid channel variable offset: " + desc.name);
        }
        lock_guard guard(lock_);
        param::value_type value;
        if (have_hardware && io) {
            hw::memory::dsp dsp(*this);
            hw::convert(dsp.read(channel, desc.address), value);
            channels[channel].vars[index].value[offset].value = value;
            channels[channel].vars[index].value[offset].dirty = false;
        } else {
            value = channels[channel].vars[index].value[offset].value;
        }
        return value;
    }

    void
    module::write_var(const std::string& var,
                      param::value_type value,
                      size_t channel,
                      size_t offset,
                      bool io)
    {
        log(log::info) << module_label(*this)
                       << "write: var=" << var
                       << " channel=" << channel
                       << " value[" << offset << "]=" << value
                       << " (0x" << std::hex << value << ')';
        try {
            write_var(param::lookup_module_var(var), value, offset, io);
        } catch (pixie::error::error& e) {
            if (e.type != error::code::module_invalid_var) {
                throw;
            }
            write_var(param::lookup_channel_var(var), value, channel, offset, io);
        }
    }

    void
    module::write_var(param::module_var var,
                      param::value_type value,
                      size_t offset,
                      bool io)
    {
        log(log::debug) << module_label(*this)
                        << "write: var=" << int(var)
                        << " value[" << offset << "]=" << value
                        << " (0x" << std::hex << value << ')';
        online_check();
        const size_t index = static_cast<size_t>(var);
        if (index >= module_var_descriptors.size()) {
            std::ostringstream oss;
            oss << "invalid module variable: " << index;
            throw error(number, slot,
                        error::code::module_invalid_param, oss.str());
        }
        const auto& desc = module_var_descriptors[index];
        if (desc.state == param::disable) {
            throw error(number, slot,
                        error::code::module_param_disabled,
                        "module variable disabled: " + desc.name);
        }
        if (desc.mode == param::ro) {
            throw error(number, slot,
                        error::code::module_param_readonly,
                        "module variable not writeable: " + desc.name);
        }
        if (offset >= desc.size) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "invalid module variable offset: " + desc.name);
        }
        lock_guard guard(lock_);
        module_vars[index].value[offset].value = value;
        module_vars[index].value[offset].dirty = true;
        if (have_hardware && io) {
            hw::word word;
            hw::convert(value, word);
            hw::memory::dsp dsp(*this);
            dsp.write(desc.address, word);
            module_vars[index].value[offset].dirty = false;
        }
    }

    void
    module::write_var(param::channel_var var,
                      param::value_type value,
                      size_t channel,
                      size_t offset,
                      bool io)
    {
        log(log::debug) << module_label(*this)
                        << "write: var=" << int(var)
                        << " channel=" << channel
                        << " value[" << offset << "]=" << value
                        << " (0x" << std::hex << value << ')';
        online_check();
        channel_check(channel);
        const size_t index = static_cast<size_t>(var);
        if (index >= channel_var_descriptors.size()) {
            std::ostringstream oss;
            oss << "invalid channel variable: " << index;
            throw error(number, slot,
                        error::code::channel_invalid_param, oss.str());
        }
        const auto& desc = channel_var_descriptors[index];
        if (desc.state == param::disable) {
            throw error(number, slot,
                        error::code::channel_param_disabled,
                        "channel variable disabled: " + desc.name);
        }
        if (desc.mode == param::ro) {
            throw error(number, slot,
                        error::code::channel_param_readonly,
                        "channel variable not writeable: " + desc.name);
        }
        if (offset >= desc.size) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "invalid channel variable offset: " + desc.name);
        }
        lock_guard guard(lock_);
        channels[channel].vars[index].value[offset].value = value;
        channels[channel].vars[index].value[offset].dirty = true;
        if (have_hardware && io) {
            hw::word word;
            hw::convert(value, word);
            hw::memory::dsp dsp(*this);
            dsp.write(channel, desc.address, word);
            channels[channel].vars[index].value[offset].dirty = false;
        }
    }

    void
    module::sync_vars()
    {
        lock_guard guard(lock_);
        hw::memory::dsp dsp(*this);
        for (auto& var : module_vars) {
            const auto& desc = var.var;
            if (desc.state == param::enable &&
                desc.mode != param::ro) {
                for (auto& value : var.value) {
                    if (value.dirty) {
                        if (have_hardware) {
                            hw::word word;
                            hw::convert(value.value, word);
                            dsp.write(desc.address, word);
                        }
                        value.dirty = false;
                    }
                }
            }
        }
        for (auto& channel : channels) {
            for (auto& var : channel.vars) {
                const auto& desc = var.var;
                if (desc.state == param::enable &&
                    desc.mode != param::ro) {
                    for (auto& value : var.value) {
                        if (value.dirty) {
                            if (have_hardware) {
                                hw::word word;
                                hw::convert(value.value, word);
                                dsp.write(desc.address, word);
                                value.dirty = false;
                            }
                        }
                    }
                }
            }
        }
    }

    void
    module::run_end()
    {
        online_check();
        lock_guard guard(lock_);
        if (run_task == hw::run::run_task::nop) {
            log(log::warning) << module_label(*this)
                              << "run-end: no run active";
        } else {
            hw::run::end(*this);
        }
    }

    bool
    module::run_active()
    {
        online_check();
        lock_guard guard(lock_);
        return hw::run::active(*this);
    }

    void
    module::acquire_baselines()
    {
        log(log::info) << module_label(*this) << "acquire-baselines";
        online_check();
        lock_guard guard(lock_);
        hw::run::control(*this, hw::run::control_task::get_baselines);
    }

    void
    module::adjust_offsets()
    {
        log(log::info) << module_label(*this) << "adjust-offsets";
        online_check();
        lock_guard guard(lock_);
        hw::run::control(*this, hw::run::control_task::adjust_offsets);
    }

    void
    module::get_traces()
    {
        log(log::info) << module_label(*this) << "get-traces";
        online_check();
        lock_guard guard(lock_);
        hw::run::control(*this, hw::run::control_task::get_traces);
    }

    void
    module::set_dacs()
    {
        log(log::info) << module_label(*this) << "set-dacs";
        online_check();
        lock_guard guard(lock_);
        hw::run::control(*this, hw::run::control_task::set_dacs);
    }

    void
    module::start_histograms(hw::run::run_mode mode)
    {
        log(log::info) << module_label(*this)
                       << "start-histograms: mode=" << int(mode);
        online_check();
        lock_guard guard(lock_);
        if (run_task != hw::run::run_task::nop) {
            throw error(number, slot,
                        error::code::module_invalid_operation,
                        "module already runinng a task");
        }
        hw::run::run(*this, mode, hw::run::run_task::histogram);
    }

    void
    module::start_listmode(hw::run::run_mode mode)
    {
        log(log::info) << module_label(*this)
                       << "start-listmode: mode=" << int(mode);
        online_check();
        lock_guard guard(lock_);
        if (run_task != hw::run::run_task::nop) {
            throw error(number, slot,
                        error::code::module_invalid_operation,
                        "module already runinng a task");
        }
        fifo_data.flush();
        hw::run::run(*this, mode, hw::run::run_task::list_mode);
    }

    void
    module::read_adc(size_t channel,
                     hw::adc_word* buffer,
                     size_t size,
                     bool run)
    {
        log(log::info) << module_label(*this)
                       << "read-adc: channel=" << channel
                       << " size=" << size
                       << " run=" << std::boolalpha << run;
        online_check();
        lock_guard guard(lock_);
        channel::channel& chan = channels[channel];
        if (run) {
            get_traces();
        }
        if (control_task != hw::run::control_task::get_traces) {
            throw error(number, slot,
                        error::code::module_invalid_operation,
                        "control task not `get_traces`");
        }
        chan.read_adc(buffer, size);
    }

    void
    module::read_adc(size_t channel, hw::adc_trace& buffer, bool run)
    {
        read_adc(channel, buffer.data(), buffer.size(), run);
    }

    void
    module::bl_find_cut(channel::range& channels_, param::values& cuts)
    {
        log(log::info) << module_label(*this)
                       << "bl-find-cut: channels=" << channels.size();
        cuts.clear();
        channel::baseline bl(*this, channels_);
        lock_guard guard(lock_);
        bl.find_cut();
        cuts = bl.cuts;
    }

    void
    module::bl_get(channel::range& channels_,
                   channel::baseline::channels_values& values,
                   bool run)
    {
        log(log::info) << module_label(*this)
                       << "bl-get: channels=" << channels.size();
        channel::baseline bl(*this, channels_);
        lock_guard guard(lock_);
        if (control_task != hw::run::control_task::get_baselines) {
            throw error(number, slot,
                        error::code::module_invalid_operation,
                        "control task `get_baseline` has not run");
        }
        bl.get(values, run);
    }

    void
    module::read_histogram(size_t channel, hw::words& values)
    {
        log(log::info) << module_label(*this)
                       << "read-histogram: channel=" << channel
                       << " length=" << values.size();
        online_check();
        lock_guard guard(lock_);
        if (run_task != hw::run::run_task::histogram) {
            throw error(number, slot,
                        error::code::module_invalid_operation,
                        "run task not `histogram`");
        }
        channels[channel].read_histogram(values);
    }

    void
    module::read_histogram(size_t channel,
                           hw::word_ptr values,
                           const size_t size)
    {
        log(log::info) << module_label(*this)
                       << "read-histogram: channel=" << channel
                       << " length=" << size;
        online_check();
        lock_guard guard(lock_);
        if (run_task != hw::run::run_task::histogram) {
            throw error(number, slot,
                        error::code::module_invalid_operation,
                        "run task not `histogram`");
        }
        channels[channel].read_histogram(values, size);
    }

    size_t
    module::read_list_mode_level()
    {
        log(log::info) << module_label(*this)
                       << "read-list-mode-level";
        online_check();
        if (!fifo_worker_running.load()) {
            log(log::info) << module_label(*this)
                           << "read-list-mode-level: FIFO worker not running";
        }
        return fifo_data.size();
    }

    void
    module::read_list_mode(hw::words& values)
    {
        log(log::info) << module_label(*this)
                       << "read-list-mode: length=" << values.size()
                       << " fifo-size=" << fifo_data.size();
        online_check();
        if (!fifo_worker_running.load()) {
            log(log::warning) << module_label(*this)
                              << "read-list-mode: FIFO worker not running";
        }
        if (!fifo_data.empty()) {
            lock_guard guard(lock_);
            fifo_data.copy(values);
            log(log::debug) << module_label(*this)
                            << "read-list-mode: values=" << values.size()
                            << " fifo-size=" << fifo_data.size();
        }
    }

    void
    module::read_list_mode(hw::word_ptr values, const size_t size)
    {
        log(log::info) << module_label(*this)
                       << "read-list-mode: length=" << size
                       << " fifo-size=" << fifo_data.size();
        online_check();
        if (!fifo_data.empty()) {
            lock_guard guard(lock_);
            fifo_data.copy(values, size);
        }
    }

    void
    module::read_stats(stats::stats& stats)
    {
        log(log::info) << module_label(*this)
                       << "bl-find-count: channels=" << channels.size();
        online_check();
        lock_guard guard(lock_);
        stats::read(*this, stats);
    }

    void
    module::output(std::ostream& out) const
    {
        util::ostream_guard flags(out);
        out << std::boolalpha
            << "number: " << std::setw(2) << number
            << " slot: " << std::setw(2) << slot
            << " present:" << present_.load()
            << " online:" << online_.load()
            << " serial:" << serial_num
            << " rev:" << revision_label()
            << " (" << revision
            << ") vaddr:" << vmaddr
            << " fw: " << firmware.size();
    }

    char
    module::revision_label() const
    {
        return static_cast<char>(revision + 55);
    }

    void
    module::dma_read(const hw::address source, hw::words& values)
    {
        dma_read(source, values.data(), values.size());
    }

    void
    module::dma_read(const hw::address source,
                     hw::word_ptr values,
                     const size_t size)
    {
        log(log::debug) << module_label(*this)
                        << "dma read: addr=0x" << std::hex << source
                        << " length=" << std::dec << size;

        online_check();

        if (bus_lock_.try_lock()) {
            bus_lock_.unlock();
            throw error(number, slot,
                        error::code::device_dma_failure,
                        "bus lock not held");
        }

        util::timepoint tp;
        tp.start();

        PLX_DMA_PARAMS dma_params;

        memset(&dma_params, 0, sizeof(PLX_DMA_PARAMS));

#if PLX_SDK_VERSION_MAJOR < 6
        dma_params.u.UserVa = static_cast<PLX_UINT_PTR>(values);
        dma_params.LocalToPciDma = 1;
#else
        dma_params.UserVa = PLX_PTR_TO_INT(values);
        dma_params.Direction = PLX_DMA_LOC_TO_PCI;
#endif
        dma_params.LocalAddr = source;
        dma_params.ByteCount = size * sizeof(hw::words::value_type);

        /*
         * Wait while reading. The call will block until the interrupt happens.
         */
        PLX_STATUS ps = ::PlxPci_DmaTransferUserBuffer(&device->handle,
                                                       0,
                                                       &dma_params,
                                                       5 * 1000);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "DMA read: " << pci_error_text(ps);
            throw error(number, slot,
                        error::code::device_dma_failure,
                        oss.str());
        }

        tp.end();

        log(log::debug) << module_label(*this)
                        << "dma read: done, period=" << tp;
    }

    bool
    module::operator==(const rev_tag rev) const
    {
        return revision == int(rev);
    }

    bool
    module::operator!=(const rev_tag rev) const
    {
        return revision != int(rev);
    }

    bool
    module::operator>=(const rev_tag rev) const
    {
        return revision >= int(rev);
    }

    bool
    module::operator<=(const rev_tag rev) const
    {
        return revision <= int(rev);
    }

    bool
    module::operator<(const rev_tag rev) const
    {
        return revision < int(rev);
    }

    bool
    module::operator>(const rev_tag rev) const
    {
        return revision > int(rev);
    }

    void
    module::erase_values()
    {
        module_vars.clear();
        channels.clear();
    }

    void
    module::init_values()
    {
        if (num_channels == 0) {
            throw error(number, slot,
                        error::code::internal_failure,
                        "number of channels is 0");
        }
        if (configs.size() != num_channels) {
            throw error(number, slot,
                        error::code::internal_failure,
                        "invalid number of channels configurations");
        }

        firmware::firmware_ref vars = get("var");
        param::load(vars,
                    module_var_descriptors,
                    channel_var_descriptors);

        param_addresses.set(num_channels,
                            module_var_descriptors,
                            channel_var_descriptors);

        log(log::info) << module_label(*this)
                       << "address map: " << param_addresses;

        erase_values();
        for (const auto& desc : module_var_descriptors) {
            module_vars.push_back(param::module_variable(desc));
        }
        channels.resize(num_channels, channel::channel(*this));
        for (size_t channel = 0; channel < num_channels; ++channel) {
            channels[channel].number = channel;
            channels[channel].config = configs[channel];
            for (const auto& desc : channel_var_descriptors) {
                channels[channel].vars.push_back(param::channel_variable(desc));
            }
        }

    }

    bool
    module::eeprom_v2() const
    {
        return false;
    }

    void
    module::module_csrb(param::value_type value)
    {
        write_var(param::module_var::ModCSRB, value);
        hw::run::control(*this, hw::run::control_task::program_fippi);

        bus_guard guard(*this);

        param::value_type csr = 0xaa;

        /*
         * Set up Pull-up resistors
         */
        if ((value & (1 << MODCSRB_CPLDPULLUP)) != 0) {
            csr |= 1 << MODCSRB_CPLDPULLUP;
        } else {
            csr &= ~(1 << MODCSRB_CPLDPULLUP);
        }

        /*
         * Enable connections of PXI nearest neighbor lines (J2) onto the
         * backplane if the module is a Rev-B or C module
         */
        if (*this == rev_B || *this == rev_C) {
            csr |= 1 << CPLDCSR_BPCONNECT;
        }

        write_word(CFG_CTRLCS, csr);

        /*
         * Set pullups for the SYNCH lines on the backplane
         */
        csr = hw::csr::read(*this);

        if ((csr & (1 << MODCSRB_CHASSISMASTER)) != 0) {
            csr |= 1 << PULLUP_CTRL;
        } else {
            csr &= ~(1 << PULLUP_CTRL);
        }
        hw::csr::write(*this, csr);
    }

    void
    module::slow_filter_range(param::value_type value)
    {
        if (value < SLOWFILTERRANGE_MIN) {
            std::stringstream oss;
            oss << "slow filter value below min: " << value;
            throw error(number, slot,
                        error::code::module_invalid_param,
                        oss.str());
        }
        if (value > SLOWFILTERRANGE_MAX) {
            std::stringstream oss;
            oss << "slow filter value above max: " << value;
            throw error(number, slot,
                        error::code::module_invalid_param,
                        oss.str());
        }

        write_var(param::module_var::SlowFilterRange, value);

        /*
         * Recompute the FIFO settings
         */
        value = 1 << read_var(param::module_var::FastFilterRange, 0, false);
        for (size_t channel = 0; channel < num_channels; ++channel) {
            param::value_type paf_length =
                read_var(param::channel_var::PAFlength, channel, 0, false);
            param::value_type trigger_delay =
                read_var(param::channel_var::TriggerDelay, channel, 0, false);
            channels[channel].update_fifo(paf_length - (trigger_delay / value));
        }

        /*
         * Apply the settings to the FIPPI FPGA
         */
        hw::run::control(*this, hw::run::control_task::program_fippi);

        /*
         * Update the baseline cut value
         */
        channel::range chans(num_channels);
        channel::range_set(chans);
        channel::baseline bl(*this, chans);
        bl.find_cut();
    }

    void
    module::fast_filter_range(param::value_type value)
    {
        if (value > FASTFILTERRANGE_MAX) {
            value = FASTFILTERRANGE_MAX;
            log(log::warning) << "setting FAST_FILTER_RANGE to max: "
                              << FASTFILTERRANGE_MAX;
        }

#if FASTFILTERRANGE_MIN > 0
        if (value < FASTFILTERRANGE_MIN) {
            value = FASTFILTERRANGE_MIN;
            log(log::warning) << "setting FAST_FILTER_RANGE to min: "
                              << FASTFILTERRANGE_MIN;
        }
#endif

        param::value_type last_ffr =
            1 << read_var(param::module_var::FastFilterRange, 0, false);

        write_var(param::module_var::FastFilterRange, value);

        /*
         * Recompute the FIFO settings
         */
        for (size_t channel = 0; channel < num_channels; ++channel) {
            param::value_type paf_length =
                read_var(param::channel_var::PAFlength, channel, 0, false);
            param::value_type trigger_delay =
                read_var(param::channel_var::TriggerDelay, channel, 0, false);
            channels[channel].update_fifo(paf_length - (trigger_delay / last_ffr));
        }

        /*
         * Apply the settings to the FIPPI FPGA
         */
        hw::run::control(*this, hw::run::control_task::program_fippi);
    }

    void
    module::online_check() const
    {
        if (!online()) {
            throw error(number, slot,
                        error::code::module_offline,
                        "module is not online");
        }
    }

    void
    module::channel_check(const size_t channel) const
    {
        if (channel >= num_channels) {
            std::ostringstream oss;
            oss << "invalid channel number: " << channel;
            throw error(number, slot,
                        error::code::channel_number_invalid, oss.str());
        }
    }

    void
    module::start_fifo_services()
    {
        hw::fpga::comms comms(*this);
        if (comms.done()) {
            hw::fpga::fippi fippi(*this);
            if (fippi.done()) {
                hw::csr::reset(*this);
                if (!fifo_pool.valid()) {
                    fifo_pool.create(fifo_buffers, hw::max_dma_block_size);
                    start_fifo_worker();
                    hw::run::end(*this);
                }
            }
        }
    }

    void
    module::stop_fifo_services()
    {
        stop_fifo_worker();
        fifo_data.flush();
        fifo_pool.destroy();
    }

    void
    module::start_fifo_worker()
    {
        log(log::debug) << module_label(*this)
                        << std::boolalpha
                        << "FIFO worker: starting: running="
                        << fifo_worker_running.load();
        if (!fifo_worker_running.load()) {
            fifo_worker_finished = false;
            fifo_worker_running = true;;
            fifo_thread = std::thread(&module::fifo_worker, this);
        }
    }

    void
    module::stop_fifo_worker()
    {
        log(log::debug) << module_label(*this)
                        << std::boolalpha
                        << "FIFO worker: stopping: running="
                        << fifo_worker_running.load();
        fifo_worker_running = false;
        if (fifo_thread.joinable()) {
            fifo_thread.join();
        }
    }

    void
    module::fifo_worker()
    {
        log(log::info) << module_label(*this)
                       << "FIFO worker: running";

        hw::memory::fifo fifo(*this);

        try {
            size_t wait_time = fifo_run_wait_usecs.load();
            size_t hold_time = 0;

            bool pool_empty_logged = false;
            bool fifo_full_logged = false;

            while (fifo_worker_running.load()) {
                if (!online()) {
                    hw::wait(250 * 1000);
                    continue;
                }

                const hw::run::run_task this_run_tsk = run_task.load();

                /*
                 * If list mode is running the wait period is the currently
                 * configured run period.
                 *
                 * Any other mode will decay the wait period every hold period
                 * until capped at the idle wait period.
                 */
                if (this_run_tsk == hw::run::run_task::list_mode) {
                    wait_time = fifo_run_wait_usecs.load();
                    /*
                     * See if the task is still running? If not the module may
                     * have been directed to stop running by another module.
                     */
                    if (!hw::run::active(*this)) {
                        fifo_worker_running = false;
                        run_task = hw::run::run_task::nop;
                        log(log::info) << module_label(*this)
                                       << "FIFO worker: run not active";
                    }
                } else {
                    const size_t idle_wait_time = fifo_idle_wait_usecs.load();
                    /*
                     * WHen the wait time is equal to the idle period nothing
                     * further happens. The hold time should also be 0 which
                     * means any new data will be held for the hold period.
                     */
                    if (wait_time > idle_wait_time) {
                        wait_time = idle_wait_time;
                    } else if (wait_time < idle_wait_time &&
                               hold_time >= fifo_hold_usecs.load()) {
                        wait_time <<= 1;
                        if (wait_time > idle_wait_time) {
                            wait_time = idle_wait_time;
                        }
                        hold_time = 0;
                    }
                }

                /*
                 * Loop while there is data to read. Read the FIFO if the level
                 * is more than the DMA block size or it has sat in the FIFO
                 * for the hold time. The hold time is reset on every transfer
                 * from the FIFO no matter the run task state. If we are idle
                 * and reading data the hold time is reset so the wait time
                 * only starts to decay once we do not see data.
                 */
                while (fifo_worker_running.load()) {
                    size_t level = fifo.level();
                    if (level == 0 ||
                        (hold_time < fifo_hold_usecs.load() &&
                         level < hw::max_dma_block_size)) {
                        break;
                    }
                    buffer::handle buf;
                    if (fifo_pool.empty()) {
                        if (!pool_empty_logged) {
                            log(log::warning) << module_label(*this)
                                              << "FIFO worker: pool empty,"
                                              <<" compacting queue ...";
                        }
                        fifo_data.compact();
                    }
                    /*
                     * If the pool is still empty after compacting wait
                     * letting the FIFO fill.
                     */
                    if (!fifo_pool.empty()) {
                        log(log::debug) << module_label(*this)
                                        << "FIFO read, level=" << level;
                        buf = fifo_pool.request();
                        if (level > buf->capacity()) {
                            level = buf->capacity();
                        }
                        buf->resize(level);
                        fifo.read(*buf, level);
                        fifo_data.push(buf);
                        hold_time = 0;
                        pool_empty_logged = false;
                        fifo_full_logged = false;
                    } else {
                        if (!pool_empty_logged) {
                            log(log::warning) << module_label(*this)
                                              << "FIFO worker: pool empty";
                            pool_empty_logged = true;
                        }
                        if (!fifo_full_logged &&
                            level >= hw::fifo_size_words) {
                            fifo_full_logged = true;
                            log(log::warning) << module_label(*this)
                                              << "FIFO worker: FIFO full";
                        }
                        break;
                    }
                }

                /*
                 * Nap the wait time.
                 */
                hw::wait(wait_time);

                /*
                 * Update the hold time if below the hold period.
                 */
                if (hold_time < fifo_hold_usecs.load()) {
                    hold_time += wait_time;
                }
            }

            /*
             * Flush the buffers from the queue back into the pool. Any user
             * active calls should be done in a few seconds.
             */
            fifo_data.flush();
            size_t wait_period = 5 * 1000 / 10;
            while (wait_period-- > 0) {
                if (fifo_pool.full()) {
                    break;
                }
                hw::wait(10000);
            }
        } catch (error& e) {
            log(log::error) << "FIFO worker: " << e;
        } catch (std::exception& e) {
            log(log::error) << "FIFO worker: error: " << e.what();
        } catch (...) {
            log(log::error) << "FIFO worker: unhandled exception";
        }

        log(log::info) << module_label(*this)
                       << "FIFO worker: finishing";

        fifo_worker_running = false;
        fifo_worker_finished = true;
    }

    void
    assign(modules& modules_, const number_slots& numbers)
    {
        for (auto number_slot : numbers) {
            for (auto& mod : modules_) {
                if (mod->slot == number_slot.second) {
                    mod->number = number_slot.first;
                    break;
                }
            }
        }
    }

    void
    order_by_number(modules& mods)
    {
        std::sort(mods.begin(),
                  mods.end(),
                  [](module_ptr& a, module_ptr& b) {
                      return a->number < b->number; } );
    }

    void
    order_by_slot(modules& mods)
    {
        std::sort(mods.begin(),
                  mods.end(),
                  [](module_ptr& a, module_ptr& b) {
                      return a->slot < b->slot; } );
    }

    void
    set_number_by_slot(modules& mods)
    {
        order_by_slot(mods);
        int number = 0;
        for (auto& mod : mods) {
            mod->number = number;
            number++;
        }
    }
};
};
};

std::ostream&
operator<<(std::ostream& out, const xia::pixie::module::module& module)
{
    module.output(out);
    return out;
}
