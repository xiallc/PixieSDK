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

#include <hw/fpga.hpp>

namespace xia
{
namespace pixie
{
namespace hw
{
namespace fpga
{
    /*
     * The module bus lock is not held here. Hold at the
     * FPGA instance level.
     */
    control::controls::controls(const uint32_t clear_,
                                const uint32_t set_,
                                const uint32_t done_)
        : clear(clear_),
          set(set_),
          done(done_)
    {
    }

    control::regs::regs(const int DATACS_,
                        const int CTRLCS_,
                        const int RDCS_)
        : DATACS(DATACS_),
          CTRLCS(CTRLCS_),
          RDCS(RDCS_)
    {
    }

    control::control(module::module& module_,
                   const std::string name_,
                   const controls& load_ctrl_,
                   const controls& clear_ctrl_,
                   const regs& reg_,
                   bool trace_)
      : module(module_),
        name(name_),
        load_ctrl(load_ctrl_),
        clear_ctrl(clear_ctrl_),
        reg(reg_),
        trace(trace_)
    {
    }

    void
    control::load(const firmware::image& image, int retries)
    {
        log(log::info) << "fpga-" << name
                       << " [slot " << module.slot
                       << "] load: length=" << image.size()
                       << " retries=" << retries
                       << std::hex
                       << " clear-controls: clear=0x" << clear_ctrl.clear
                       << ",set=0x" << clear_ctrl.set
                       << ",done=0x" << clear_ctrl.done
                       << " load-controls: clear=0x" << load_ctrl.clear
                       << ",set=0x" << load_ctrl.set
                       << ",done=0x" << load_ctrl.done;

        if (image.empty()) {
            throw error(error::code::device_image_failure, "no image loaded");
        }

        bool programmed = false;

        while (!programmed) {
            uint32_t data;

            bool cleared = false;
            int timeout_usec = 250;

            while (!cleared) {
                log(log::debug) << "fpga-" << name
                                << " [slot " << module.slot
                                << "] clearing retries=" << retries;

                /*
                 * Clear the FPGA(s)
                 */
                data = bus_read(reg.RDCS);
                data &= clear_ctrl.clear;
                data |= clear_ctrl.set;
                bus_write(reg.CTRLCS, data);

                /*
                 * Ready the FPGA(s)
                 */
                data = bus_read(reg.RDCS);
                data &= load_ctrl.clear;
                data |= load_ctrl.set;
                bus_write(reg.CTRLCS, data);

                while (true) {
                    wait(10);
                    data = bus_read(reg.RDCS);
                    if ((data & clear_ctrl.done) == clear_ctrl.done) {
                        /*
                         * Clear
                         */
                        cleared = true;
                        break;
                    }
                    timeout_usec -= 10;
                    if (timeout_usec <= 0) {
                        --retries;
                        if (retries <= 0) {
                            throw error(error::code::device_load_failure,
                                        make_what("clear failure"));
                        }
                        break;
                    }
                }
            }

            log(log::debug) << "fpga-" << name
                            << " [slot " << module.slot
                            << "] programming";

            /*
             * Load the data.
             */
            size_t i = 0;
            while (i < image.size()) {
                firmware::image_value_type value;
                value  = image[i++];
                value |= ((firmware::image_value_type) image[i++]) << 8;
                value |= ((firmware::image_value_type) image[i++]) << 16;
                value |= ((firmware::image_value_type) image[i++]) << 24;
                bus_write(reg.DATACS, value);
            }

            log(log::debug) << "fpga-" << name
                            << " [slot " << module.slot
                            << "] waiting for done";

            timeout_usec = 250000;

            while (true) {
                wait(1000);
                auto is_done = done();
                if (is_done) {
                    /*
                     * Programmed
                     */
                    programmed = true;
                    break;
                }
                timeout_usec -= 1000;
                if (timeout_usec <= 0) {
                    --retries;
                    if (retries <= 0) {
                        throw error(error::code::device_load_failure,
                                    make_what("programming failure"));
                    }
                    break;
                }
            }
        }

        log(log::debug) << "fpga-" << name
                        << " [slot " << module.slot
                        << "] done";
    }

    bool
    control::done()
    {
        return (bus_read(reg.RDCS) & load_ctrl.done) == load_ctrl.done;
    }

    void
    control::bus_write(int reg, uint32_t data)
    {
        module.write_word(reg, data);
    }

    uint32_t
    control::bus_read(int reg)
    {
        return module.read_word(reg);
    }

    std::string
    control::make_what(const char* msg)
    {
        std::string what;
        what = "fpga-";
        what += name;
        what += ' ';
        what += msg;
        return what;
    }
};
};
};
};
