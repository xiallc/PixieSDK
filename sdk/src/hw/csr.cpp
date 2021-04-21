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

#include <pixie_module.hpp>

#include <hw/csr.hpp>

#include <pixie16sys_defs.h>

namespace xia
{
namespace pixie
{
namespace hw
{
namespace csr
{
word read(module::module& module)
{
    return module.read_word(CSR_ADDR);
}

void write(module::module& module, word value)
{
    module.write_word(CSR_ADDR, value);
}

void
set(module::module& module, word mask)
{
    write(module, read(module) | mask);
}

void
clear(module::module& module, word mask)
{
    write(module, read(module) & ~mask);
}

set_clear::set_clear(module::module& module_, word mask_)
    : module(module_),
      mask(mask_)
{
    set(module, mask);
}

set_clear::~set_clear()
{
    clear(module, mask);
}

void
fifo_ready_wait(module::module& module, const size_t polls)
{
    size_t count = 0;
    while (count++ < polls) {
        if ((read(module) & (1 << EXTFIFO_WML)) != 0) {
            return;
        }
    }
    throw error(error::code::device_dma_busy,
                "csr: EXT FIFO failed to get ready for read");
}
}
}
}
}
