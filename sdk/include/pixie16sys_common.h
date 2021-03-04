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
/// @file pixie16sys_common.h
/// @brief Declaration of prototypes of common functions.
/// @author H. Tan
/// @date August 11, 2006

#ifndef XIA_PIXIE16SYS_COMMON_H
#define XIA_PIXIE16SYS_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PIXIE16APP_DEFS_H
#include "pixie16sys_defs.h"
#endif

PIXIE16SYS_EXPORT unsigned short PIXIE16SYS_API SYS16_SetBit(unsigned short bit, unsigned short value);
PIXIE16SYS_EXPORT unsigned short PIXIE16SYS_API SYS16_ClrBit(unsigned short bit, unsigned short value);
PIXIE16SYS_EXPORT unsigned short PIXIE16SYS_API SYS16_TstBit(unsigned short bit, unsigned short value);
PIXIE16SYS_EXPORT unsigned int PIXIE16SYS_API SYS32_SetBit(unsigned short bit, unsigned int value);
PIXIE16SYS_EXPORT unsigned int PIXIE16SYS_API SYS32_ClrBit(unsigned short bit, unsigned int value);
PIXIE16SYS_EXPORT unsigned int PIXIE16SYS_API SYS32_TstBit(unsigned short bit, unsigned int value);

PIXIE16SYS_EXPORT int PIXIE16SYS_API get_ns_per_cycle(double* ns_per_cycle);
PIXIE16SYS_EXPORT void PIXIE16SYS_API wait_for_a_short_time(int cycles);

int Pixie_DSP_Memory_Burst_Read(unsigned int* dsp_data,  // DSP data for the I/O
                                unsigned int dsp_address,  // DSP data memory address
                                unsigned int nWords,  // Number of DSP data words for the I/O
                                unsigned short ModNum);  // The Pixie module for the I/O

int I2CM24C64_start(unsigned short ModNum);
int  I2CM24C64_stop(unsigned short ModNum);
int  I2CM24C64_byte_write(unsigned short ModNum, char ByteToSend);
int  I2CM24C64_byte_read(unsigned short ModNum, char* ByteToReceive);
char I2CM24C64_getACK(unsigned short ModNum);
char I2CM24C64_sendACK(unsigned short ModNum);


int PCF8574_start(unsigned short ModNum);
int PCF8574_stop(unsigned short ModNum);
int PCF8574_byte_write(unsigned short ModNum, char ByteToSend);
int PCF8574_byte_read(unsigned short ModNum, char *ByteToReceive);
char PCF8574_getACK(unsigned short ModNum);
char PCF8574_sendACK(unsigned short ModNum);

#ifdef __cplusplus
}
#endif  // End of notice for C++ compilers

#endif  // End of pixie16sys_common.h
