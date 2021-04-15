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
#include <pixie_log.hpp>

/*
 * Make a local type for the log.
 */
typedef xia::pixie::log log;

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

/*
 * Return an error code for an unhandled exception.
 */
static bool throw_unhandled;

/*
 * Type safe subtraction of the module number, 0 means all.
 */
static size_t
api_mod_num(unsigned short mod_num) {
    if (mod_num == 0)
        return 0;
    return static_cast<size_t>(mod_num - 1);
}

PIXIE_EXPORT int PIXIE_API PixieAcquireADCTrace(unsigned short ModNum) {
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieAcquireBaselines(unsigned short ModNum) {
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieAdjustOffsets(unsigned short ModNum) {
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT static void
PixieBootModule(xia::pixie::module::module& module,
                const char* ComFPGAConfigFile,
                const char* SPFPGAConfigFile,
                const char* DSPCodeFile,
                const char* DSPParFile,
                const char* DSPVarFile,
                unsigned short BootPattern)
{
    typedef xia::pixie::firmware::firmware firmware;

    (void) DSPParFile;
    (void) DSPVarFile;

    firmware comm_fw(0, module.revision, "sys");
    firmware fippi_fw(0, module.revision, "fippi");
    firmware dsp_fw(0, module.revision, "dsp");

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

    module.boot(boot_comm, boot_fippi, boot_dsp);
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
    log(log::info) << "PixieBootModule: ModNum=" << ModNum
                   << std::hex
                   << " BootPattern=0x" << BootPattern;
    log(log::info) << "PixieBootModule: ModNum=" << ModNum
                   << " ComFPGAConfigFile=" << ComFPGAConfigFile;
    log(log::info) << "PixieBootModule: ModNum=" << ModNum
                   << " SPFPGAConfigFile=" << SPFPGAConfigFile;
    log(log::info) << "PixieBootModule: ModNum=" << ModNum
                   << " DSPCodeFile=" << DSPCodeFile;
    log(log::info) << "PixieBootModule: ModNum=" << ModNum
                   << " DSPParFile=" << DSPParFile;
    log(log::info) << "PixieBootModule: ModNum=" << ModNum
                   << " DSPVarFile=" << DSPVarFile;

    try {
        if (ModNum == 0) {
            for (auto& module : crate.modules) {
                PixieBootModule(module,
                                ComFPGAConfigFile,
                                SPFPGAConfigFile,
                                DSPCodeFile,
                                DSPParFile,
                                DSPVarFile,
                                BootPattern);
            }
        } else {
            xia::pixie::crate::module_handle module(crate, api_mod_num(ModNum));
            PixieBootModule(module.handle,
                            ComFPGAConfigFile,
                            SPFPGAConfigFile,
                            DSPCodeFile,
                            DSPParFile,
                            DSPVarFile,
                            BootPattern);
        }
    } catch (xia::pixie::error::error& e) {
        log(log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        log(log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        log(log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieCheckExternalFIFOStatus(unsigned int* nFIFOWords, unsigned short ModNum) {
    (void) ModNum;
    (void) nFIFOWords;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieCheckRunStatus(unsigned short ModNum) {
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT double PIXIE_API PixieComputeInputCountRate(unsigned int* Statistics, unsigned short ModNum,
                                  unsigned short ChanNum) {
    (void) ModNum;
    (void) Statistics;
    (void) ChanNum;
    return -11111;
}

PIXIE_EXPORT double PIXIE_API PixieComputeLiveTime(unsigned int* Statistics, unsigned short ModNum, unsigned short ChanNum) {
    (void) Statistics;
    (void) ModNum;
    (void) ChanNum;
    return -11111;
}

PIXIE_EXPORT double PIXIE_API PixieComputeOutputCountRate(unsigned int* Statistics, unsigned short ModNum,
                                   unsigned short ChanNum) {
    (void) Statistics;
    (void) ModNum;
    (void) ChanNum;
    return -11111;
}

PIXIE_EXPORT double PIXIE_API PixieComputeProcessedEvents(unsigned int* Statistics, unsigned short ModNum) {
    (void) Statistics;
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT double PIXIE_API PixieComputeRealTime(unsigned int* Statistics, unsigned short ModNum) {
    (void) Statistics;
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieEndRun(unsigned short ModNum) {
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API
PixieExitSystem(unsigned short ModNum)
{
    log(log::info) << "PixieExitSystem: ModNum=" << ModNum;

    try {
        if (ModNum == 0) {
            for (auto& module : crate.modules) {
                module.close();
            }
        } else {
            xia::pixie::crate::module_handle module(crate, api_mod_num(ModNum));
            module.handle.close();
        }
    } catch (xia::pixie::error::error& e) {
        log(log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        log(log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        log(log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API
PixieInitSystem(unsigned short NumModules,
                unsigned short* PXISlotMap,
                unsigned short OfflineMode)
{
    /*
     * Create a log file.
     */
    xia::pixie::logging::start("log", "PixieMsg.txt", log::info, false);

    log(log::info) << "PixieInitSystem: NumModules=" << NumModules
                   << " PXISlotMap=" << PXISlotMap
                   << " OfflineMode=" << OfflineMode;

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
            xia::pixie::module::number_slots numbers;
            for (int i = 0; i < static_cast<int>(NumModules); ++i) {
                typedef xia::pixie::module::number_slot number_slot;
                log(log::info) << "PixieInitSystem: slot map: "
                               << PXISlotMap[i] << " => " << i + 1;
                numbers.push_back(number_slot(i, PXISlotMap[i]));
            }
            crate.assign(numbers);
        }
    } catch (xia::pixie::error::error& e) {
        log(log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        log(log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        log(log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data, unsigned int nFIFOWords,
                                  unsigned short ModNum) {
    (void) ExtFIFO_Data;
    (void) nFIFOWords;
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieReadHistogramFromModule(unsigned int* Histogram, unsigned int NumWords,
                                 unsigned short ModNum, unsigned short ChanNum) {
    (void) Histogram;
    (void) NumWords;
    (void) ModNum;
    (void) ChanNum;
    return -11111;
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

PIXIE_EXPORT int PIXIE_API PixieReadSglChanADCTrace(unsigned short* Trace_Buffer, unsigned int Trace_Length,
                             unsigned short ModNum, unsigned short ChanNum) {
    (void) Trace_Buffer;
    (void) Trace_Length;
    (void) ModNum;
    (void) ChanNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieReadSglChanBaselines(double* Baselines, double* TimeStamps, unsigned short NumBases,
                              unsigned short ModNum, unsigned short ChanNum) {
    (void) Baselines;
    (void) TimeStamps;
    (void) NumBases;
    (void) ModNum;
    (void) ChanNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieReadSglChanPar(const char* ChanParName, double* ChanParData, unsigned short ModNum,
                        unsigned short ChanNum) {
    (void) ChanParName;
    (void) ChanParData;
    (void) ModNum;
    (void) ChanNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieReadSglModPar(const char* ModParName, unsigned int* ModParData, unsigned short ModNum) {
    (void) ModParName;
    (void) ModParData;
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieReadStatisticsFromModule(unsigned int* Statistics, unsigned short ModNum) {
    (void) Statistics;
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieSaveDSPParametersToFile(const char* FileName) {
    (void) FileName;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieSaveHistogramToFile(const char* FileName, unsigned short ModNum) {
    (void) FileName;
    (void) ModNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieStartHistogramRun(unsigned short ModNum, unsigned short mode) {
    (void) ModNum;
    (void) mode;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieStartListModeRun(unsigned short ModNum, unsigned short RunType, unsigned short mode) {
    (void) ModNum;
    (void) RunType;
    (void) mode;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieWriteSglChanPar(const char* ChanParName, double ChanParData, unsigned short ModNum,
                         unsigned short ChanNum) {
    (void) ChanParName;
    (void) ChanParData;
    (void) ModNum;
    (void) ChanNum;
    return -11111;
}

PIXIE_EXPORT int PIXIE_API PixieWriteSglModPar(const char* ModParName, unsigned int ModParData, unsigned short ModNum) {
    (void) ModParName;
    (void) ModParData;
    (void) ModNum;
    return -11111;
}
