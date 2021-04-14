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

#include <iostream>
#include <iomanip>

#include <pixie_module.hpp>
#include <pixie_log.hpp>

#include <hw/dsp.hpp>

#include <pixie16sys_defs.h>
#include <def21160.h>

namespace xia
{
namespace pixie
{
namespace hw
{
namespace dsp
{
    dsp::clear_download::clear_download(dsp& dsp__)
        : dsp_(dsp__) {
    }

    dsp::clear_download::~clear_download() {
        dsp_.bus_write(CSR_ADDR, dsp_.bus_read(CSR_ADDR) & ~DSPDOWNLOAD);
    }

    dsp::dsp(module::module& module_, bool trace_)
        : module(module_),
          online(false),
          trace(trace_)
    {
    }

    dsp&
    dsp::operator=(dsp&& d)
    {
        trace = d.trace;
        online = d.online;
        d.trace = false;
        d.online = false;
        return *this;
    }

    void
    dsp::boot(const firmware::image& image, int retries)
    {
        log(log::info) << "dsp [slot " << module.slot
                       << "] load: length=" << image.size()
                       << " retries=" << retries;

        bool running = false;

        /*
         * Makes sure the guard bit is clear when destructing.
         */
        clear_download download_clear(*this);

        while (!running) {
            --retries;

            try {
                /*
                 * The DSP is loaded using 32 bit PCI writes to MMA of 16 bit
                 * values. We use the reader with a 16 bit word size.
                 */
                firmware::reader reader(image, sizeof(load_value_type));

                /*
                 * The reset self clears, the download bit needs to be cleared.
                 *
                 * The download bit is cleared by the guard instance of @ref
                 * clear_download.
                 */
                static const uint32_t DSP_BOOT =
                    (1 << DSPDOWNLOAD) | (1 << DSPRESET);

                bool ok;

                /*
                 * Reset the DSP and wait 1msec
                 */
                bus_write(CSR_ADDR, bus_read(CSR_ADDR) | DSP_BOOT);
                wait(1000);

                /*
                 * HBR done for reset (not sure what this means!)
                 */
                bus_write(HBR_DONE, 0);
                wait(1000);

                /*
                 * SYSCON address
                 */
                bus_write(EXT_MEM_TEST, SYSCON);

                /*
                 * HBR request
                 */
                ok = checked_write(REQUEST_HBR, SYSCON, WRT_DSP_MMA, 0x10, 1000);
                if (!ok) {
                    throw error(error::code::device_load_failure,
                                make_what("DSP SYSCON set failure"));
                }

                /*
                 * Check DSP DMAC10
                 */
                ok = checked_write(EXT_MEM_TEST, DMAC10, WRT_DSP_MMA, 0xa1);
                if (!ok) {
                    throw error(error::code::device_load_failure,
                                make_what("DSP DMAC10 set failure"));
                }

                /*
                 * Download the boot kernel. Blocks of 3 bus words are written as a
                 * time.
                 */
                bus_write(EXT_MEM_TEST, EPB0);
                for (size_t i = 0; i < 256; ++i) {
                    bus_write(WRT_DSP_MMA, reader.get());
                    bus_write(WRT_DSP_MMA, reader.get());
                    bus_write(WRT_DSP_MMA, reader.get());
                }

                /*
                 * De-assert HBR
                 */
                bus_write(HBR_DONE, 0);

                /*
                 * Load the image
                 */
                while (reader.offset < image.size()) {
                    uint32_t tag = reader.peek();
                    switch (tag) {
                    case ZERO_DM16:
                    case ZERO_PM16:
                    case ZERO_DM32:
                    case ZERO_PM32:
                    case ZERO_DM40:
                    case ZERO_PM40:
                    case ZERO_PM48:
                    case ZERO_DM64:
                    case ZERO_PM64:
                        section_load(reader, 0);
                        wait(2000);
                        break;
                    case INIT_DM16:
                    case INIT_PM16:
                    case INIT_DM32:
                    case INIT_PM32:
                    case INIT_PM48:
                        section_load(reader, sizeof(load_value_type));
                        break;
                    case INIT_DM64:
                    case INIT_PM64:
                        section_load(reader, 2 * sizeof(load_value_type));
                        break;
                    case FINAL_INIT:
                        bus_write(REQUEST_HBR, 0);
                        for (size_t i = 0; i < 258; ++i) {
                            bus_write(WRT_DSP_MMA, reader.get());
                            bus_write(WRT_DSP_MMA, reader.get());
                            bus_write(WRT_DSP_MMA, reader.get());
                        }
                        break;
                    default: {
                        std::ostringstream oss;
                        oss << "DSP image tag invalid: offset="
                            << reader.offset;
                        throw error(error::code::device_image_failure,
                                    make_what(oss.str().c_str()));
                    }
                    }
                }

                /*
                 * Wait until DSP variable PowerUpInitDone is set to 1 by the DSP
                 * after it completes its initialization tasks on hard RESET. This
                 * should prevent premature downloading of DSP parameters by the
                 * host, which would cause corruption of DSP internal memory.
                 */
                size_t retry = 10;
                while (--retry > 0) {
                    if (done()) {
                        running = true;
                        break;
                    }
                    wait(1000);
                }
                if (retry == 0) {
                    throw error(error::code::device_boot_failure,
                                make_what("DSP failed to start"));
                }
            } catch (error& e) {
                if (retries == 0) {
                    throw;
                }
                log(log::error) << "dsp [slot " << module.slot
                                << "] retries: " << retries
                                << ": " << e.what();
            }
        }

        log(log::info) << "dsp [slot " << module.slot
                       << "] running";

        online = true;
    }

    bool
    dsp::done()
    {
        memory value = read(POWERUPINITDONE_ADDRESS);
        return value == 1;
    }

    memory
    dsp::read(const address addr)
    {
        bus_write(REQUEST_HBR, 0);
        bus_write(EXT_MEM_TEST, addr);
        memory value = bus_read(WRT_DSP_MMA);
        bus_write(HBR_DONE, 0);
        return value;
    }

    memory
    dsp::read(const size_t channel, const address addr)
    {
        return read(addr + (channel * sizeof(memory)));
    }

    void
    dsp::write(const address addr, const memory value)
    {
        bus_write(REQUEST_HBR, 0);
        bus_write(EXT_MEM_TEST, addr);
        bus_write(WRT_DSP_MMA, value);
        bus_write(HBR_DONE, 0);
    }

    void
    dsp::write(const size_t channel, const address addr, const memory value)
    {
        write(addr + (channel * sizeof(memory)), value);
    }

    void
    dsp::write(const address addr, const memories& values)
    {
        bus_write(REQUEST_HBR, 0);
        bus_write(EXT_MEM_TEST, addr);
        for (auto value : values) {
            bus_write(WRT_DSP_MMA, value);
        }
        bus_write(HBR_DONE, 0);
    }

    void
    dsp::write(const size_t channel,
               const address addr, const memories& values)
    {
        write(addr + (channel * sizeof(memory)), values);
    }

    void
    dsp::section_load(firmware::reader& reader, const size_t wordsize)
    {
        bus_write(REQUEST_HBR, 0);
        bus_write(WRT_DSP_MMA, reader.get());
        bus_write(WRT_DSP_MMA, reader.get());
        bus_write(WRT_DSP_MMA, reader.get());
        bus_write(HBR_DONE, 0);
        size_t wordcount = reader.peek();
        bus_write(REQUEST_HBR, 0);
        bus_write(WRT_DSP_MMA, reader.get());
        bus_write(WRT_DSP_MMA, reader.get());
        bus_write(WRT_DSP_MMA, reader.get());
        bus_write(HBR_DONE, 0);
        if (wordsize != 0) {
            if ((reader.remaining()) < (wordcount * 3 * wordsize)) {
                throw error(error::code::device_image_failure,
                            make_what("image section too small"));
            }
            wordcount *= wordsize / sizeof(load_value_type);
            for (size_t i = 0; i < wordcount; ++i) {
                bus_write(REQUEST_HBR, 0);
                bus_write(WRT_DSP_MMA, reader.get());
                bus_write(WRT_DSP_MMA, reader.get());
                bus_write(WRT_DSP_MMA, reader.get());
                bus_write(HBR_DONE, 0);
            }
        }
    }

    bool
    dsp::checked_write(const uint32_t out,
                       const uint32_t value,
                       const uint32_t in,
                       const uint32_t result,
                       const int out_wait,
                       const int in_wait)
    {
        int retry = 5;
        bus_write(out, value);
        if (out_wait > 0) {
            wait(out_wait);
        }
        while (--retry > 0) {
            uint32_t data = bus_read(in);
            if (data == result) {
                return true;
            }
            wait(in_wait);
        }
        return false;
    }

    void
    dsp::bus_write(int reg, uint32_t data)
    {
        module.write_32(reg, data);
    }

    uint32_t
    dsp::bus_read(int reg)
    {
        return module.read_32(reg);
    }

    std::string
    dsp::make_what(const char* msg)
    {
        std::string what;
        what = "dsp: ";
        what += msg;
        return what;
    }
};
};
};
};
