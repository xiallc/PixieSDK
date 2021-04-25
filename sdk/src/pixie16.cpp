/**----------------------------------------------------------------------
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
*----------------------------------------------------------------------**/
/// @file pixie16.cpp
/// @brief C wrappers for the C++ API that expose the same signature as the legacy code
/// @author C. Johns / S. V. Paulauskas
/// @date March 29, 2021

#include <pixie16.h>

#include <pixie_crate.hpp>
#include <pixie_error.hpp>
#include <pixie_log.hpp>

/*
 * Local types for convenience.
 */
typedef xia::log xia_log;
typedef xia::pixie::error::error xia_error;

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
 * Return an error code for an unhandled exception. If true the exception is
 * thrown and you can catch it in a debugger or see it on the console to find
 * out what it is.
 */
static bool throw_unhandled;

static int
not_supported()
{
    int error = xia::pixie::error::api_result_not_supported();
    return xia::pixie::error::return_code(error);
}

PIXIE_EXPORT int PIXIE_API
PixieAcquireADCTrace(unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieAcquireADCTrace: ModNum=" << ModNum;

    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieAcquireBaselines(unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieAcquireBaselines: ModNum=" << ModNum;

    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieAdjustOffsets(unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieAdjustOffsets: ModNum=" << ModNum;

    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
Pixie16BLcutFinder(unsigned short ModNum,
                   unsigned short ChanNum,
                   unsigned int* BLcut)
{
    xia_log(xia_log::info) << "Pixie16BLcutFinder: ModNum=" << ModNum
                           << " ChanNum=" << ChanNum;

    (void) BLcut;
    return not_supported();
}

static void
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
    xia_log(xia_log::info) << "PixieBootModule: ModNum=" << ModNum
                           << std::hex
                           << " BootPattern=0x" << BootPattern;
    xia_log(xia_log::info) << "PixieBootModule: ModNum=" << ModNum
                           << " ComFPGAConfigFile=" << ComFPGAConfigFile;
    xia_log(xia_log::info) << "PixieBootModule: ModNum=" << ModNum
                           << " SPFPGAConfigFile=" << SPFPGAConfigFile;
    xia_log(xia_log::info) << "PixieBootModule: ModNum=" << ModNum
                           << " DSPCodeFile=" << DSPCodeFile;
    xia_log(xia_log::info) << "PixieBootModule: ModNum=" << ModNum
                           << " DSPParFile=" << DSPParFile;
    xia_log(xia_log::info) << "PixieBootModule: ModNum=" << ModNum
                           << " DSPVarFile=" << DSPVarFile;

    try {
        if (ModNum == crate.num_modules) {
            xia::pixie::crate::crate::user user(crate);
            for (auto& module : crate.modules) {
                PixieBootModule(*module,
                                ComFPGAConfigFile,
                                SPFPGAConfigFile,
                                DSPCodeFile,
                                DSPParFile,
                                DSPVarFile,
                                BootPattern);
            }
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            PixieBootModule(*module,
                            ComFPGAConfigFile,
                            SPFPGAConfigFile,
                            DSPCodeFile,
                            DSPParFile,
                            DSPVarFile,
                            BootPattern);
        }
    } catch (xia_error& e) {
        xia_log(xia_log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API
PixieCheckExternalFIFOStatus(unsigned int* nFIFOWords,
                             unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieCheckExternalFIFOStatus: ModNum=" << ModNum;

    (void) nFIFOWords;
    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieCheckRunStatus(unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieCheckRunStatus: ModNum=" << ModNum;

    int result = 0;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        if (module->run_active()) {
            result = 1;
        }
    } catch (xia_error& e) {
        xia_log(xia_log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return result;
}

PIXIE_EXPORT double PIXIE_API
PixieComputeInputCountRate(unsigned int* Statistics,
                           unsigned short ModNum,
                           unsigned short ChanNum)
{
    xia_log(xia_log::info) << "PixieComputeInputCountRate: ModNum=" << ModNum
                           << " ChanNum=" << ChanNum;

    (void) Statistics;
    return not_supported();
}

PIXIE_EXPORT double PIXIE_API
PixieComputeLiveTime(unsigned int* Statistics,
                     unsigned short ModNum,
                     unsigned short ChanNum)
{
    xia_log(xia_log::info) << "PixieComputeLiveTime: ModNum=" << ModNum
                           << " ChanNum=" << ChanNum;

    (void) Statistics;
    return not_supported();
}

PIXIE_EXPORT double PIXIE_API
PixieComputeOutputCountRate(unsigned int* Statistics,
                            unsigned short ModNum,
                            unsigned short ChanNum)
{
    xia_log(xia_log::info) << "PixieComputeOutputCountRate: ModNum=" << ModNum
                           << " ChanNum=" << ChanNum;

    (void) Statistics;
    return not_supported();
}

PIXIE_EXPORT double PIXIE_API
PixieComputeProcessedEvents(unsigned int* Statistics,
                            unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieComputeProcessedEvents: ModNum=" << ModNum;

    (void) Statistics;
    return not_supported();
}

PIXIE_EXPORT double PIXIE_API
PixieComputeRealTime(unsigned int* Statistics,
                     unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieComputeRealTime: ModNum=" << ModNum;

    (void) Statistics;
    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieEndRun(unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieEndRun: ModNum=" << ModNum;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        module->run_end();
    } catch (xia_error& e) {
        xia_log(xia_log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API
PixieExitSystem(unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieExitSystem: ModNum=" << ModNum;

    try {
        crate.ready();
        if (ModNum == crate.num_modules) {
            xia::pixie::crate::crate::user user(crate);
            for (auto& module : crate.modules) {
                module->close();
            }
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            module->close();
        }
    } catch (xia_error& e) {
        xia_log(xia_log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
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
    xia::logging::start("log", "PixieMsg.txt", xia_log::info, false);

    xia_log(xia_log::info) << "PixieInitSystem: NumModules=" << NumModules
                           << " PXISlotMap=" << PXISlotMap
                           << " OfflineMode=" << OfflineMode;

    /*
     * Not supported. We now have tools that can access the DSP variables.
     */
    if (OfflineMode != 0) {
        return not_supported();
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
                xia_log(xia_log::info) << "PixieInitSystem: slot map: "
                               << PXISlotMap[i] << " => " << i + 1;
                numbers.push_back(number_slot(i, PXISlotMap[i]));
            }
            crate.assign(numbers);
        }
    } catch (xia_error& e) {
        xia_log(xia_log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API
PixieReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data,
                              unsigned int nFIFOWords,
                              unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieReadDataFromExternalFIFO: ModNum=" << ModNum
                           << " nFIFOWords=" << nFIFOWords;

    (void) ExtFIFO_Data;
    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieReadHistogramFromModule(unsigned int* Histogram,
                             unsigned int NumWords,
                             unsigned short ModNum,
                             unsigned short ChanNum)
{
    xia_log(xia_log::info) << "PixieReadHistogramFromModule: ModNum=" << ModNum
                           << " ChanNum=" << ChanNum
                           << " NumWords=" << NumWords;

    (void) Histogram;
    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieReadModuleInfo(unsigned short ModNum,
                    unsigned short* ModRev,
                    unsigned int* ModSerNum,
                    unsigned short* ModADCBits,
                    unsigned short* ModADCMSPS)
{
    xia_log(xia_log::info) << "PixieReadModuleInfo: ModNum=" << ModNum;

    (void) ModRev;
    (void) ModSerNum;
    (void) ModADCBits;
    (void) ModADCMSPS;
    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieReadSglChanADCTrace(unsigned short* Trace_Buffer,
                         unsigned int Trace_Length,
                         unsigned short ModNum,
                         unsigned short ChanNum)
{
    xia_log(xia_log::info) << "PixieReadSglChanADCTrace: ModNum=" << ModNum
                           << " ChanNum=" << ChanNum
                           << " Trace_Length=" << Trace_Length;

    (void) Trace_Buffer;
    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieReadSglChanBaselines(double* Baselines,
                          double* TimeStamps,
                          unsigned short NumBases,
                          unsigned short ModNum,
                          unsigned short ChanNum)
{
    xia_log(xia_log::info) << "PixieReadSglChanADCTrace: ModNum=" << ModNum
                           << " ChanNum=" << ChanNum
                           << " NumBases=" << NumBases;

    (void) Baselines;
    (void) TimeStamps;
    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieReadSglChanPar(const char* ChanParName,
                    double* ChanParData,
                    unsigned short ModNum,
                    unsigned short ChanNum)
{
    xia_log(xia_log::info) << "PixieReadSglChanPar: ModNum=" << ModNum
                           << " ChanNum=" << ChanNum
                           << " ChanParName=" << ChanParName;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        *ChanParData = module->read(ChanParName, ChanNum);
        xia_log(xia_log::debug) << "PixieReadSglChanPar: ModNum=" << ModNum
                                << " ChanNum=" << ChanNum
                                << " ChanParName=" << ChanParName
                                << " ChanParData=" << *ChanParData;
    } catch (xia_error& e) {
        xia_log(xia_log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API
PixieReadSglModPar(const char* ModParName,
                   unsigned int* ModParData,
                   unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieReadSglModPar: ModNum=" << ModNum
                           << " ModParName=" << ModParName;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        *ModParData = module->read(ModParName);
        xia_log(xia_log::debug) << "PixieReadSglModPar: ModNum=" << ModNum
                                << " ModParName=" << ModParName
                                << " ModParData=" << *ModParData;
    } catch (xia_error& e) {
        xia_log(xia_log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API
PixieReadStatisticsFromModule(unsigned int* Statistics,
                              unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieReadStatisticsFromModule: ModNum=" << ModNum;

    (void) Statistics;
    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieSaveDSPParametersToFile(const char* FileName)
{
    xia_log(xia_log::info) << "PixieReadStatisticsFromModule: FileName=" << FileName;

    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieSaveHistogramToFile(const char* FileName,
                         unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieSaveHistogramToFile: ModNum=" << ModNum
                           << " FileName=" << FileName;

    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieSetDACs(unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieSetDACs: ModNum=" << ModNum;

    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieStartHistogramRun(unsigned short ModNum,
                       unsigned short mode)
{
    xia_log(xia_log::info) << "PixieStartHistogramRun: ModNum=" << ModNum
                           << " mode=" << mode;

    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieStartListModeRun(unsigned short ModNum,
                      unsigned short RunType,
                      unsigned short mode)
{
    xia_log(xia_log::info) << "PixieStartListModeRun: ModNum=" << ModNum
                           << " RunType=" << RunType
                           << " mode=" << mode;

    return not_supported();
}

PIXIE_EXPORT int PIXIE_API
PixieWriteSglChanPar(const char* ChanParName,
                     double ChanParData,
                     unsigned short ModNum,
                     unsigned short ChanNum)
{
    xia_log(xia_log::info) << "PixieWriteSglChanPar: ModNum=" << ModNum
                           << " ChanNum=" << ChanNum
                           << " ChanParName=" << ChanParName
                           << " ChanParData=" << ChanParData;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        module->write(ChanParName, ChanNum, ChanParData);
    } catch (xia_error& e) {
        xia_log(xia_log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API
PixieWriteSglModPar(const char* ModParName,
                    unsigned int ModParData,
                    unsigned short ModNum)
{
    xia_log(xia_log::info) << "PixieWriteSglModPar: ModNum=" << ModNum
                           << " ModParName=" << ModParName
                           << " ModParData=" << ModParData;

    try {
        crate.ready();
        bool bcast;
        if (ModNum == crate.num_modules) {
            bcast = true;
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            bcast = module->write(ModParName, ModParData);
        }
        if (bcast) {
            xia::pixie::crate::crate::user user(crate);
            for (auto& module : crate.modules) {
                if (ModNum != module->number) {
                    module->write(ModParName, ModParData);
                }
            }
        }
    } catch (xia_error& e) {
        xia_log(xia_log::error) << e;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (throw_unhandled) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::api_result_unknown_error();
    }

    return 0;
}
