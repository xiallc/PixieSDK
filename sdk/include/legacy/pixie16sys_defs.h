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

/** @file pixie16sys_defs.h
 * @brief Declares internal constants for use by the API functions.
 * @note This file will be deprecated July 31, 2023
*/

#ifndef XIA_PIXIE16SYS_DEFS_H
#define XIA_PIXIE16SYS_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN64) || defined(_WIN32)
#define PIXIE16SYS_EXPORT __declspec(dllexport)
#define PIXIE16SYS_API _stdcall
#else
#define PIXIE16SYS_EXPORT
#define PIXIE16SYS_API
#endif

#ifndef PCI_CODE
#define PCI_CODE
#endif

#ifndef LITTLE_ENDIAN  // LITTLE_ENDIAN: least significant byte stored first
#define LITTLE_ENDIAN  // BIG_ENDIAN:    most significant byte stored first
#endif

// Preset maximum number of modules for defining data structures
#define SYS_MAX_NUM_MODULES 32

// Maximum number of PXI slots for one PXI chassis (currently 14-slot is the biggest chassis)
#define SYS_MAX_PXI_SLOTS 14

/*
 * ADSP-21160 registers defined from
 * https://www.analog.com/media/en/dsp-documentation/processor-manuals/ADSP-21160_hwr_rev4.1.pdf
 * last accessed on 2021-06-24
 */
#define SYSCON 0x00 /* System configuration register                   	*/
#define EPB0 0x04 /* External port DMA buffer 0                      	*/
#define DMAC10 0x1c /* EP DMA10 control register			      		*/
#define DMASTAT 0x37 /* DMA channel status register		      			*/

/*-------------------------------------
	Multiplication factor for the ns_per_cycle
  -------------------------------------*/
#define NSMULTIPLIER 1.0


/*-------------------------------------
	Option for printing out debugging messages 
  -------------------------------------*/
#define PRINT_DEBUG_MSG 1


/*------------------------------------- 
	DSP code tag types
  -------------------------------------*/

#define FINAL_INIT 0X0000
#define ZERO_DM16 0X0001
#define ZERO_DM32 0X0002
#define ZERO_DM40 0X0003
#define INIT_DM16 0X0004
#define INIT_DM32 0X0005
#define INIT_DM40 0X0006
#define ZERO_PM16 0X0007
#define ZERO_PM32 0X0008
#define ZERO_PM40 0X0009
#define ZERO_PM48 0X000A
#define INIT_PM16 0X000B
#define INIT_PM32 0X000C
#define INIT_PM40 0X000D
#define INIT_PM48 0X000E
#define ZERO_DM64 0X000F
#define INIT_DM64 0X0010
#define ZERO_PM64 0X0011
#define INIT_PM64 0X0012


/*------------------------------------- 
	I/O Mode
  -------------------------------------*/

#define SYS_MOD_READ 1  // Host read from modules
#define SYS_MOD_WRITE 0  // Host write to modules

#define MAX_ERRMSG_LENGTH 1024  // Maximum length of error message


/*-------------------------------------
	Host PCI CSR bits definitions
 --------------------------------------*/
#define RUNENA 0  // Enable run
#define DSPDOWNLOAD 1  // Enable DSP code download
#define PCIACTIVE 2  // Indicate PCI I/O is active
#define PULLUP_CTRL 3  // Control pull-up for the SYNC lines
#define DSPRESET 4  // Reset DSP
#define EXTFIFO_WML 6  // External FIFO watermark level indicator
#define RUNACTIVE 13  // Run active indicator
#define CLREXTMEM_ACTIVE 15  // Clearing external memory active indicator


/*-------------------------------------
	PCI address decoding and control registers
-------------------------------------*/

#define CFG_DATACS 0x00
#define CFG_CTRLCS 0x04
#define CFG_RDCS 0x08
#define I2CM24C64_ADDR 0x10
#define PCF8574_ADDR 0x10
#define CFG_DCMRST 0x14
#define CSR_ADDR 0x48
#define REQUEST_HBR 0x80
#define HBR_DONE 0x84
#define WRT_EXT_MEM 0xC0
#define EXT_MEM_TEST 0xC4
#define WRT_DSP_MMA 0xC8
#define SET_EXT_FIFO 0xCC

#define WRT_DSP_DMAC11 0xD0
#define WRT_DSP_II11 0xD4
#define WRT_CLR_EXTMEM 0xD8
#define WRT_DSP_C11 0xDC
#define WRT_DSP_IM11 0xE0
#define WRT_DSP_EC11 0xE4
#define RD_WRT_FIFO_WML 0xE8
#define SET_EXMEM_FIFO 0xEC
#define SET_INT_FIFO 0xF0
#define WRT_RESET_RFCNT 0xF4

/*------------------------------------- 
	Special DSP varaible address
-------------------------------------*/
#define POWERUPINITDONE_ADDRESS 0x4A03F  // Address for DSP variable PowerUpInitDone

/*------------------------------------- 
	PLX DMA transfer timeout setting
-------------------------------------*/
#define DMATRANSFER_TIMEOUT 5 * 1000  // PLX DMA transfer timeout limit, in ms

/*-------------------------------------
	I2C control bits as OR-able hex patterns
-------------------------------------*/
#define SDA 0x9  // bit 3 is used in PXlarge instead of bit 0
#define SCL 0x2
#define CTRL 0x4


#ifdef __cplusplus
}
#endif  // End of notice for C++ compilers

#endif  // End of pixie16sys_defs.h
