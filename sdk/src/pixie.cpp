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

#include <pixie.h>

#include <pixie_crate.hpp>
#include <pixie_error.hpp>

/*
 * Boot patterns
 */
#define BOOTPATTERN_COMFPGA_BIT   0
#define BOOTPATTERN_SPFPGA_BIT    2
#define BOOTPATTERN_DSPCODE_BIT   3
#define BOOTPATTERN_DSPPAR_BIT    4
#define BOOTPATTERN_PROGFIPPI_BIT 5
#define BOOTPATTERN_SETDACS_BIT   6

/*
 * The crate. We only handle a single crate with the legacy API.
 */
static xia::pixie::crate::crate crate;

PIXIE_EXPORT int PIXIE_API
PixieInitSystem(unsigned short NumModules,
                unsigned short* PXISlotMap,
                unsigned short OfflineMode)
{
    /*
     * Not supported. We now have tools that can access the DSP variables.
     */
    if (OfflineMode != 0) {
        return -11111;
    }

    try {
        crate.initialize(static_cast<size_t>(NumModules));

        /*
         * Only handle the index to slot assignment if the user supplied the
         * number of modules (ie the length of the array) and the array.
         */
        if (NumModules > 0 && PXISlotMap != nullptr) {
            xia::pixie::module::index_slots indexes;
            for (int i = 0; i < static_cast<int>(NumModules); ++i) {
                typedef xia::pixie::module::index_slot index_slot;
                indexes.push_back(index_slot(i, PXISlotMap[i]));
            }
            crate.assign(indexes);
        }
    } catch (xia::pixie::error::error& e) {
        std::cout << e << std::endl;
        return e.return_code();;
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API
PixieExitSystem(unsigned short ModNum)
{
    try {
        xia::pixie::crate::module_handle module(crate, ModNum);
        module.handle.close();
    } catch (xia::pixie::error::error& e) {
        std::cout << e << std::endl;
        return e.return_code();;
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API
PixieReadModuleInfo(unsigned short ModNum,
                    unsigned short* ModRev,
                    unsigned int* ModSerNum,
                    unsigned short* ModADCBits,
                    unsigned short* ModADCMSPS)
{
    (void) ModNum;
    (void) ModRev;
    (void) ModSerNum;
    (void) ModADCBits;
    (void) ModADCMSPS;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API
PixieBootModule(const char* ComFPGAConfigFile,
                const char* SPFPGAConfigFile,
                const char* ,
                const char* DSPCodeFile,
                const char* DSPParFile,
                const char* DSPVarFile,
                unsigned short ModNum,
                unsigned short BootPattern)
{
    try {
        xia::pixie::crate::module_handle module(crate, ModNum);

        typedef xia::pixie::firmware::firmware firmware;

        firmware comm_fw(0, module.handle.revision, "sys");
        firmware fippi_fw(0, module.handle.revision, "fippi");
        firmware dsp_fw(0, module.handle.revision, "dsp");

        comm_fw.filename = ComFPGAConfigFile;
        fippi_fw.filename = SPFPGAConfigFile;
        dsp_fw.filename = DSPCodeFile;

        xia::pixie::firmware::add(crate.firmware, comm_fw);
        xia::pixie::firmware::add(crate.firmware, fippi_fw);
        xia::pixie::firmware::add(crate.firmware, dsp_fw);

        crate.set_firmware();

        bool boot_comm = (BootPattern & BOOTPATTERN_COMFPGA_BIT) != 0;
        bool boot_fippi = (BootPattern & BOOTPATTERN_SPFPGA_BIT) != 0;
        bool boot_dsp = (BootPattern & BOOTPATTERN_DSPCODE_BIT) != 0;

        module.handle.boot(boot_comm, boot_fippi, boot_dsp);
    } catch (xia::pixie::error::error& e) {
        std::cout << e << std::endl;
        return e.return_code();;
    }

    (void) DSPParFile;
    (void) DSPVarFile;
    (void) BootPattern;

    return -11111;
}
