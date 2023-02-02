/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file pixie16.h
 * @brief C wrappers for the C++ API that expose the same signature as the legacy code
 */

#ifndef PIXIE_H
#define PIXIE_H

#include <stddef.h>

#include <pixie/os_compat.hpp>

#ifdef __cplusplus
extern "C" {
#endif

struct fifo_worker_config {
    size_t bandwidth_mb_per_sec;
    size_t buffers;
    size_t dma_trigger_level_bytes;
    size_t hold_usecs;
    size_t idle_wait_usecs;
    size_t run_wait_usecs;
};

#define PIXIE16_API_MOD_CONFIG_MAX_STRING (256)
struct module_config {
    unsigned short adc_bit_resolution;
    unsigned short adc_sampling_frequency;
    char sys_fpga[PIXIE16_API_MOD_CONFIG_MAX_STRING];
    char dsp_code[PIXIE16_API_MOD_CONFIG_MAX_STRING];
    char dsp_var[PIXIE16_API_MOD_CONFIG_MAX_STRING];
    unsigned short number;
    unsigned short number_of_channels;
    unsigned short revision;
    unsigned int serial_number;
    unsigned short slot;
    char sp_fpga[PIXIE16_API_MOD_CONFIG_MAX_STRING];
};

struct module_fifo_stats {
    size_t in;
    size_t out;
    size_t dma_in;
    size_t overflows;
    size_t dropped;
    size_t hw_overflows;
};

PIXIE_EXPORT double PIXIE_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);

PIXIE_EXPORT unsigned int PIXIE_API Decimal2IEEEFloating(double DecimalNumber);

PIXIE_EXPORT unsigned int PIXIE_API Pixie16GetStatisticsSize(void);

PIXIE_EXPORT int PIXIE_API PixieGetReturnCodeText(int return_code, char* buf,
                                                  unsigned int buf_size);

PIXIE_EXPORT int PIXIE_API PixieGetHistogramLength(unsigned short mod_num, unsigned short chan_num,
                                                   unsigned int* hist_length);

PIXIE_EXPORT int PIXIE_API PixieGetTraceLength(unsigned short mod_num, unsigned short chan_num,
                                               unsigned int* trace_length);

PIXIE_EXPORT int PIXIE_API PixieGetMaxNumBaselines(unsigned short mod_num, unsigned short chan_num,
                                                   unsigned int* max_num_baselines);

PIXIE_EXPORT int PIXIE_API Pixie16AcquireADCTrace(unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16AcquireBaselines(unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16AdjustOffsets(unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16BLcutFinder(unsigned short ModNum, unsigned short ChanNum,
                                              unsigned int* BLcut);

PIXIE_EXPORT int PIXIE_API Pixie16BootModule(const char* ComFPGAConfigFile,
                                             const char* SPFPGAConfigFile,
                                             const char* TrigFPGAConfigFile,
                                             const char* DSPCodeFile, const char* DSPParFile,
                                             const char* DSPVarFile, unsigned short ModNum,
                                             unsigned short BootPattern);

PIXIE_EXPORT int PIXIE_API Pixie16BootModuleFirmware(const char* DSPParFile, unsigned short ModNum,
                                                    unsigned short BootPattern);

PIXIE_EXPORT int PIXIE_API Pixie16CheckExternalFIFOStatus(unsigned int* nFIFOWords,
                                                          unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16CheckRunStatus(unsigned short ModNum);

PIXIE_EXPORT double PIXIE_API Pixie16ComputeInputCountRate(unsigned int* Statistics,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum);

PIXIE_EXPORT double PIXIE_API Pixie16ComputeLiveTime(unsigned int* Statistics,
                                                     unsigned short ModNum, unsigned short ChanNum);

PIXIE_EXPORT double PIXIE_API Pixie16ComputeOutputCountRate(unsigned int* Statistics,
                                                            unsigned short ModNum,
                                                            unsigned short ChanNum);

PIXIE_EXPORT double PIXIE_API Pixie16ComputeRawInputCount(unsigned int* Statistics,
                                                          unsigned short ModNum,
                                                          unsigned short ChanNum);

PIXIE_EXPORT double PIXIE_API Pixie16ComputeRawOutputCount(unsigned int* Statistics,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum);

PIXIE_EXPORT double PIXIE_API Pixie16ComputeRealTime(unsigned int* Statistics,
                                                     unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16CopyDSPParameters(unsigned short BitMask,
                                                    unsigned short SourceModule,
                                                    unsigned short SourceChannel,
                                                    unsigned short* DestinationMask);

PIXIE_EXPORT int PIXIE_API Pixie16EndRun(unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16ExitSystem(unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16InitSystem(unsigned short NumModules, unsigned short* PXISlotMap,
                                             unsigned short OfflineMode);

PIXIE_EXPORT int PIXIE_API Pixie16LoadModuleFirmware(const char* SearchPath);

PIXIE_EXPORT int PIXIE_API Pixie16SetModuleFirmware(const char* FwFile, unsigned int ModNum, const char* Device);

PIXIE_EXPORT int PIXIE_API Pixie16ReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data,
                                                           unsigned int nFIFOWords,
                                                           unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16LoadDSPParametersFromFile(const char* FileName);

PIXIE_EXPORT int PIXIE_API Pixie16ReadHistogramFromModule(unsigned int* Histogram,
                                                          unsigned int NumWords,
                                                          unsigned short ModNum,
                                                          unsigned short ChanNum);

PIXIE_EXPORT int PIXIE_API Pixie16ReadModuleInfo(unsigned short ModNum, unsigned short* ModRev,
                                                 unsigned int* ModSerNum,
                                                 unsigned short* ModADCBits,
                                                 unsigned short* ModADCMSPS);

PIXIE_EXPORT int PIXIE_API PixieGetModuleInfo(unsigned short mod_num, struct module_config* cfg);

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanADCTrace(unsigned short* Trace_Buffer,
                                                      unsigned int Trace_Length,
                                                      unsigned short ModNum,
                                                      unsigned short ChanNum);

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanBaselines(double* Baselines, double* TimeStamps,
                                                       unsigned short NumBases,
                                                       unsigned short ModNum,
                                                       unsigned short ChanNum);

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanPar(const char* ChanParName, double* ChanParData,
                                                 unsigned short ModNum, unsigned short ChanNum);

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglModPar(const char* ModParName, unsigned int* ModParData,
                                                unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16ReadStatisticsFromModule(unsigned int* Statistics,
                                                           unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16SaveDSPParametersToFile(const char* FileName);

PIXIE_EXPORT int PIXIE_API Pixie16SetDACs(unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API Pixie16StartHistogramRun(unsigned short ModNum, unsigned short mode);

PIXIE_EXPORT int PIXIE_API Pixie16StartListModeRun(unsigned short ModNum, unsigned short RunType,
                                                   unsigned short mode);

PIXIE_EXPORT int PIXIE_API Pixie16TauFinder(unsigned short ModNum, double* Tau);

PIXIE_EXPORT int PIXIE_API Pixie16WriteSglChanPar(const char* ChanParName, double ChanParData,
                                                  unsigned short ModNum, unsigned short ChanNum);

PIXIE_EXPORT int PIXIE_API Pixie16WriteSglModPar(const char* ModParName, unsigned int ModParData,
                                                 unsigned short ModNum);

enum PIXIE_BOOT_MODE {
    PIXIE_BOOT_PROBE,
    PIXIE_BOOT_RESET_LOAD,
    PIXIE_BOOT_SETTINGS_LOAD
};

PIXIE_EXPORT int PIXIE_API PixieBootCrate(const char* settings_file,
                                          const enum PIXIE_BOOT_MODE boot_mode);

PIXIE_EXPORT int PIXIE_API PixieGetWorkerConfiguration(unsigned short mod_num,
                                                       struct fifo_worker_config* worker_config);

PIXIE_EXPORT int PIXIE_API PixieRegisterFirmware(const unsigned int version, const int revision,
                                                 const int adc_msps, const int adc_bits,
                                                 const char* device, const char* path,
                                                 unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API PixieSetWorkerConfiguration(unsigned short mod_num,
                                                       struct fifo_worker_config* worker_config);

PIXIE_EXPORT int PIXIE_API PixieReadRunFifoStats(unsigned short mod_num,
                                                 struct module_fifo_stats* fifo_stats);

#ifdef __cplusplus
}
#endif

#endif /* PIXIE_H */