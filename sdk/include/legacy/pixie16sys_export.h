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

/** @file pixie16sys_export.h
 * @brief Declares internal function prototypes
 * @note This file will be deprecated July 31, 2023
 */

#ifndef XIA_PIXIE16SYS_EXPORT_H
#define XIA_PIXIE16SYS_EXPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PIXIE16SYS_DEFS_H
#include "pixie16sys_defs.h"
#endif


PIXIE16SYS_EXPORT int Pixie_InitSystem(unsigned short NumModules, unsigned short* PXISlotMap,
                                       unsigned short OfflineMode);

PIXIE16SYS_EXPORT int Pixie_ClosePCIDevices(unsigned short ModNum);

PIXIE16SYS_EXPORT int Pixie_Boot_CompFPGA(unsigned short ModNum, unsigned int* Com_FPGA_conf,
                                          unsigned int NumComFPGAconf);

PIXIE16SYS_EXPORT int Pixie_Boot_TrigFPGA(unsigned short ModNum, unsigned int* Trig_FPGA_conf,
                                          unsigned int NumTrigFPGAconf);

PIXIE16SYS_EXPORT int Pixie_Boot_FIPPI(unsigned short ModNum, unsigned int* SP_FPGA_conf, unsigned int NumSPFPGAconf);

PIXIE16SYS_EXPORT int Pixie_Boot_DSP(unsigned short ModNum, unsigned int* DSP_code, unsigned int NumDSPCodeWords);

PIXIE16SYS_EXPORT int Pixie_DSP_Memory_IO(unsigned int* dsp_data,  // DSP data for the I/O
                                          unsigned int dsp_address,  // DSP data memory address
                                          unsigned int nWords,  // Number of DSP data words for the I/O
                                          unsigned short direction,  // I/O direction
                                          unsigned short ModNum);  // The Pixie module for the I/O

PIXIE16SYS_EXPORT int
Pixie_Read_ExtFIFOStatus(unsigned int* nFIFOWords,  // Return the number of 32-bit words in the external FIFO
                         unsigned short ModNum);  // The Pixie module being addressed

PIXIE16SYS_EXPORT int Pixie_ExtFIFO_Read(unsigned int* extfifo_data,  // To receive the external FIFO data
                                         unsigned int nWords,  // Number of external FIFO data words to read
                                         unsigned short ModNum);  // The Pixie module for the I/O

PIXIE16SYS_EXPORT int Pixie_Main_Memory_IO(unsigned int* memory_data,  // Memory data for the I/O
                                           unsigned int memory_address,  // Main memory address
                                           unsigned int nWords,  // Number of data words for the I/O
                                           unsigned short direction,  // I/O direction
                                           unsigned short ModNum);  // The Pixie module for the I/O

PIXIE16SYS_EXPORT int Pixie_Clear_Main_Memory(unsigned int memory_address,  // Main memory address
                                              unsigned int nWords,  // Number of data words for the I/O
                                              unsigned short ModNum);  // The Pixie module for the I/O

PIXIE16SYS_EXPORT int Pixie_Register_IO(unsigned short ModNum,  // the Pixie module to communicate to
                                        unsigned int address,  // register address
                                        unsigned short direction,  // either MOD_READ or MOD_WRITE
                                        unsigned int* value);  // holds or receives the data

PIXIE16SYS_EXPORT void Pixie_ReadCSR(unsigned short ModNum, unsigned int* CSR);

PIXIE16SYS_EXPORT void Pixie_WrtCSR(unsigned short ModNum, unsigned int CSR);

#if __GNUC__
#define PIXIE16_PRINTF(_s, _f) __attribute__ ((format (printf, _s, _f)))
#else
#define PIXIE16_PRINTF(_s, _f)
#endif
#define PIXIE_FUNC __func__

PIXIE16SYS_EXPORT void Pixie_Print_MSG(const char* format, ...) PIXIE16_PRINTF(1, 2);

PIXIE16SYS_EXPORT void Pixie_Print_Error(const char* func, char* format, ...) PIXIE16_PRINTF(2, 3);

PIXIE16SYS_EXPORT void Pixie_Print_Warning(const char* func, char* format, ...) PIXIE16_PRINTF(2, 3);

PIXIE16SYS_EXPORT void Pixie_Print_Info(const char* func, char* format, ...) PIXIE16_PRINTF(2, 3);

PIXIE16SYS_EXPORT void Pixie_Print_Debug(const char* func, char* format, ...) PIXIE16_PRINTF(2, 3);

PIXIE16SYS_EXPORT int I2CM24C64_Write_One_Byte(unsigned short ModNum,  // Pixie module module
                                               unsigned short Address,  // The address to write this byte
                                               char* ByteValue);  // The byte value

PIXIE16SYS_EXPORT int I2CM24C64_Read_One_Byte(unsigned short ModNum,  // Pixie module number
                                              unsigned short Address,  // The address to read this byte
                                              char* ByteValue);  // The byte value

PIXIE16SYS_EXPORT int
I2CM24C64_Page_Write(unsigned short ModNum,  // Pixie module module
                     unsigned short Address,  // The starting address to write this page
                     unsigned short NumBytesToWrite,  // The number of bytes for this page to write
                     char* ByteValue);  // The byte values

PIXIE16SYS_EXPORT int
I2CM24C64_Sequential_Read(unsigned short ModNum,  // Pixie module number
                          unsigned short Address,  // The starting address to read
                          unsigned short NumBytesToRead,  // Number of bytes to read (maximum 32 bytes)
                          char* ByteValue);  // The byte values

PIXIE16SYS_EXPORT int PCF8574_Read_One_Byte (unsigned short ModNum, char *ByteValue );

#ifdef __cplusplus
}
#endif  // End of notice for C++ compilers

#endif  // End of pixie16sys_export.h
