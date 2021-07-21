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

/** @file pixie16app.c
 * @brief Implements user-facing API functions.
 * @note This file will be deprecated July 31, 2023
 */

#include <errno.h>
#include <stdio.h>

#include <pixie16app_common.h>
#include <pixie16app_export.h>
#include <pixie16app_globals.h>
#include <pixie16sys_export.h>
#include <xia_common.h>

#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
#include <windows.h>
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
#include <stdlib.h>
#include <unistd.h>
#endif

#include <stdint.h>

static uint32_t
CRC32_update(uint32_t crc, const unsigned char* data, int len)
{
    const uint32_t table[256] =  {
        0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
        0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
        0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
        0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
        0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
        0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
        0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
        0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
        0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
        0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
        0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
        0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
        0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
        0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
        0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
        0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
        0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
        0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
        0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
        0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
        0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
        0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
        0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
        0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
        0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
        0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
        0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
        0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
        0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
        0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
        0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
        0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
        0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
        0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
        0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
        0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
        0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
        0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
        0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
        0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
        0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
        0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
        0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
        0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
        0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
        0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
        0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
        0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
        0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
        0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
        0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
        0x2d02ef8dL
    };
    crc = ~crc;
    while (len-- != 0) {
        crc = table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
    }
    return ~crc;
}

/*
 * The use of fread does not match the documentation. Paper over
 * here.
 */
int Pixie16_fread(void *buffer, size_t size, size_t count, FILE *stream)
{
    size_t r = fread(buffer, size, count, stream);
    if (r != count) {
        return -1;
    }
    return (int) r;
}

/**
 * @defgroup PUBLIC_API Public API
 * Functions for use in end-user software.
 */
/**
 * @ingroup PUBLIC_API
 * @brief Initializes the system by mapping PXI slots to modules
 *
 * Use this function to configure the Pixie-16 modules in the PXI chassis.
 *
 * NumModules is the total number of Pixie-16 modules installed in the system. PXISlotMap is the
 * pointer to an array that must have at least as many entries as there are Pixie-16 modules in the
 * chassis.
 *
 * PXISlotMap serves as a simple mapping of the logical module number and the physical slot number
 * that the modules reside in. The logical module number runs from 0. For instance, in a system
 * with 5 Pixie-16 modules, these 5 modules may occupy slots 3 through 7. The user must fill
 * PXISlotMap as follows: PXISlotMap = {3, 4, 5, 6, 7 ...} since module number 0 resides in slot
 * number 3, etc. To find out in which slot a module is located, any piece of subsequent code can
 * use the expression PXISlotMap[ModNum], where ModNum is the logic module number.
 * OfflineMode is used to indicate to the API whether the system is running in OFFLINE mode (1) or
 * ONLINE mode (0). OFFLINE mode is useful for situations where no Pixie-16 modules are present but
 * users can still test their calls to the API functions in their application software.
 * This function must be called as the first step in the boot process. It makes the modules known
 * to the system and “opens” each module for communication.
 *
 * @param[in] NumModules: The total number of Pixie16 modules in the system provided by the user.
 * @param[in] PXISlotMap: An array containing the PXI slot number for each module. The array index
 *     indicates which module number the slot number takes. The first slot number is Module 0.
 * @param[in] OfflineMode: Used to tell the API that there are no modules connected to the system.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid total number of Pixie16 modules
 * @retval -2 - Null pointer *PXISlotMap.
 * @retval -3 - Failed to initialize system
 * @retval -4 - Failed to read the module's information
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16InitSystem(unsigned short NumModules, unsigned short* PXISlotMap,
                                                       unsigned short OfflineMode) {
    int retval;  // return value
    unsigned short k;

    // Check the validity of total number of Pixie16 modules
    if ((NumModules < 1) || (NumModules > PRESET_MAX_MODULES))  // At least one module is necessary
    {
        Pixie_Print_Error(PIXIE_FUNC, "invalid total number (%d) of Pixie16 modules", NumModules);
        return (-1);
    } else {
        Number_Modules = NumModules;  // Number_Modules is a global variable
    }

    // Check if offline mode is requested
    if (OfflineMode == 0) {
        Offline = 0;  // Online mode
    } else if (OfflineMode >= 1) {
        Offline = 1;  // Offline mode
    }

    // Assign module variant if running in offline mode
    for (k = 0; k < Number_Modules; k++) { Module_Information[k].Module_OfflineVariant = OfflineMode; }

    // The physical address (slot number) of each Pixie16 module is in PXISlotMap
    if (PXISlotMap == NULL)  // Check if PXISlotMap is valid
    {
        Pixie_Print_Error(PIXIE_FUNC, "null pointer *PXISlotMap");
        return (-2);
    } else {
        // Scan PCI devices and assign address
        retval = Pixie_InitSystem(Number_Modules, PXISlotMap, Offline);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to initialize system, retval=%d", retval);
            return (-3);
        }

        // Read and store module information
        for (k = 0; k < Number_Modules; k++) {
            retval =
                    Pixie16ReadModuleInfo(k, &Module_Information[k].Module_Rev, &Module_Information[k].Module_SerNum,
                                          &Module_Information[k].Module_ADCBits, &Module_Information[k].Module_ADCMSPS);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "failed to read module information in module %d, retval=%d", k,
                                  retval);
                return (-4);
            }
        }

        return (0);
    }
}


/**
 * @ingroup PUBLIC_API
 * @brief Release resources used by PCI devices before exiting the application.
 * @param[in] ModNum: The module number that we'll be closing.
 * @returns A status code indicating the result of the operation
 * @retval:  0 - Success
 * @retval: -1 - Invalid Pixie module number
 * @retval: -2 - Failed to close Pixie module
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ExitSystem(unsigned short ModNum) {

    int retval;  // return values

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    retval = Pixie_ClosePCIDevices(ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to close PCI device, retval=%d", retval);
        return (-2);
    }

    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Read information stored on each module, including its revision, serial number, and ADC.
 *
 * Use this function to read information stored on each module, including its revision, serial
 * number, ADC bits and sampling rate. This should be done after initializing the PCI communication.
 * Information from the module can be used to select the appropriate firmware, DSP, and
 * configuration parameters files before booting the module.
 *
 * @param[in] ModNum: The module number (counts from 0) that we'll read information
 * @param[out] ModRev: The revision read from the on-board EEPROM
 * @param[out] ModSerNum: The serial number read from the on-board EEPROM
 * @param[out] ModADCBits: The ADC bit resolution read from the on-board EEPROM
 * @param[out] ModADCMSPS: The ADC sampling frequency read from the on-board EEPROM
 * @returns A status code indicating the result of the operation
 * @retval:  0 - Success
 * @retval: -1 - Invalid Pixie module number
 * @retval: -2 - Failed to read the serial number from I2C serial EEPROM
 * @retval: -3 - Failed to read the ADC information from I2C serial EEPROM
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadModuleInfo(unsigned short ModNum, unsigned short* ModRev,
                                                           unsigned int* ModSerNum, unsigned short* ModADCBits,
                                                           unsigned short* ModADCMSPS) {

    int retval;  // return values
    unsigned char sbuffer[256] = {0};  // Temporary buffer
    unsigned short startaddr;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Check if running in OFFLINE mode
    if (Offline == 1)  // Returns a set of default module information for offline analysis
    {
        *ModRev = 0xF;
        *ModSerNum = 1000;  // dummy serial number

        if (Module_Information[ModNum].Module_OfflineVariant == 1) {
            *ModRev = 0xD;
            *ModSerNum = 275;  // dummy serial number
            *ModADCBits = 12;
            *ModADCMSPS = 100;
            Module_Information[ModNum].Module_ADCBits = 12;
            Module_Information[ModNum].Module_ADCMSPS = 100;
        } else if (Module_Information[ModNum].Module_OfflineVariant == 2) {
            *ModADCBits = 14;
            *ModADCMSPS = 100;
            Module_Information[ModNum].Module_ADCBits = 14;
            Module_Information[ModNum].Module_ADCMSPS = 100;
        } else if (Module_Information[ModNum].Module_OfflineVariant == 3) {
            *ModADCBits = 12;
            *ModADCMSPS = 250;
            Module_Information[ModNum].Module_ADCBits = 12;
            Module_Information[ModNum].Module_ADCMSPS = 250;
        } else if (Module_Information[ModNum].Module_OfflineVariant == 4) {
            *ModADCBits = 14;
            *ModADCMSPS = 250;
            Module_Information[ModNum].Module_ADCBits = 14;
            Module_Information[ModNum].Module_ADCMSPS = 250;
        } else if (Module_Information[ModNum].Module_OfflineVariant == 5) {
            *ModADCBits = 12;
            *ModADCMSPS = 500;
            Module_Information[ModNum].Module_ADCBits = 12;
            Module_Information[ModNum].Module_ADCMSPS = 500;
        } else if (Module_Information[ModNum].Module_OfflineVariant == 6) {
            *ModADCBits = 14;
            *ModADCMSPS = 500;
            Module_Information[ModNum].Module_ADCBits = 14;
            Module_Information[ModNum].Module_ADCMSPS = 500;
        } else if (Module_Information[ModNum].Module_OfflineVariant == 7) {
            *ModADCBits = 16;
            *ModADCMSPS = 250;
            Module_Information[ModNum].Module_ADCBits = 16;
            Module_Information[ModNum].Module_ADCMSPS = 250;
        }

        return (0);
    }

    // EEPROM Format
    //
    // Read the first 128 bytes from the beginning of the I2C serial EEPROM.
    //
    // Version 1:
    //  First three words of EEPROM stores serial number and revision number
    //
    // Version 2:
    //  If the CRC in the header validates it is a version 2 or higher format.
    //
    //  Header: [CRC32 (little endian, 4 bytes)] [CTRL (1 byte)] [RESERVED (1 byte)]
    //  Data: [[ID (1 byte)] [DATA (variable length)]
    //
    //  Supported Ids:
    //   SERIAL_NUM = 11, little endian numnber, size = 4
    //   REVISION = 12, little endian number, size = 2
    //   END = 255
    //
    startaddr = 0;
    retval = I2CM24C64_Sequential_Read(ModNum, startaddr, 128, (char*) sbuffer);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Could not read EEPROM contents for Module=%d; retval=%d",
                          ModNum, retval);
        return (-2);
    }
    // CRC covers all data except the CRC field in the header
    int crc = CRC32_update(0, &sbuffer[4], 128 - 4);
    int eeprom_crc = (int)sbuffer[0] | ((int)sbuffer[1] << 8) | ((int)sbuffer[2] << 16) | ((int)sbuffer[3] << 24);
    if (eeprom_crc == crc) {
        // Version 2 format
        int offset = 4 + 1 + 1;
        *ModSerNum = 0;
        *ModRev = 0;
        while (offset < 128) {
            switch (sbuffer[offset]) {
            case 11:
                ++offset;
                *ModSerNum =
                    (int)sbuffer[offset + 0] | ((int)sbuffer[offset + 1] << 8) |
                    ((int)sbuffer[2] << 16) | ((int)sbuffer[3] << 24);
                offset += 4;
                break;
            case 12:
                ++offset;
                *ModRev = (int)sbuffer[offset + 0];
                offset += 1;
                break;
            case 13:
                ++offset;
                offset += 1;
                break;
            case 255:
                offset = 128;
                break;
            default:
                Pixie_Print_Error(PIXIE_FUNC, "Invalid EEPROM tag for Module=%d; tag=%d offset=%d",
                                  ModNum, sbuffer[offset], offset);
                return (-2);
            }
        }
        if (*ModSerNum == 0) {
            Pixie_Print_Error(PIXIE_FUNC, "Could not find serial numnber in EEPROM contents for Module=%d; retval=%d",
                              ModNum, retval);
            return (-2);
        }
        if (*ModRev == 0) {
            Pixie_Print_Error(PIXIE_FUNC, "Could not find revision in EEPROM contents for Module=%d; retval=%d",
                              ModNum, retval);
            return (-2);
        }
        /*
         * Hack in the DB.
         */
        *ModADCBits = 14;
        *ModADCMSPS = 250;

        if (*ModRev > 15) {
            Pixie_Print_Error(PIXIE_FUNC, "Module revision not supported by the API for Module=%d: rev=%d",
                              ModNum, *ModRev);
            return (-20);
        }
    } else {
        // Version 1 format
        // Starting with serial number 256, serial number is stored in the
        // first two bytes of EEPROM, follwed by revision number, which is at
        // least 11 (i.e. Rev-B)
        if (sbuffer[2] >= 11) {
            *ModSerNum = (unsigned short) (unsigned char) sbuffer[0] + 256 * (unsigned short) (unsigned char) sbuffer[1];
            *ModRev = (unsigned short) (unsigned char) sbuffer[2];

            // For serial number below 275, no ADC information is stored in the I2C serial EEPROM.
            // However, all such modules have 12-bit, 100 MSPS ADCs.
            if (*ModSerNum < 275) {
                *ModADCBits = 12;
                *ModADCMSPS = 100;
            }
            // For serial number between 1000 and 1034, no ADC information is stored in the I2C serial EEPROM.
            // However, all such modules have 12-bit, 250 MSPS ADCs.
            else if ((*ModSerNum >= 1000) && (*ModSerNum <= 1034)) {
                *ModADCBits = 12;
                *ModADCMSPS = 250;
            }
            // For serial number 1035 and above, ADC information is stored in the I2C serial EEPROM.
            else if (*ModSerNum >= 1035) {
                // ADC information is stored in the I2C serial EEPROM, starting at address 99
                *ModADCBits = (unsigned short) (unsigned char) sbuffer[99];
                *ModADCMSPS =
                    (unsigned short) (unsigned char) sbuffer[99 + 1] + 256 *
                    (unsigned short) (unsigned char) sbuffer[99 + 2];
            }
        } else {
            *ModSerNum = (unsigned short) (unsigned char) sbuffer[0];
            *ModRev = (unsigned short) (unsigned char) sbuffer[1];

            // Prior to serial number 256, no ADC information is stored in the I2C serial EEPROM.
            // However, all such modules have 12-bit, 100 MSPS ADCs.
            *ModADCBits = 12;
            *ModADCMSPS = 100;
        }
    }
    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Boot one or more modules in the system.
 *
 * Use this function to boot Pixie-16 modules so that they can be set up for data taking. The
 * function downloads to the Pixie-16 modules the communication (or system) FPGA configurations,
 * signal processing FPGA configurations, trigger FPGA configurations (Revision A modules only),
 * executable code for the digital signal processor (DSP), and DSP parameters.
 *
 * The FPGA configurations consist of a fixed number of words depending on the hardware mounted on
 * the modules; the DSP codes have a length which depends on the actual compiled code; and the set
 * of DSP parameters always consists of 1280 32-bit words for each module. The host software has to
 * make the names of those boot data files on the hard disk available to the boot function.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will be booted. But
 * if ModNum is equal to the total number of modules in the system, e.g. there are 5 modules in
 * the chassis and ModNum = 5, then all modules in the system will be booted.
 *
 * #### Boot Pattern Bits
 * The boot pattern is a bit mask (shown below) indicating which on-board chip will be booted.
 * Under normal circumstances, all on-board chips should be booted, i.e. the boot pattern would
 * be 0x7F. For Rev-B, C, D, F modules, bit 1, i.e., “Boot trigger FPGA”, will be ignored even if
 * that bit is set to 1.
 * | Bit | Description | Applicable Hardware |
 * |---|---|---|
 * | 0 | Boot communication FPGA  | All Pixie-16 Revisions  |
 * | 1 | Boot trigger FPGA  | Revision A - Ignored  |
 * | 2 | Boot signal processing FPGA  | All Pixie-16 Revisions  |
 * | 3 | Boot digital signal processor (DSP)  | All Pixie-16 Revisions  |
 * | 4 | Download DSP I/O parameters  | All Pixie-16 Revisions  |
 * | 5 | Program on-board FPGAs  | All Pixie-16 Revisions  |
 * | 6 | Set on-board DACs  | All Pixie-16 Revisions  |
 *
 * @param[in] ComFPGAConfigFile: name of communications FPGA configuration file
 * @param[in] SPFPGAConfigFile: name of signal processing FPGA configuration file
 * @param[in] TrigFPGAConfigFile: name of trigger FPGA configuration file (!!! IGNORED)
 * @param[in] DSPCodeFile: name of executable code file for digital signal processor (DSP)
 * @param[in] DSPParFile: name of DSP parameter file
 * @param[in] DSPVarFile: name of DSP variable names file
 * @param[in] ModNum: The module number. If set to Number_Modules we'll boot all modules.
 * @param[in] BootPattern: boot pattern bit mask
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid module number
 * @retval -2 - Size of ComFPGAConfigFile is invalid
 * @retval -3 - Could not read the ComFPGAConfig file
 * @retval -4 - Failed to allocate memory to store ComFPGAConfig
 * @retval -5 - Failed to open ComFPGAConfigFile
 * @retval -10 - Size of SPFPGAConfigFile is invalid
 * @retval -11 - Failed to read SPFPGAConfigFile
 * @retval -12 - Failed to allocate memory to store SPFPGAConfigFile
 * @retval -13 - Failed to open SPFPGAConfigFile
 * @retval -14 - Failed to read DSPCodeFile
 * @retval -15 - Failed to allocate memory to store DSP executable code
 * @retval -16 - Failed to open DSPCodeFile
 * @retval -17 - Size of DSPParFile is invalid
 * @retval -18 - Failed to open DSPParFile
 * @retval -19 - Can't initialize DSP variable indices
 * @retval -20 - Can't copy DSP variable indices
 * @retval -21 - Failed to program Fippi in a module
 * @retval -22 - Failed to set DACs in a module
 * @retval -23 - Failed to start RESET_ADC run in a module
 * @retval -24 - RESET_ADC run timed out in a module
 * @retval -25 - Failed to boot the Communication FPGA
 * @retval -26 - Failed to boot signal processing FPGA
 * @retval -27 - Failed to boot DSP
 * @retval -28 - Failed to read DSPParFile
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BootModule(const char* ComFPGAConfigFile, const char* SPFPGAConfigFile,
                                                       const char* TrigFPGAConfigFile, const char* DSPCodeFile,
                                                       const char* DSPParFile, const char* DSPVarFile,
                                                       unsigned short ModNum, unsigned short BootPattern) {
    int retval;  // return values
    FILE* configfil;
    unsigned int* configuration;
    unsigned short k, dspcode;
    unsigned int TotalWords, CSR;
    unsigned int CPLD_CSR;
    unsigned int FPGA_ConfigSize;
    unsigned short sourcemodule;

    (void) TrigFPGAConfigFile;

    // Check if ModNum is valid
    if (ModNum > Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    //**************************************************
    // Boot communication FPGA
    //**************************************************

    if ((APP16_TstBit(BOOTPATTERN_COMFPGA_BIT, BootPattern)) && (Offline == 0)) {
        // Open communication FPGA configuration file
        if ((configfil = fopen(ComFPGAConfigFile, "rb")) != NULL)  // Make sure ComFPGAConfigFile is opened successfully
        {
            // Download to one or all modules
            if (ModNum == Number_Modules)  // Download to all modules
            {
                sourcemodule = 0;  // All modules are assumed to have the same revision
            } else  // Download to a single module only
            {
                sourcemodule = ModNum;  // Use the specified module
            }

            // Set FPGA configuration size based on module revision
            if ((Module_Information[sourcemodule].Module_Rev == 0xB) ||
                (Module_Information[sourcemodule].Module_Rev == 0xC) ||
                (Module_Information[sourcemodule].Module_Rev == 0xD))
                FPGA_ConfigSize = N_COM_FPGA_CONF_REVBCD;
            else if (Module_Information[sourcemodule].Module_Rev == 0xF)
                FPGA_ConfigSize = N_COM_FPGA_CONF_REVF;

            // Check if file size is consistent with predefined length FPGA_ConfigSize
            retval = fseek(configfil, 0, SEEK_END);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "failed to seek config, error=%d",
                                  errno);
                (void) fclose(configfil);
                return (-3);
            }

            TotalWords = (ftell(configfil) + 1) / 4;

            if (TotalWords != FPGA_ConfigSize) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "size of ComFPGAConfigFile is invalid. Check ComFPGAConfigFile name");
                (void) fclose(configfil);
                return (-2);
            }

            // Allocate memory
            if ((configuration = (unsigned int*) malloc(sizeof(unsigned int) * FPGA_ConfigSize)) != NULL) {
                // Point configfil to the beginning of file
                retval = fseek(configfil, 0, SEEK_SET);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "failed to seek config, error=%d",
                                      errno);
                  free(configuration);
                  (void) fclose(configfil);
                  return (-3);
                }

                // Read communication FPGA configuration
                retval = Pixie16_fread(configuration, sizeof(unsigned int), FPGA_ConfigSize, configfil);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "failed to read config, error=%d",
                                      errno);
                  free(configuration);
                  (void) fclose(configfil);

                  return (-3);
                }

                if (ModNum == Number_Modules)  // Download to all modules
                {
                    for (k = 0; k < Number_Modules; k++) {
                        printf("Start to boot Communication FPGA in module %d\n", k);

                        retval = Pixie_Boot_CompFPGA(k, configuration, FPGA_ConfigSize);
                        if (retval < 0) {
                            Pixie_Print_Error(PIXIE_FUNC,
                                              "failed to boot Communication FPGA in module %d, retval=%d",
                                              k, retval);
                            free(configuration);
                            (void) fclose(configfil);
                            return (-25);
                        }
                    }
                } else  // Download to a single module only
                {
                    printf("Start to boot Communication FPGA in module %d\n", ModNum);

                    retval = Pixie_Boot_CompFPGA(ModNum, configuration, FPGA_ConfigSize);
                    if (retval < 0) {
                        Pixie_Print_Error(PIXIE_FUNC,
                                          "failed to boot Communication FPGA in module %d, retval=%d",
                                          ModNum, retval);
                        free(configuration);
                        (void) fclose(configfil);
                        return (-25);
                    }
                }

                // free allocated memory
                free(configuration);
            } else {
                Pixie_Print_Error(PIXIE_FUNC, "failed to allocate memory to store ComFPGAConfig");
                return (-4);
            }

            // close opened files
            (void) fclose(configfil);
        } else {
            Pixie_Print_Error(PIXIE_FUNC, "failed to open ComFPGAConfigFile %s", ComFPGAConfigFile);
            return (-5);
        }
    }

    //**************************************************
    // Boot signal processing FPGA
    //**************************************************

    if ((APP16_TstBit(BOOTPATTERN_SPFPGA_BIT, BootPattern)) && (Offline == 0)) {
        // Open signal processing FPGA configuration file
        if ((configfil = fopen(SPFPGAConfigFile, "rb")) != NULL)  // Make sure SPFPGAConfigFile is opened successfully
        {
            // Download to one or all modules
            if (ModNum == Number_Modules)  // Download to all modules
            {
                sourcemodule = 0;  // All modules are assumed to have the same revision
            } else  // Download to a single module only
            {
                sourcemodule = ModNum;  // Use the specified module
            }

            // Set FPGA configuration size based on module revision
            if ((Module_Information[sourcemodule].Module_Rev == 0xB) ||
                (Module_Information[sourcemodule].Module_Rev == 0xC) ||
                (Module_Information[sourcemodule].Module_Rev == 0xD))
                FPGA_ConfigSize = N_SP_FPGA_CONF_REVBCD;
            else if (Module_Information[sourcemodule].Module_Rev == 0xF)
                FPGA_ConfigSize = N_SP_FPGA_CONF_REVF;

            // Check if file size is consistent with predefined length FPGA_ConfigSize
            fseek(configfil, 0, SEEK_END);
            TotalWords = (ftell(configfil) + 1) / 4;

            if (TotalWords != FPGA_ConfigSize) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "size of SPFPGAConfigFile is invalid. Check SPFPGAConfigFile name");
                return (-10);
            }

            // Allocate memory
            if ((configuration = (unsigned int*) malloc(sizeof(unsigned int) * FPGA_ConfigSize)) != NULL) {
                // Point configfil to the beginning of file
                retval = fseek(configfil, 0, SEEK_SET);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "failed to seek config, error=%d",
                                      errno);
                    free(configuration);
                    (void) fclose(configfil);
                    return (-11);
                }

                // Read trigger FPGA configuration
                retval = Pixie16_fread(configuration, sizeof(unsigned int), FPGA_ConfigSize, configfil);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "failed to read config, error=%d",
                                      errno);
                    free(configuration);
                    (void) fclose(configfil);
                    return (-11);
                }

                // Download to one or all modules
                if (ModNum == Number_Modules)  // Download to all modules
                {
                    for (k = 0; k < Number_Modules; k++) {
                        printf("Start to boot signal processing FPGA in module %d\n", k);

                        retval = Pixie_Boot_FIPPI(k, configuration, FPGA_ConfigSize);
                        if (retval < 0) {
                            Pixie_Print_Error(PIXIE_FUNC,
                                              "failed to boot signal processing FPGA in module %d, retval=%d",
                                              k, retval);
                            free(configuration);
                            (void) fclose(configfil);
                            return (-26);
                        }
                    }
                } else  // Download to a single module only
                {
                    printf("Start to boot signal processing FPGA in module %d\n", ModNum);

                    retval = Pixie_Boot_FIPPI(ModNum, configuration, FPGA_ConfigSize);
                    if (retval < 0) {
                        Pixie_Print_Error(PIXIE_FUNC,
                                          "failed to boot signal processing FPGA in module %d, retval=%d",
                                          ModNum, retval);
                        free(configuration);
                        (void) fclose(configfil);
                        return (-26);
                    }
                }

                // free allocated memory
                free(configuration);
            } else {
                Pixie_Print_Error(PIXIE_FUNC, "failed to allocate memory to store SPFPGAConfig");
                return (-12);
            }

            // close opened files
            fclose(configfil);
        } else {
            Pixie_Print_Error(PIXIE_FUNC, "failed to open SPFPGAConfigFile %s", SPFPGAConfigFile);
            return (-13);
        }
    }

    //**************************************************
    // Boot Digital Signal Processor (DSP)
    //**************************************************
    if ((APP16_TstBit(BOOTPATTERN_DSPCODE_BIT, BootPattern)) && (Offline == 0)) {
        // Open DSP executable code file
        if ((configfil = fopen(DSPCodeFile, "rb")) != NULL)  // Make sure DSPCodeFile is opened successfully
        {
            // Check the file size
            fseek(configfil, 0, SEEK_END);
            TotalWords = (ftell(configfil) + 1) / 2;

            // Allocate memory
            if ((configuration = (unsigned int*) malloc(sizeof(unsigned int) * TotalWords)) != NULL) {
                // Point configfil to the beginning of file
                retval = fseek(configfil, 0, SEEK_SET);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC, "failed config seek, error=%d",
                                      errno);

                    // free allocated memory and close opened files
                    free(configuration);
                    (void) fclose(configfil);

                    return (-14);
                }

                // Read DSP executable code
                for (k = 0; k < TotalWords; k++) {
                  retval = Pixie16_fread(&dspcode, sizeof(unsigned short), 1, configfil);
                    if (retval < 0) {
                        Pixie_Print_Error(PIXIE_FUNC, "failed config read for module %d, error=%d",
                                          k, errno);

                        // free allocated memory and close opened files
                        free(configuration);
                        (void) fclose(configfil);

                        return (-14);
                    }

                    // Rearrange byte order
                    configuration[k] = (unsigned int) ((dspcode >> 8) + (dspcode & 0xFF) * 256);
                }

                // Download to one or all modules
                if (ModNum == Number_Modules)  // Download to all modules
                {
                    for (k = 0; k < Number_Modules; k++) {
                        printf("Start to boot DSP in module %d\n", k);

                        retval = Pixie_Boot_DSP(k, configuration, TotalWords);
                        if (retval < 0) {
                            Pixie_Print_Error(PIXIE_FUNC, "failed to boot DSP in module %d, retval=%d",
                                              k, retval);

                            // free allocated memory and close opened files
                            free(configuration);
                            (void) fclose(configfil);

                            return (-27);
                        }
                    }
                } else  // Download to a single module only
                {
                    printf("Start to boot DSP in module %d\n", ModNum);

                    retval = Pixie_Boot_DSP(ModNum, configuration, TotalWords);
                    if (retval < 0) {
                        Pixie_Print_Error(PIXIE_FUNC, "failed to boot DSP in module %d, retval=%d",
                                          ModNum, retval);

                        // free allocated memory and close opened files
                        free(configuration);
                        (void) fclose(configfil);

                        return (-27);
                    }
                }

                // free allocated memory
                free(configuration);
            } else {
                Pixie_Print_Error(PIXIE_FUNC, "failed to allocate memory to store DSP executable code");
                return (-15);
            }

            // close opened files
            fclose(configfil);
        } else {
            Pixie_Print_Error(PIXIE_FUNC, "failed to open DSPCodeFile %s", DSPCodeFile);
            return (-16);
        }
    }

    //**************************************************
    // Download DSP I/O parameters
    //**************************************************

    if (APP16_TstBit(BOOTPATTERN_DSPPAR_BIT, BootPattern)) {
        // Open DSP parameters file
        if ((configfil = fopen(DSPParFile, "rb")) != NULL)  // Make sure DSPParFile is opened successfully
        {
            // Check if file size is consistent with predefined length (N_DSP_PAR * PRESET_MAX_MODULES)
            retval = fseek(configfil, 0, SEEK_END);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed config seek, error=%d",
                                  errno);
                (void) fclose(configfil);
                return (-28);
            }

            TotalWords = (ftell(configfil) + 1) / 4;
            if (TotalWords != (N_DSP_PAR * PRESET_MAX_MODULES)) {
                Pixie_Print_Error(PIXIE_FUNC, "size of DSPParFile is invalid. Check DSPParFile name");
                fclose(configfil);
                return (-17);
            }

            // Point configfil to the beginning of file
            retval = fseek(configfil, 0, SEEK_SET);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed config seek, error=%d",
                                  errno);
                (void) fclose(configfil);
                return (-28);
            }

            // Read DSP parameters
            for (k = 0; k < PRESET_MAX_MODULES; k++) {
              retval = Pixie16_fread(Pixie_Devices[k].DSP_Parameter_Values, sizeof(unsigned int), N_DSP_PAR, configfil);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC, "failed config read for module %d, error=%d",
                                      k, errno);
                    (void) fclose(configfil);
                    return (-28);
                }

                // Force correct module number
                Pixie_Devices[k].DSP_Parameter_Values[0] = k;
            }

            // Download to one or all modules
            if (ModNum == Number_Modules)  // Download to all modules
            {
                for (k = 0; k < Number_Modules; k++) {
                    Pixie_DSP_Memory_IO(Pixie_Devices[k].DSP_Parameter_Values, DATA_MEMORY_ADDRESS, DSP_IO_BORDER,
                                        MOD_WRITE, k);
                }
            } else  // Download to just one module
            {
                Pixie_DSP_Memory_IO(Pixie_Devices[ModNum].DSP_Parameter_Values, DATA_MEMORY_ADDRESS, DSP_IO_BORDER,
                                    MOD_WRITE, ModNum);
            }

            // close opened files
            (void) fclose(configfil);
        } else {
            Pixie_Print_Error(PIXIE_FUNC, "failed to open DSPParFile %s", DSPParFile);
            return (-18);
        }
    }

    //**************************************************
    // Read DSP variable names from file
    //**************************************************

    if (ModNum == Number_Modules)  // Copy to all modules
    {
        // First read DSP variable indices for Module #0
        retval = Pixie_Init_DSPVarAddress(DSPVarFile, 0);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "can't initialize DSP variable indices for module 0");
            return (-19);
        }

        // Copy DSP variable indices from Module #0 to other modules
        for (k = 1; k < Number_Modules; k++) {
            retval = Pixie_Copy_DSPVarAddress(0, k);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "can't copy DSP variable indices from module 0 to module %d", k);
                return (-20);
            }
        }
    } else {
        // Only read DSP variable indices for Module #0
        if (ModNum == 0) {
            retval = Pixie_Init_DSPVarAddress(DSPVarFile, ModNum);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "can't initialize DSP variable indices for module %d",
                                  ModNum);
                return (-19);
            }
        } else  // Copy DSP variable indices from Module #0 to other modules
        {
            retval = Pixie_Copy_DSPVarAddress(0, ModNum);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "can't copy DSP variable indices from module 0 to module %d",
                                  ModNum);
                return (-20);
            }
        }
    }

    //**************************************************
    // Program Fippi
    //**************************************************
    if ((APP16_TstBit(BOOTPATTERN_PROGFIPPI_BIT, BootPattern)) && (Offline == 0)) {
        // Program Fippi in one or all modules
        if (ModNum == Number_Modules)  // Program Fippi in all modules
        {
            for (k = 0; k < Number_Modules; k++) {
                retval = Pixie16ProgramFippi(k);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC, "failed to program Fippi in module %d, retval=%d", k,
                                      retval);
                    return (-21);
                }
            }
        } else  // Program Fippi in just one module
        {
            retval = Pixie16ProgramFippi(ModNum);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed to program Fippi in module %d, retval=%d", ModNum,
                                  retval);
                return (-21);
            }
        }
    }

    //**************************************************
    // Set DACs
    //**************************************************
    if ((APP16_TstBit(BOOTPATTERN_SETDACS_BIT, BootPattern)) && (Offline == 0)) {
        // Set DACs in one or all modules
        if (ModNum == Number_Modules)  // Set DACs in all modules
        {
            for (k = 0; k < Number_Modules; k++) {
                retval = Pixie16SetDACs(k);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC, "failed to set DACs in module %d, retval=%d", k,
                                      retval);
                    return (-22);
                }
            }
        } else  // Set DACs in just one module
        {
            retval = Pixie16SetDACs(ModNum);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed to set DACs in module %d, retval=%d", ModNum,
                                  retval);
                return (-22);
            }
        }
    }

    //**************************************************
    // Set pullups for the trigger lines on the backplane
    //
    // Also enable connections of PXI nearest neighbor
    // lines (J2) onto the backplane if the module is
    // a Rev-B or C module
    //**************************************************

    if (ModNum == Number_Modules)  // Set modules all at once
    {
        for (k = 0; k < Number_Modules; k++) {
            CPLD_CSR = 0xAAA;

            // Pull-up
            if (APP32_TstBit(MODCSRB_CPLDPULLUP,
                             Pixie_Devices[k].DSP_Parameter_Values[ModCSRB_Address[k] - DATA_MEMORY_ADDRESS])) {
                CPLD_CSR = APP32_SetBit(CPLDCSR_PULLUP, CPLD_CSR);
            } else {
                CPLD_CSR = APP32_ClrBit(CPLDCSR_PULLUP, CPLD_CSR);
            }

            // Enable connections of PXI nearest neighbor lines (J2) onto the
            // backplane if the module is a Rev-B or C module
            if ((Module_Information[k].Module_Rev == 0xB) || (Module_Information[k].Module_Rev == 0xC)) {
                CPLD_CSR = APP32_SetBit(CPLDCSR_BPCONNECT, CPLD_CSR);
            }

            Pixie_Register_IO(k, CFG_CTRLCS, MOD_WRITE, &CPLD_CSR);
        }
    } else {
        // Set module by module

        CPLD_CSR = 0xAAA;

        // Pull-up
        if (APP32_TstBit(MODCSRB_CPLDPULLUP,
                         Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS])) {
            CPLD_CSR = APP32_SetBit(CPLDCSR_PULLUP, CPLD_CSR);
        } else {
            CPLD_CSR = APP32_ClrBit(CPLDCSR_PULLUP, CPLD_CSR);
        }

        // Enable connections of PXI nearest neighbor lines (J2) onto the
        // backplane if the module is a Rev-B or C module
        if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC)) {
            CPLD_CSR = APP32_SetBit(CPLDCSR_BPCONNECT, CPLD_CSR);
        }

        Pixie_Register_IO(ModNum, CFG_CTRLCS, MOD_WRITE, &CPLD_CSR);
    }

    //**************************************************
    // Set pullups for the SYNCH lines on the backplane
    //**************************************************

    if (ModNum == Number_Modules)  // Set modules all at once
    {
        for (k = 0; k < Number_Modules; k++) {
            Pixie_ReadCSR(k, &CSR);
            if (APP32_TstBit(MODCSRB_CHASSISMASTER,
                             Pixie_Devices[k].DSP_Parameter_Values[ModCSRB_Address[k] - DATA_MEMORY_ADDRESS])) {
                CSR = APP32_SetBit(PULLUP_CTRL, CSR);
            } else {
                CSR = APP32_ClrBit(PULLUP_CTRL, CSR);
            }
            Pixie_WrtCSR(k, CSR);
        }
    } else {
        // Set module by module
        Pixie_ReadCSR(ModNum, &CSR);
        if (APP32_TstBit(MODCSRB_CHASSISMASTER,
                         Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS])) {
            CSR = APP32_SetBit(PULLUP_CTRL, CSR);
        } else {
            CSR = APP32_ClrBit(PULLUP_CTRL, CSR);
        }
        Pixie_WrtCSR(ModNum, CSR);
    }

    //**************************************************
    // Reset ADCs for RevF modules
    //**************************************************

    if (ModNum == Number_Modules)  // Set modules all at once
    {
        for (k = 0; k < Number_Modules; k++) {
            if (Module_Information[k].Module_Rev == 0xF) {
                retval = Pixie_Control_Task_Run(k, RESET_ADC, 10000);
                if (retval == -1) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "failed to start RESET_ADC run in module %d; retval=%d", k,
                                      retval);
                    return (-23);
                } else if (retval == -2) {
                    Pixie_Print_Error(PIXIE_FUNC, "RESET_ADC run timed out in module %d; retval=%d", k,
                                      retval);
                    return (-24);  // Time Out
                }
            }
        }
    } else {
        // Set module by module
        if (Module_Information[ModNum].Module_Rev == 0xF) {
            retval = Pixie_Control_Task_Run(ModNum, RESET_ADC, 10000);
            if (retval == -1) {
                Pixie_Print_Error(PIXIE_FUNC, "failed to start RESET_ADC run in module %d; retval=%d",
                                  ModNum, retval);
                return (-23);
            } else if (retval == -2) {
                Pixie_Print_Error(PIXIE_FUNC, "RESET_ADC run timed out in module %d; retval=%d", ModNum,
                                  retval);
                return (-24);  // Time Out
            }
        }
    }

    return (0);  // success
}

/**
 * @ingroup PUBLIC_API
 * @brief Acquire ADC traces from a Pixie module.
 *
 * Use this function to acquire ADC traces from Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its ADC traces acquired. But if ModNum is
 * equal to the total number of modules in the system, then all modules in the system will have
 * their ADC traces acquired.
 *
 * After the successful return of this function, the DSP’s internal memory will be filled with ADC
 * trace data. A user’s application software should then call the function
 * Pixie16ReadSglChanADCTrace to read the ADC trace data out to the host computer,
 * channel by channel.
 *
 * @see Pixie16ReadSglChanADCTrace
 *
 * @param[in] ModNum: The module number that we'll be working with
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start run
 * @retval -3 - Acquiring ADC traces timed out
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireADCTrace(unsigned short ModNum) {
    int retval;  // return values
    unsigned int count;

    // Check if running in OFFLINE mode
    if (Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    retval = Pixie_Start_Run(NEW_RUN, 0, GET_TRACES, ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to start run; retval=%d", retval);
        return (-2);
    }

    count = 0;
    do {
#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
        Sleep(1);
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
        usleep(1000);
#endif

        retval = Pixie_Check_Run_Status(ModNum);
        count++;
    } while ((retval != 0) && (count < 10000));  // The maximal waiting time is 10 s

    if (count >= 10000) {
        Pixie_Print_Error(PIXIE_FUNC, "acquiring ADC traces in module #%d timed out", ModNum);
        return (-3);  // Time Out
    }

    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Acquire ADC traces from a Pixie module.
 *
 * Use this function to read ADC trace data from a Pixie-16 module. Before calling this function,
 * another function Pixie16AcquireADCTrace should be called to fill the DSP internal memory first.
 * Also, the host code should allocate appropriate amount of memory to store the trace data. The
 * ADC trace data length for each channel is 8192. Since the trace data are 16-bit unsigned
 * integers (for hardware variants with less than 16-bit ADCs only the lower 12-bit or 14-bit
 * contain real data), two consecutive 16-bit words are packed into one 32-bit word in the DSP
 * internal memory. So for each channel, 4096 32-bit words are read out first from the DSP, and
 * then each 32-bit word is unpacked to form two 16-bit words.
 *
 * Specify the module using ModNum and the channel on the module using ChanNum. Note that both the
 * modules and channels are counted starting at 0.
 *
 * @see Pixie16AcquireADCTrace
 *
 * @param[out] Trace_Buffer: Pointer to the data buffer that will hold the data.
 * @param[in] Trace_Length: Length of the traces for the module.
 * @param[in] ModNum: The module that we want to read
 * @param[in] ChanNum: The channel that we want to read
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Invalid trace length
 * @retval -4 - Failed to allocate memory to store ADC traces
 * @retval -5 - Failed to read ADC traces
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanADCTrace(unsigned short* Trace_Buffer,
                                                                unsigned int Trace_Length,
                                                                unsigned short ModNum,
                                                                unsigned short ChanNum)
{
    unsigned int count;
    unsigned int* ADCData;
    int retval;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Check if ChanNum is valid
    if (ChanNum >= NUMBER_OF_CHANNELS) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie channel number %d", ChanNum);
        return (-2);
    }

    // Check if Trace_Length is valid
    if (Trace_Length > MAX_ADC_TRACE_LEN) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid ADC trace length %u for channel %d",
                          Trace_Length, ChanNum);
        return (-3);
    }

    // Allocate memory to store ADC trace data temporarily
    if ((ADCData = (unsigned int*) malloc(sizeof(unsigned int) * Trace_Length)) != NULL) {
        // Read out ADC trace for the requested channel
        retval = Pixie_DSP_Memory_IO(ADCData, (IO_BUFFER_ADDRESS + MAX_ADC_TRACE_LEN / 2 * ChanNum), (Trace_Length / 2),
                                     MOD_READ, ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "reading ADC trace failed for module %d, retval=%d",
                              ModNum, retval);

            // Release memory
            free(ADCData);
            return (-5);
        }
        // Transfer to Trace_Buffer
        for (count = 0; count < (Trace_Length / 2); count++) {
            Trace_Buffer[count * 2] = (unsigned short) (ADCData[count] & 0x0000FFFF);
            Trace_Buffer[count * 2 + 1] = (unsigned short) ((ADCData[count] & 0xFFFF0000) >> 16);
        }

        // Release memory
        free(ADCData);
    } else {
        Pixie_Print_Error(PIXIE_FUNC,
                          "failed to allocate memory to store ADC trace data for module %d",
                          ModNum);
        return (-4);
    }

    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Read or write data from/to the DSP internal memory of a Pixie module.
 *
 * Use this function to directly transfer data between the host and the DSP internal memory of a
 * Pixie-16 module.
 *
 * The DSP internal memory is split into two blocks with address range 0x40000 to 0x4FFFF for the
 * first block and address range 0x50000 to 0x5FFFF for the second block. Within the first block,
 * address range 0x40000 to 0x49FFF is reserved for program memory and shouldn’t be accessed
 * directly by the host computer. Address range 0x4A000 to 0x4A4FF is used by the DSP I/O parameters
 * which are stored in the configuration files (.set files) in the host. Within this range, 0x4A000
 * to 0x4A33F can be both read and written, but 0x4A340 to 0x4A4FF can only be read but not written.
 * The remaining address range (0x4A500 to 4FFFF) in the first block and the entire second block
 * (0x50000 to 0x5FFFF) should only be read but not written by the host.
 *
 * @param[in,out] Buffer: Memory block used to hold the data for read or write
 * @param[in] NumWords: Number of words that we're going to read or write.
 * @param[in] Address: The address that we'd like to perform the operation against
 * @param[in] Direction: 0 for writes and 1 for reads.
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - success
 * @retval -1 - null pointer for buffer data
 * @retval -2 - number of buffer words exceeds the limit
 * @retval -3 - invalid DSP internal memory address
 * @retval -4 - invalid I/O direction
 * @retval -5 - invalid Pixie module number
 * @retval -6 - I/O Failure
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16IMbufferIO(unsigned int* Buffer, unsigned int NumWords,
                                                       unsigned int Address,
                                                       unsigned short Direction,
                                                       unsigned short ModNum)
{
    int retval;  // return values
    unsigned int k;

    // Check if Buffer is a valid pointer
    if (Buffer == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "null pointer for buffer data");
        return (-1);
    }

    // Check if NumWords is valid
    if (NumWords > (DSP_IMBUFFER_END_ADDR - DSP_IMBUFFER_START_ADDR + 1)) {
        Pixie_Print_Error(PIXIE_FUNC,
                          "%u (number of buffer words to read or write) exceeds the limit %d",
                          NumWords, (DSP_IMBUFFER_END_ADDR - DSP_IMBUFFER_START_ADDR + 1));
        return (-2);
    }

    // Check if Address is valid
    if ((Address < DSP_IMBUFFER_START_ADDR) || (Address > DSP_IMBUFFER_END_ADDR)) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid DSP internal memory address %u", Address);
        return (-3);
    }

    // Check if Direction is valid
    if ((Direction != MOD_READ) && (Direction != MOD_WRITE)) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid I/O direction %d", Direction);
        return (-4);
    }

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-5);
    }

    // Check if running in OFFLINE mode
    if (Offline == 1) {
        // Only return parameter values stored in DSP_Parameter_Values for MOD_READ
        if ((Direction == MOD_READ)) {
            for (k = 0; k < NumWords; k++) {
                Buffer[k] = Pixie_Devices[ModNum].DSP_Parameter_Values[Address + k - DATA_MEMORY_ADDRESS];
            }
        }
    }

    // I/O
    retval = Pixie_DSP_Memory_IO(Buffer, Address, NumWords, Direction, ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "IMbuffer I/O failed for module %d, retval=%d", ModNum, retval);
        return (-6);
    }

    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Read or write data from/to the external memory of a Pixie module.
 *
 * Use this function to directly read data from or write data to the on-board external memory of a
 * Pixie-16 module. The valid memory address is from 0x0 to 0x7FFFF (32-bit wide).
 *
 * The external memory is used to store the histogram data accumulated for each of the 16 channels
 * of a Pixie-16 module. Each channel has a fixed histogram length of 32768 words (32-bit wide),
 * and the placement of the histogram data in the memory is in the same order of the channel
 * number, i.e. channel 0 occupies memory address 0x0 to 0x7FFF, channel 1 occupies 0x8000 to
 * 0xFFFF, and so on.
 *
 * NOTE: another function Pixie16ReadHistogramFromModule can also be used to read out the
 * histograms except that it needs to be called channel by channel.
 *
 * @param[in,out] Buffer: Memory block used to hold the data for read or write
 * @param[in] NumWords: Number of words that we're going to read or write.
 * @param[in] Address: The address that we'd like to perform the operation against
 * @param[in] Direction: 0 for writes and 1 for reads.
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - success
 * @retval -1 - null pointer for buffer data
 * @retval -2 - number of buffer words exceeds the limit
 * @retval -3 - invalid System FPGA internal memory address
 * @retval -4 - invalid I/O direction
 * @retval -5 - invalid Pixie module number
 * @retval -6 - I/O Failure
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EMbufferIO(unsigned int* Buffer, unsigned int NumWords,
                                                       unsigned int Address,
                                                       unsigned short Direction,
                                                       unsigned short ModNum)
{
    int retval;  // return values

    if (Buffer == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "null pointer for buffer data");
        return (-1);
    }

    if (NumWords > (DSP_EMBUFFER_END_ADDR - DSP_EMBUFFER_START_ADDR + 1)) {
        Pixie_Print_Error(PIXIE_FUNC,
                          "%u (number of buffer words to read or write) exceeds the limit %d",
                          NumWords, (DSP_EMBUFFER_END_ADDR - DSP_EMBUFFER_START_ADDR + 1));
        return (-2);
    }

#if DSP_EMBUFFER_START_ADDR > 0
    if (Address < DSP_EMBUFFER_START_ADDR) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid System FPGA internal memory address %u", Address);
        return (-3);
    }
#endif
    if (Address > DSP_EMBUFFER_END_ADDR) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid System FPGA internal memory address %u", Address);
        return (-3);
    }

    if ((Direction != MOD_READ) && (Direction != MOD_WRITE)) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid I/O direction %d", Direction);
        return (-4);
    }

    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-5);
    }

    retval = Pixie_Main_Memory_IO(Buffer, Address, NumWords, Direction, ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "EMbuffer I/O failed for module %d, retval=%d", ModNum, retval);
        return (-6);
    }

    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Start or resume a list-mode run.
 *
 * Use this function to start a list-mode data acquisition run in Pixie-16 modules. list-mode runs
 * are used to collect data on an event-by-event basis, gathering energies, timestamps, pulse shape
 * analysis values, and waveforms for each event. Runs will continue until the user terminates the
 * run by calling function Pixie16EndRun. To start the data acquisition this function has to be
 * called for every Pixie-16 module in the system. If all modules are to run synchronously, the
 * last module addressed will release all others and the acquisition starts then. The first module
 * to end the run will immediately stop the run in all other modules if run synchronization has
 * been set up among these modules.
 *
 * There is only one list-mode run type supported, that is, 0x100. However, different output data
 * options can be chosen by enabling or disabling different CHANCSRA bits.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will have its list
 * mode run started. But if ModNum is set to equal to the total number of modules in the system,
 * then all modules in the system will have their runs started together.
 *
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 * @param[in] RunType: The type of run that we're going to be executing. There's only one 0x100.
 * @param[in] mode: How we'll handle existing data when starting up the run. Use mode=NEW_RUN (=1)
 *     to erase histograms and statistics information before launching the new
 *     run. Note that this will cause a startup delay of up to 1 millisecond.
 *     Use mode=RESUME_RUN (=0) to resume an earlier run. This mode has a startup delay of only a
 *     few microseconds.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid mode
 * @retval -3 - Failed to start list-mode run
 * @retval -4 - Invalid run type
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartListModeRun(unsigned short ModNum,  // module number
                                                             unsigned short RunType,  // run type
                                                             unsigned short mode)  // run mode
{
    int retval;  // return values

    // Check if running in OFFLINE mode
    if (Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    // Check if ModNum is valid
    if (ModNum > Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid Pixie module number %d", ModNum);
        return (-1);
    }

    if ((mode != NEW_RUN) && (mode != RESUME_RUN)) {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid mode %d", mode);
        return (-2);
    }

    if (RunType != LIST_MODE_RUN) {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid run type %d", mode);
        return (-4);
    }

    // Start the list-mode run now
    retval = Pixie_Start_Run(mode, RunType, 0, ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to start list-mode run, retval=%d", retval);
        return (-3);
    }

    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Start or resume a histogram run.
 *
 * Use this function to begin a data acquisition run that accumulates energy histograms, one for
 * each channel. It launches a data acquisition run in which only energy information is preserved
 * and histogrammed locally to each channel.
 *
 * Call this function for each Pixie-16 module in the system to initialize the run in each module.
 * Actual data acquisition will start synchronously in all modules when the last module finished
 * the initialization (requires the synchronization parameter to be set). Histogram runs can be
 * self-terminating when the elapsed run time exceeds the preset run time, or the user can
 * prematurely terminate the run by calling Pixie16EndRun. On completion, final histogram and
 * statistics data will be available.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will have its
 * histogram run started. But if ModNum is set to be equal to the total number of modules in the
 * system, then all modules in the system will have their runs started together.
 *
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 * @param[in] mode: How we'll handle existing data when starting up the run. Use mode=NEW_RUN (=1)
 *     to erase histograms and statistics information before launching the new
 *     run. Use mode=RESUME_RUN (=0) to resume an earlier run.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid mode
 * @retval -3 - Failed to start histogram run
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartHistogramRun(unsigned short ModNum,  // module number
                                                              unsigned short mode)  // run mode
{
    int retval;  // return values

    // Check if running in OFFLINE mode
    if (Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    // Check if ModNum is valid
    if (ModNum > Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    if ((mode != NEW_RUN) && (mode != RESUME_RUN)) {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid mode %d", mode);
        return (-2);
    }

    // Start the histogram run now
    retval = Pixie_Start_Run(mode, HISTOGRAM_RUN, 0, ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to start histogram run, retval=%d", retval);
        return (-3);
    }

    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Poll the run status of a Pixie module.
 *
 * Use this function to check the run status of a Pixie-16 module while a list-mode data
 * acquisition run is in progress. If the run is still in progress continue polling.
 *
 * If the return code of this function indicates the run has finished, there might still be some
 * data in the external FIFO (Rev-B, C, D, F modules) that need to be read out to the host
 * computer. In addition, final run statistics and histogram data are available for reading out too.
 *
 * In MCA histogram run mode, this function can also be called to check if the run is still in
 * progress even though it is normally self-terminating.
 *
 * @param[in] ModNum: The module number to interrogate and counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - No run is in progress
 * @retval  1 - Run is still in progress
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckRunStatus(unsigned short ModNum)
{
    int retval;  // return values

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    retval = Pixie_Check_Run_Status(ModNum);
    return (retval);
}


/**
 * @ingroup PUBLIC_API
 * @brief Stop the run in a Pixie module.
 *
 * Use this function to end a histogram run, or to force the end of a list-mode run. In a
 * multi-module system, if all modules are running synchronously, only one module needs to be
 * addressed this way. It will immediately stop the run in all other module in the system.
 *
 * When stopping the run in the Director module (module #0) - a SYNC interrupt should be generated
 * to stop run in all modules simultaneously, if DSP parameter SynchWait is set to 1.
 * If SynchWait is set to 0, then CSR bit 0 is cleared to stop the run.
 *
 * @param[in] ModNum: The module number to interrogate and counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - No run is in progress
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EndRun(unsigned short ModNum)  // Pixie module number
{
    unsigned short modulenumber, k;
    unsigned int dummy, CSR;

    // Check if ModNum is valid
    if (ModNum > Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    modulenumber = 0;
    (void) Pixie_Register_IO(modulenumber, PCI_STOPRUN_REGADDR, MOD_WRITE, &dummy);

    if (ModNum == Number_Modules)  // Stop run in all modules
    {
        for (k = 0; k < Number_Modules; k++) {
            // Read CSR and clear bit RUNENA to stop the run
            Pixie_ReadCSR(k, &CSR);
            CSR = APP32_ClrBit(RUNENA, CSR);
            Pixie_WrtCSR(k, CSR);
        }
    } else if (ModNum < Number_Modules)  // Stop run in one module only
    {
        // Read CSR and clear bit RUNENA to stop the run
        Pixie_ReadCSR(ModNum, &CSR);
        CSR = APP32_ClrBit(RUNENA, CSR);
        Pixie_WrtCSR(ModNum, CSR);
    }

    return (0);
}


/**
 * @defgroup RUN_STATISTICS Run Statistics
 * @ingroup PUBLIC_API
 * A group of functions used calculate run statistics from the return of
 * Pixie16ReadStatisticsFromModule.
 * @see Pixie16ReadStatisticsFromModule
 */

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute input count rate of a channel
 *
 * Use this function to calculate the input count rate on one channel of a Pixie-16 module. This
 * function does not communicate with Pixie-16 modules. Before calling this function, another
 * function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the
 * module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @note The units on the return value are triggers per second.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum: ChanNum is the channel number which starts counting at 0.
 * @return 0 if the live time was 0. The number of triggers divided by the live time in seconds
 *     otherwise.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeInputCountRate(unsigned int* Statistics, unsigned short ModNum,
                                                                     unsigned short ChanNum) {
    double FastPeaks, LiveTime;

    // The fast peaks start after DSP_IO_BORDER
    FastPeaks = (double) Statistics[FastPeaksA_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] *
                pow(2.0, 32.0);
    FastPeaks += (double) Statistics[FastPeaksB_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];

    // The live times start after DSP_IO_BORDER
    LiveTime = (double) Statistics[LiveTimeA_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] *
               pow(2.0, 32.0);
    LiveTime += (double) Statistics[LiveTimeB_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];

    if (Module_Information[ModNum].Module_ADCMSPS == 100)
        LiveTime *= 1.0e-6 / (double) Module_Information[ModNum].Module_ADCMSPS;
    else if (Module_Information[ModNum].Module_ADCMSPS == 250)
        LiveTime *= 2.0 * 1.0e-6 / (double) Module_Information[ModNum].Module_ADCMSPS;
    else if (Module_Information[ModNum].Module_ADCMSPS == 500)
        LiveTime *= 5.0 * 1.0e-6 / (double) Module_Information[ModNum].Module_ADCMSPS;

    if (LiveTime == 0.0)
        return (0.0);
    else
        return (FastPeaks / LiveTime);
}

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute output count rate of a channel
 *
 * Use this function to calculate the output count rate on one channel of a Pixie-16 module. This
 * function does not communicate with Pixie-16 modules. Before calling this function, another
 * function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the
 * module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @note The return value's units are channel events per second.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum: ChanNum is the channel number which starts counting at 0.
 * @return 0 if the live time was 0. Otherwise, the number of channel events divided by the
 *     live time in seconds.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeOutputCountRate(unsigned int* Statistics, unsigned short ModNum,
                                                                      unsigned short ChanNum) {
    double ChanEvents, RealTime;

    // The channel processed events start after DSP_IO_BORDER
    ChanEvents = (double) Statistics[ChanEventsA_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] *
                 pow(2.0, 32.0);
    ChanEvents += (double) Statistics[ChanEventsB_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];

    // The real times start after DSP_IO_BORDER
    RealTime = (double) Statistics[RunTimeA_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
    RealTime += (double) Statistics[RunTimeB_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];
    RealTime *= 1.0e-6 / (double) SYSTEM_CLOCK_MHZ;

    if (RealTime == 0.0)
        return (0.0);
    else
        return (ChanEvents / RealTime);
}

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute live time that a channel accumulated in a run
 *
 * Use this function to calculate the live time that one channel of a Pixie-16 module has spent on
 * data acquisition. This function does not communicate with Pixie-16 modules. Before calling this
 * function, another function, Pixie16ReadStatisticsFromModule, should be called to read
 * statistics data from the module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number, which starts counting at 0.
 * @param[in] ChanNum: ChanNum is the channel number, which starts counting at 0.
 * @return The live time of the module in seconds.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeLiveTime(unsigned int* Statistics, unsigned short ModNum,
                                                               unsigned short ChanNum) {
    double LiveTime;

    // The live times start after DSP_IO_BORDER
    LiveTime = (double) Statistics[LiveTimeA_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] *
               pow(2.0, 32.0);
    LiveTime += (double) Statistics[LiveTimeB_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];

    if (Module_Information[ModNum].Module_ADCMSPS == 100)
        LiveTime *= 1.0e-6 / (double) Module_Information[ModNum].Module_ADCMSPS;
    else if (Module_Information[ModNum].Module_ADCMSPS == 250)
        LiveTime *= 2.0 * 1.0e-6 / (double) Module_Information[ModNum].Module_ADCMSPS;
    else if (Module_Information[ModNum].Module_ADCMSPS == 500)
        LiveTime *= 5.0 * 1.0e-6 / (double) Module_Information[ModNum].Module_ADCMSPS;

    return (LiveTime);
}

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute number of events processed by a module
 *
 * @note This function is only used by Rev-A modules.
 *
 * Use this function to calculate the number of events that have been processed by a Pixie-16
 * module during a data acquisition run. This function does not communicate with Pixie-16 modules.
 * Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be
 * called to read statistics data from the module first.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number, which starts counting at 0.
 * @return The number of events processed by the module.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeProcessedEvents(unsigned int* Statistics, unsigned short ModNum) {
    double ProcessedEvents;

    // The processed events start after DSP_IO_BORDER
    ProcessedEvents =
            (double) Statistics[NumEventsA_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
    ProcessedEvents += (double) Statistics[NumEventsB_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];

    return (ProcessedEvents);
}

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute real time that a module accumulated in a run
 *
 * Use this function to calculate the real time that a Pixie-16 module has spent on data
 * acquisition.This function does not communicate with Pixie-16 modules.
 * Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be
 * called to read statistics data from the module first.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: The module number, which starts counting at 0.
 * @return The number of seconds that the module spent on data acquisition.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeRealTime(unsigned int* Statistics, unsigned short ModNum) {
    double RealTime;

    // The real time starts after DSP_IO_BORDER
    RealTime = (double) Statistics[RunTimeA_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
    RealTime += (double) Statistics[RunTimeB_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];
    RealTime *= 1.0e-6 / (double) SYSTEM_CLOCK_MHZ;

    return (RealTime);
}


/**
 * @defgroup HELPER_FUNCTIONS Helper Functions
 * @ingroup PUBLIC_API
 * A group of functions used to support other functionality in the API.
 */

/**
 * @ingroup HELPER_FUNCTIONS
 * @brief Computes the Complex FFT of a trace using Cooley-Turkey algorithm
 *
 * Implementation based on Numerical Recipes in C.
 *
 * @see https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
 *
 * @param[in,out] data: A pointer to an array containing the data we'll transform with the
 *    complex FFT. This array is overwritten with the results of the transformation.
 * @param[in] length: The length of the data array, which **must** be a power of 2. Ex. 8192, 16384
 * @returns Zero, always.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16complexFFT(double* data, unsigned int length) {
    double tempR;
    double tempI;
    double theta;
    double wr;
    double wi;
    double wpr;
    double wpi;
    double wtemp;

    unsigned int nComplex;
    unsigned int m;
    unsigned int mmax;
    unsigned int iStep;
    unsigned int i, j;

    nComplex = 2 * length;

    // Do the bit reversal re-ordering
    j = 0;
    for (i = 0; i < nComplex; i += 2) {
        if (j > i) {
            tempR = data[j];
            tempI = data[j + 1];
            data[j] = data[i];
            data[j + 1] = data[i + 1];
            data[i] = tempR;
            data[i + 1] = tempI;
        }

        m = nComplex / 2;
        while ((m >= 2) && (j >= m)) {
            j = j - m;
            m = m / 2;
        }

        j = j + m;
    }

    // Do the recursive FFT
    mmax = 2;
    while (nComplex > mmax) {
        iStep = 2 * mmax;
        theta = PI2 / mmax;
        wpr = sin(0.5 * theta);
        wpr = -2. * wpr * wpr;
        wpi = sin(theta);
        wr = 1.;
        wi = 0.;
        for (m = 0; m < mmax; m += 2) {
            for (i = m; i < nComplex; i += iStep) {
                j = i + mmax;
                tempR = wr * data[j] - wi * data[j + 1];
                tempI = wr * data[j + 1] + wi * data[j];
                data[j] = data[i] - tempR;
                data[j + 1] = data[i + 1] - tempI;
                data[i] = data[i] + tempR;
                data[i + 1] = data[i + 1] + tempI;
            }
            wtemp = wr;
            wr = wr * wpr - wi * wpi + wr;
            wi = wi * wpr + wtemp * wpi + wi;
        }
        mmax = iStep;
    }

    return 0;
}

/**
 * @ingroup HELPER_FUNCTIONS
 * @brief Test a bit in a 16-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 16-bit number that we'll test to see if the requested bit is true.
 * @return 0 if the bit was set, 1 otherwise.
 */
PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_TstBit(unsigned short bit, unsigned short value) {
    if (bit > 15)
        return FALSE_;
    return (((value & (unsigned short) (pow(2.0, (double) bit))) >> bit));
}

/**
 * @ingroup HELPER_FUNCTIONS
 * @brief Set a bit in a 16-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 16-bit number that will have its bit set to true.
 * @return The new integer after the bit has been toggled.
 */
PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_SetBit(unsigned short bit, unsigned short value) {
    if(bit > 15)
        return value;
    return (value | (unsigned short) (pow(2.0, (double) bit)));
}

/**
 * @ingroup HELPER_FUNCTIONS
 * @brief Clear a bit in a 16-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 16-bit number that will have its bit set to false.
 * @return The new integer after the bit has been toggled.
 */
PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_ClrBit(unsigned short bit, unsigned short value) {
    if(bit > 15)
        return value;
    value = APP16_SetBit(bit, value);
    return (value ^ (unsigned short) (pow(2.0, (double) bit)));
}

/**
 * @ingroup HELPER_FUNCTIONS
 * @brief Set a bit in a 32-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 32-bit number that will have its bit set to true.
 * @return The new integer after the bit has been toggled.
 */
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_SetBit(unsigned short bit, unsigned int value) {
    if(bit > 31)
        return value;
    return (value | (unsigned int) (pow(2.0, (double) bit)));
}

/**
 * @ingroup HELPER_FUNCTIONS
 * @brief Clear a bit in a 32-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 32-bit number that will have its bit set to false.
 * @return The new integer after the bit has been toggled.
 */
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_ClrBit(unsigned short bit, unsigned int value) {
    if (bit > 31)
        return value;
    value = APP32_SetBit(bit, value);
    return (value ^ (unsigned int) (pow(2.0, (double) bit)));
}

/**
 * @ingroup HELPER_FUNCTIONS
 * @brief Test a bit in a 32-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 32-bit number that we'll test to see if the requested bit is true.
 * @return 0 if the bit was set, 1 otherwise.
 */
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_TstBit(unsigned short bit, unsigned int value) {
    if(bit > 31)
        return FALSE_;
    return (((value & (unsigned int) (pow(2.0, (double) bit))) >> bit));
}

/**
 * @defgroup CONTROL_TASK_RUN Control Task functions
 * @ingroup PUBLIC_API
 * A group of functions used to execute specific tasks on the module.
 */

/**
 * @ingroup CONTROL_TASK_RUN
 * @brief Set all DACs.
 *
 * Use this function to reprogram the on-board digital to analog converters (DAC) of the Pixie-16
 * modules. In this operation the DSP uses data from the DSP parameters that were previously
 * downloaded.
 *
 * @param[in] ModNum: The module number, which starts counting at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the SET_DACs run
 * @retval -3 - SET_DACs run timed out after 10 seconds of trying.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SetDACs(unsigned short ModNum) {
    int retval;
    unsigned int Max_Poll;

    // Check if ModNum is valid
    if (ModNum > Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    Max_Poll = 10000;  // The maximal waiting time is 10 s
    retval = Pixie_Control_Task_Run(ModNum, SET_DACS, Max_Poll);
    if (retval == -1) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to start SET_DACS run; retval=%d", retval);
        return (-2);
    } else if (retval == -2) {
        Pixie_Print_Error(PIXIE_FUNC, "SET_DACS run timed out; retval=%d", retval);
        return (-3);  // Time Out
    }

    return (0);  // Normal finish
}


/**
 * @ingroup CONTROL_TASK_RUN
 * @brief Program Fippi
 *
 * Use this function to program the on-board signal processing FPGAs of the Pixie-16 modules.
 * After the host computer has written the DSP parameters to the DSP memory, the DSP needs to
 * write some of these parameters to the FPGAs. This function makes the DSP perform that action.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the PROGRAM_FIPPI run
 * @retval -3 - PROGRAM_FIPPI run timed out after 10 seconds of trying.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ProgramFippi(unsigned short ModNum) {
    int retval;
    unsigned int Max_Poll;

    // Check if ModNum is valid
    if (ModNum > Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    Max_Poll = 10000;  // The maximal waiting time is 10 s
    retval = Pixie_Control_Task_Run(ModNum, PROGRAM_FIPPI, Max_Poll);
    if (retval == -1) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to start PROGRAM_FIPPI run; retval=%d", retval);
        return (-2);
    } else if (retval == -2) {
        Pixie_Print_Error(PIXIE_FUNC, "PROGRAM_FIPPI run timed out; retval=%d", retval);
        return (-3);  // Time Out
    }

    return (0);  // Normal finish
}


/**
 * @ingroup CONTROL_TASK_RUN
 * @brief Adjust DC-Offsets
 *
 * Use this function to adjust the DC-offsets of Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its DC-offsets adjusted. But if ModNum is
 * set to be equal to the total number of modules in the system, then all modules in the system
 * will have their DC-offsets adjusted.
 *
 * After the DC-offset levels have been adjusted, the baseline level of the digitized input
 * signals will be determined by the DSP parameter BaselinePercent. For instance, if
 * BaselinePercent is set to 10(%), the baseline level of the input signals will be ~ 409 on the
 * 12-bit ADC scale (minimum: 0; maximum: 4095).
 *
 * The main purpose of this function is to ensure the input signals fall within the voltage range
 * of the ADCs so that all input signals can be digitized by the ADCs properly.
 *
 * @param[in] ModNum: The module number to adjust baselines for
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the ADJUST_OFFSETS run
 * @retval -3 - ADJUST_OFFSETS run timed out after 10 seconds of attempts
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AdjustOffsets(unsigned short ModNum) {
    int retval;
    unsigned int Max_Poll;

    // Check if ModNum is valid
    if (ModNum > Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    Max_Poll = 10000;  // The maximal waiting time is 10 s
    retval = Pixie_Control_Task_Run(ModNum, ADJUST_OFFSETS, Max_Poll);
    if (retval == -1) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to start ADJUST_OFFSETS run; retval=%d", retval);
        return (-2);
    } else if (retval == -2) {
        Pixie_Print_Error(PIXIE_FUNC, "ADJUST_OFFSETS run timed out; retval=%d", retval);
        return (-3);
    }

    return (0);
}

/**
 * @ingroup CONTROL_TASK_RUN
 * @brief Acquire baselines
 *
 * Use this function to acquire baselines from Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its baselines acquired. But if ModNum is
 * set to be equal to the total number of modules in the system, then all modules in the system
 * will have their baselines acquired.
 *
 * After the successful return of this function, the DSP’s internal memory will be filled with
 * baselines data. Users should then call the function Pixie16ReadSglChanBaselines to read the
 * baselines data out to the host computer, channel by channel.
 *
 * @see Pixie16ReadSglChanBaselines
 *
 * @param[in] ModNum: The module number to get baselines from
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the GET_BASELINES run
 * @retval -3 - GET_BASELINES run timed out
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireBaselines(unsigned short ModNum)
{
    int retval;
    unsigned int Max_Poll, sfr;

    // Check if ModNum is valid
    if (ModNum > Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Get the SlowFilteRange value
    sfr = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];

    // Compute Max_Poll, in ms
    // Factor 2 added to avoid time out
    Max_Poll = 2 * (unsigned int) (2.0 * (double) SLOWFILTER_MAX_LEN * pow(2.0, (double) sfr) *
                                   (double) MAX_NUM_BASELINES / (double) DSP_CLOCK_MHZ / 1000.0);

    // Need extra delay if SlowFilterRange is lower than 3 due to GetBaselines overhead in the DSP
    if (sfr <= 2) {
        Max_Poll = (unsigned int) ((double) Max_Poll * pow(2.0, (3.0 - (double) sfr)));
    }

    // Acquire the baselines
    retval = Pixie_Control_Task_Run(ModNum, GET_BASELINES, Max_Poll);
    if (retval == -1) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to start GET_BASELINES run; retval=%d", retval);
        return (-2);
    } else if (retval == -2) {
        Pixie_Print_Error(PIXIE_FUNC, "GET_BASELINES run timed out; retval=%d", retval);
        return (-3);  // Time Out
    }

    return (0);  // Normal finish
}

/**
 * @defgroup CONFIGURATION Configuration Management
 * @ingroup PUBLIC_API
 * A group of functions used to setup and configure the modules
 */

/**
 * @ingroup CONFIGURATION
 * @brief Read baselines from a channel in a module
 *
 * Use this function to read baseline data from a Pixie-16 module. Before calling this function,
 * another function Pixie16AcquireBaselines should be called to fill the DSP internal memory first.
 * Also, the host code should allocate appropriate amount of memory to store the baseline data.
 * The number of baselines for each channel is 3640. In the DSP internal memory, each baseline is
 * a 32-bit IEEE floating point number. After being read out to the host, this function will
 * convert each baseline data to a decimal number. In addition to baseline values, timestamps
 * corresponding to each baseline are also returned after this function call.
 *
 * @see Pixie16AcquireBaselines
 *
 * @param[out] Baselines: Buffer to hold the baseline values
 * @param[in] TimeStamps: The timestamp that the baselines were obtained at
 * @param[in] NumBases: The number of baselines values read
 * @param[in] ModNum: The module number to read the baselines from, counting from 0.
 * @param[in] ChanNum: The channel number to read the baselines from, counting from 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Requested number of baselines exceeded the limit
 * @retval -3 - Failed to allocate memory to store baselines
 * @retval -4 - Failed to read baselines
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanBaselines(double* Baselines,
                                                                 double* TimeStamps,
                                                                 unsigned short NumBases,
                                                                 unsigned short ModNum,
                                                                 unsigned short ChanNum)
{
    unsigned int k;
    unsigned int* buffer;
    double startbasetime;
    int retval;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Check if NumBases is within limit
    if (NumBases > MAX_NUM_BASELINES) {
        Pixie_Print_Error(PIXIE_FUNC,
                          "requested number of baselines (%d) exceeded the limit (%d)",
                          NumBases, MAX_NUM_BASELINES);
        return (-2);
    }

    // Allocate memory
    if ((buffer = (unsigned int*) malloc(sizeof(unsigned int) * IO_BUFFER_LENGTH)) != NULL) {

        retval = Pixie_DSP_Memory_IO(buffer, IO_BUFFER_ADDRESS, IO_BUFFER_LENGTH, MOD_READ, ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "reading baselines failed for module %d, retval=%d",
                              ModNum, retval);

            // Release memory
            free(buffer);
            return (-4);
        }

        // Compute timestamp at the beginning of the baseline capture
        startbasetime = (double) buffer[0] * pow(2.0, 32.0);
        startbasetime += (double) buffer[1];
        startbasetime *= 1.0e-6 / (double) SYSTEM_CLOCK_MHZ;

        // Convert the baselines for the selected channel
        for (k = 0; k < NumBases; k++) {
            TimeStamps[k] = (double) buffer[2 + k * BASELINES_BLOCK_LEN] * pow(2.0, 32.0);
            TimeStamps[k] += (double) buffer[2 + k * BASELINES_BLOCK_LEN + 1];
            TimeStamps[k] *= 1.0e-6 / (double) SYSTEM_CLOCK_MHZ;
            TimeStamps[k] -= startbasetime;

            Baselines[k] = IEEEFloating2Decimal(buffer[2 + k * BASELINES_BLOCK_LEN + 2 + ChanNum]);
        }

        // Release memory
        free(buffer);
    } else {
        Pixie_Print_Error(PIXIE_FUNC,
                          "failed to allocate memory to store baselines for module %d",
                          ModNum);
        return (-3);
    }

    return (0);  // Normal finish
}


/**
 * @ingroup CONTROL_TASK_RUN
 * @brief Ramp Offset DACs of a module and record the baselines
 *
 * @note However, this function is no longer needed due to the introduction of function
 *     Pixie16AdjustOffsets.
 *
 * Use this function to execute the RAMP_OFFSETDACS control task run. Each Offset DAC has 65536
 * steps, and the RAMP_OFFSETDACS control task ramps the DAC from 0 to 65335 with a step size of
 * 64, i.e., a total of 1024 steps. At each DAC step, the control task computes the baseline value
 * as the representation of the signal baseline and stores it in the DSP memory. After the control
 * task is finished, the stored baseline values are read out to the host computer and saved to a
 * binary file called “rampdacs.bin” in the form of IEEE 32-bit floating point numbers. Users can
 * then plot the baseline values vs. DAC steps to determine the appropriate DAC value to be set in
 * the DSP in order to bring the input signals into the voltage range of the ADCs. However, this
 * function is no longer needed due to the introduction of function Pixie16AdjustOffsets.
 *
 * If ModNum is set to less than the total number of modules in the system, only the module
 * specified by ModNum will start the RAMP_OFFSETDACS control task run. But if ModNum is equal to
 * the total number of modules in the system, e.g. there are 5 modules in the chassis and
 * ModNum = 5, then all modules in the system will start the RAMP_OFFSETDACS control task run.
 *
 * @see Pixie16AdjustOffsets
 *
 * @param[out] DCValues: The final values for the DC offsets
 * @param[in] NumDCVals: The number of DC values to read
 * @param[in] ModNum: The module number we'll query for the DACs, counting starts at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Null pointer *DCValues
 * @retval -3 - requested number of DC values exceeded the limit
 * @retval -4 - Failed to start the RAMP_OFFSETDACS run
 * @retval -5 - RAMP_OFFSETDACS run timed out
 * @retval -6 - Failed to read DC values
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RampOffsetDACs(double* DCValues,
                                                           unsigned short NumDCVals,
                                                           unsigned short ModNum) {
    int retval;
    unsigned int Max_Poll, k;
    unsigned int buffer[MAX_NUM_DCVALUES];

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Check if DCValues is valid
    if (DCValues == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "Null pointer *DCValues");
        return (-2);
    }

    // Check if NumDCVals is within limit
    if (NumDCVals > MAX_NUM_DCVALUES) {
        Pixie_Print_Error(PIXIE_FUNC, "requested number of DC values (%d) exceeded the limit (%d)",
                          NumDCVals, MAX_NUM_DCVALUES);
        return (-3);
    }

    Max_Poll = 10000;  // The maximal waiting time is 10 s
    retval = Pixie_Control_Task_Run(ModNum, RAMP_OFFSETDACS, Max_Poll);
    if (retval == -1) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to start RAMP_OFFSETDACS run; retval=%d", retval);
        return (-4);
    } else if (retval == -2) {
        Pixie_Print_Error(PIXIE_FUNC, "RAMP_OFFSETDACS run timed out; retval=%d", retval);
        return (-5);  // Time Out
    }

    // Read out data memory
    retval = Pixie_DSP_Memory_IO(buffer, IO_BUFFER_ADDRESS, MAX_NUM_DCVALUES, MOD_READ, ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "reading OffsetDAC values failed for module %d, retval=%d",
                          ModNum, retval);
        return (-6);
    }

    // Convert DC values
    for (k = 0; k < NumDCVals; k++) { *DCValues++ = IEEEFloating2Decimal(buffer[k]); }

    return (0);  // Normal finish
}


/**
 * @ingroup CONTROL_TASK_RUN
 * @brief  Execute special control tasks
 *
 * Use this function to execute special control tasks. This may include programming the Fippi or
 * setting the DACs after downloading DSP parameters.
 *
 * @see Control Tasks
 *
 * @param[in] ModNum: The module number to execute the control task against. Starts counting with 0.
 * @param[in] ControlTask : The control task number that we're going to execute.
 * @param[in] Max_Poll: The time in milliseconds that we should wait before considering the task
 *   timed out.
 * @returns A status code indicating the result of the operation
 * @retval   0 - successful
 * @retval  -1 - Invalid Pixie module number
 * @retval  -2 - control task run failed
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ControlTaskRun(unsigned short ModNum,
                                                           unsigned short ControlTask,
                                                           unsigned int Max_Poll)
{
    int retval;

    // Check if ModNum is valid
    if (ModNum > Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    retval = Pixie_Control_Task_Run(ModNum, ControlTask, Max_Poll);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "control task run %d failed", ControlTask);
        return (-2);
    }

    return (0);
}


/**
 * @ingroup CONFIGURATION
 * @brief Find the BLcut value for the selected channel and return it using pointer *BLcut.
 *
 * Use this function to find the Baseline Cut value for one channel of a Pixie-16 module. The
 * baseline cut value is then downloaded to the DSP, where baselines are captured and averaged
 * over time. The cut value would prevent a bad baseline value from being used in the averaging
 * process, i.e., if a baseline value is outside the baseline cut range, it will not be used for
 * computing the baseline average. Averaging baselines over time improves energy resolution
 * measurement.
 *
 * @param[in] ModNum: The module number to work with, starts counting at 0.
 * @param[in] ChanNum: The channel number to work with, starts counting at 0.
 * @param[out] BLcut: The Baseline cut as determined by the calculation.
 * @returns A status code indicating the result of the operation
 * @retval  0 - successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - failed to collect baselines
 * @retval -3 - failed to read baselines from the data memory
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BLcutFinder(unsigned short ModNum,
                                                        unsigned short ChanNum,
                                                        unsigned int* BLcut)
{
    unsigned int value, KeepLog, KeepBLCut, localBlCut, LC, k;
    double baseline[MAX_NUM_BASELINES], ts[MAX_NUM_BASELINES];
    double sdev, sdevCount, BLsigma, val;
    int retval;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    /*****************************************************
	*
	*	Set proper DSP parameters for collecting baselines
	*
	*****************************************************/

    // Store the DSP parameter Log2BWeight value
    KeepLog = Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
    // Temporarily set Log2BWeight to 0
    value = 0;
    Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = value;
    Pixie16IMbufferIO(&value, 1, (unsigned int) (Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

    // Store the DSP parameter BLCut value
    KeepBLCut = Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
    // Temporarily set BLcut to 0
    value = 0;
    Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = value;
    Pixie16IMbufferIO(&value, 1, (unsigned int) (BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

    /*****************************************************
	*
	*	Start to collect baselines
	*
	*****************************************************/

    sdev = 0.0;
    sdevCount = 0.0;
    LC = 0;
    localBlCut = 0;

    do {
        // Start Control Task COLLECT_BASES to collect baselines
        retval = Pixie16AcquireBaselines(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to collect baselines in module %d", ModNum);

            // Restore DSP parameter Log2BWeight
            Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                    KeepLog;
            Pixie16IMbufferIO(&KeepLog, 1, (unsigned int) (Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

            // Restore DSP parameter BLCut
            Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                    KeepBLCut;
            Pixie16IMbufferIO(&KeepBLCut, 1, (unsigned int) (BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

            return (-2);  // failed to collect baselines
        }

        // Read the baselines from the data memory
        retval = Pixie16ReadSglChanBaselines(baseline, ts, MAX_NUM_BASELINES, ModNum, ChanNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to read baselines in module %d channel %d", ModNum,
                              ChanNum);

            return (-3);  // failed to read baselines from the data memory
        }

        for (k = 0; k < (MAX_NUM_BASELINES - 1); k += 2) {
            val = fabs(baseline[k] - baseline[k + 1]);

            if (val != 0) {
                // We need a safety check here
                if ((val < (10.0 * baseline[k])) && (val < (10.0 * baseline[k + 1]))) {
                    if (localBlCut == 0) {
                        sdev += val;
                        sdevCount += 1.0;
                    } else {
                        if (val < localBlCut) {
                            sdev += val;
                            sdevCount += 1.0;
                        }
                    }
                }
            }
        }

        LC += 1;
        if (LC > 10)
            break;

    } while (sdevCount < 1000);

    // Calculate BLcut
    if (sdevCount > 0) {
        BLsigma = sdev * sqrt(PI / 2) / sdevCount;
        localBlCut = (unsigned int) floor(8.0 * BLsigma);
    } else {
        localBlCut = 0;
    }

    // Write the new BLCut value to the DSP
    Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = localBlCut;
    Pixie16IMbufferIO(&localBlCut, 1, (unsigned int) (BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);


    /*****************************************************
	*
	*	Collect baselines again using the newly calculated BLcut
	*
	*****************************************************/

    sdev = 0.0;
    sdevCount = 0.0;
    LC = 0;
    localBlCut = 0;

    do {
        // Start Control Task COLLECT_BASES to collect baselines
        retval = Pixie16AcquireBaselines(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to collect baselines in module %d", ModNum);

            // Restore DSP parameter Log2BWeight
            Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                    KeepLog;
            Pixie16IMbufferIO(&KeepLog, 1, (unsigned int) (Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

            // Restore DSP parameter BLCut
            Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                    KeepBLCut;
            Pixie16IMbufferIO(&KeepBLCut, 1, (unsigned int) (BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

            return (-2);  // failed to collect baselines
        }

        // Read the baselines from the data memory
        retval = Pixie16ReadSglChanBaselines(baseline, ts, MAX_NUM_BASELINES, ModNum, ChanNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to read baselines in module %d channel %d", ModNum,
                              ChanNum);

            return (-3);  // failed to read baselines from the data memory
        }

        for (k = 0; k < (MAX_NUM_BASELINES - 1); k += 2) {
            val = fabs(baseline[k] - baseline[k + 1]);

            if (val != 0) {
                // We need a safety check here
                if ((val < (10.0 * baseline[k])) && (val < (10.0 * baseline[k + 1]))) {
                    if (localBlCut == 0) {
                        sdev += val;
                        sdevCount += 1.0;
                    } else {
                        if (val < localBlCut) {
                            sdev += val;
                            sdevCount += 1.0;
                        }
                    }
                }
            }
        }

        LC += 1;
        if (LC > 10)
            break;

    } while (sdevCount < 1000);

    // Calculate BLcut
    if (sdevCount > 0) {
        BLsigma = sdev * sqrt(PI / 2) / sdevCount;
        localBlCut = (unsigned int) floor(8.0 * BLsigma);
    } else {
        localBlCut = 0;
    }

    // Write the new BLCut value to the DSP
    Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = localBlCut;
    Pixie16IMbufferIO(&localBlCut, 1, (unsigned int) (BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

    /*****************************************************
	*
	*	Restore several DSP parameters
	*
	*****************************************************/

    // Restore DSP parameter Log2BWeight
    Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = KeepLog;
    Pixie16IMbufferIO(&KeepLog, 1, (unsigned int) (Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

    // Update BLcut return value
    *BLcut = localBlCut;

    return (0);
}


/**
 * @ingroup CONTROL_TASK_RUN
 * @brief Find the exponential decay constant of the detector/preamplifier signals.
 *
 * Use this function to find the exponential decay time constants (Tau value) of the detector or
 * preamplifier signal that is connected to each of the 16 channels of a Pixie-16 module. Tau
 * is used as part of the on-board energy calculations.
 *
 * @see [High Rate Pulse Processing Algorithms for Microcalorimeters](https://xia.com/wp-content/uploads/2021/03/High-Rate-Pulse-Processing-Algorithms-for-Microcalorimeters-Tan-2008-.pdf)
 *
 * @param[in] ModNum: The module number to work with. Starts counting at 0.
 * @param[out] Tau: Pointer to the array containing the calculated tau values. A value of -1.0
 *     indicates a failed calculation for that channel. Units are microseconds.
 * @returns A status code indicating the result of the operation
 * @retval  0 - success
 * @retval -1 - invalid Pixie module number
 * @retval -2 - failed to start TAU_FINDER run
 * @retval -3 - TAU_FINDER run timed out
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16TauFinder(unsigned short ModNum, double* Tau)
{

    unsigned int Max_Poll;
    unsigned int autotau[NUMBER_OF_CHANNELS];
    unsigned short k;
    int retval;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    Max_Poll = 10000;  // The maximal waiting time is 10 s
    retval = Pixie_Control_Task_Run(ModNum, TAU_FINDER, Max_Poll);
    if (retval == -1) {
        Pixie_Print_Error(PIXIE_FUNC, "failed to start TAU_FINDER run; retval=%d", retval);
        return (-2);
    } else if (retval == -2) {
        Pixie_Print_Error(PIXIE_FUNC, "TAU_FINDER run timed out; retval=%d", retval);
        return (-3);  // Time Out
    }

    // Read back the tau values found by the DSP
    Pixie16IMbufferIO(autotau, NUMBER_OF_CHANNELS, AutoTau_Address[ModNum], MOD_READ, ModNum);

    // Return found tau values to the calling function
    for (k = 0; k < NUMBER_OF_CHANNELS; k++) {
        if (autotau[k] == 0xFFFFFFFF)  // 0xFFFFFFFF indicates unsuccessful tau finding for this channel
        {
            *Tau++ = -1.0;
        } else {
            *Tau++ = IEEEFloating2Decimal(autotau[k]);
        }
    }

    return (0);
}


/**
 * @ingroup CONFIGURATION
 * @brief Write a module parameter to a Pixie module
 *
 * #### List of module parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | MODULE_CSRA | bit pattern | user set | MODCSRA |
 * | MODULE_CSRB | bit pattern | user set | MODCSRB |
 * | MODULE_FORMAT | None | auto DSP | MODFORMAT |
 * | MAX_EVENTS | None | user set/auto DSP | MAXEVENTS |
 * | SYNCH_WAIT | logic (0, 1) | user set | SYNCHWAIT |
 * | IN_SYNCH | logic (0, 1) | user/DSP set | INSYNCH |
 * | SLOW_FILTER_RANGE | None | user set | SLOWFILTERRANGE |
 * | FAST_FILTER_RANGE | None | user set | FASTFILTERRANGE |
 * | FastTrigBackplaneEna | bit pattern | user set | FASTTRIGBACKPLANEENA |
 * | CrateID | None | user set | CRATEID |
 * | SlotID | None | user set | SLOTID |
 * | ModID | None | user set | MODID |
 * | TrigConfig0 | None | user set | TRIGCONFIG[0] |
 * | TrigConfig1 | None | user set | TRIGCONFIG[1] |
 * | TrigConfig2 | None | user set | TRIGCONFIG[2] |
 * | TrigConfig3 | None | user set | TRIGCONFIG[3] |
 * | HOST_RT_PRESET | seconds | user set | HOSTRUNTIMEPRESET |
 *
 * @param[in] ModParName: The name of the module parameter that we'll write
 * @param[in] ModParData: The value of the module parameter that we'll write
 * @param[in] ModNum: The module number we'll write to. Numbering starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid module parameter name
 * @retval -3 - Failed to program Fippi after downloading module parameter
 * @retval -4 - Failed to find BLcut after downloading module parameter
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglModPar(const char* ModParName,
                                                           unsigned int ModParData,
                                                           unsigned short ModNum)
{
    unsigned short k;
    unsigned int paflength, triggerdelay, tracedelay, fastfilterrange, LastFastFilterRange, baselinecut;
    unsigned int CSR, slowfilterrange;
    int retval;
    unsigned int CPLD_CSR;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Write module parameter

    if (strcmp(ModParName, "MODULE_CSRA") == 0) {
        // Update the DSP parameter ModCSRA
        Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRA_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, ModCSRA_Address[ModNum], MOD_WRITE, ModNum);

        // Program FiPPI to apply ModCSRA settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "ProgramFippi failed after downloading ModCSRA, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "MODULE_CSRB") == 0) {
        // Update the DSP parameter ModCSRB
        Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, ModCSRB_Address[ModNum], MOD_WRITE, ModNum);

        // Program FiPPI to apply ModCSRB settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "ProgramFippi failed after downloading ModCSRB, retval=%d",
                              retval);
            return (-3);
        }

        //**************************************************
        // Set up Pull-up resistors
        //**************************************************

        CPLD_CSR = 0xAAA;

        // Pull-up
        if (APP32_TstBit(MODCSRB_CPLDPULLUP, ModParData)) {
            CPLD_CSR = APP32_SetBit(CPLDCSR_PULLUP, CPLD_CSR);
        } else {
            CPLD_CSR = APP32_ClrBit(CPLDCSR_PULLUP, CPLD_CSR);
        }

        // Enable connections of PXI nearest neighbor lines (J2) onto the
        // backplane if the module is a Rev-B or C module
        if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC)) {
            CPLD_CSR = APP32_SetBit(CPLDCSR_BPCONNECT, CPLD_CSR);
        }

        Pixie_Register_IO(ModNum, CFG_CTRLCS, MOD_WRITE, &CPLD_CSR);

        //**************************************************
        // Set pullups for the SYNCH lines on the backplane
        //**************************************************

        Pixie_ReadCSR(ModNum, &CSR);
        if (APP32_TstBit(MODCSRB_CHASSISMASTER, ModParData)) {
            CSR = APP32_SetBit(PULLUP_CTRL, CSR);
        } else {
            CSR = APP32_ClrBit(PULLUP_CTRL, CSR);
        }
        Pixie_WrtCSR(ModNum, CSR);

    } else if (strcmp(ModParName, "MODULE_FORMAT") == 0) {
        // Update the DSP parameter ModFormat
        Pixie_Devices[ModNum].DSP_Parameter_Values[ModFormat_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, ModFormat_Address[ModNum], MOD_WRITE, ModNum);
    } else if (strcmp(ModParName, "MAX_EVENTS") == 0) {
        // Update the DSP parameter MaxEvents
        Pixie_Devices[ModNum].DSP_Parameter_Values[MaxEvents_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, MaxEvents_Address[ModNum], MOD_WRITE, ModNum);
    } else if (strcmp(ModParName, "SYNCH_WAIT") == 0) {
        // Update the DSP parameter SynchWait
        Pixie_Devices[ModNum].DSP_Parameter_Values[SynchWait_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, SynchWait_Address[ModNum], MOD_WRITE, ModNum);

        // When changing SynchWait in one Pixie module, we need to broadcast it to all other modules as well
        Pixie_Broadcast("SYNCH_WAIT", ModNum);
    } else if (strcmp(ModParName, "IN_SYNCH") == 0) {
        // Update the DSP parameter InSynch
        Pixie_Devices[ModNum].DSP_Parameter_Values[InSynch_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, InSynch_Address[ModNum], MOD_WRITE, ModNum);

        // When changing InSynch in one Pixie module, we need to broadcast it to all other modules as well
        Pixie_Broadcast("IN_SYNCH", ModNum);
    } else if (strcmp(ModParName, "SLOW_FILTER_RANGE") == 0) {
        // Check slow filter range limit
        slowfilterrange = ModParData;

        if (slowfilterrange > SLOWFILTERRANGE_MAX) {
            slowfilterrange = SLOWFILTERRANGE_MAX;

            Pixie_Print_Warning(PIXIE_FUNC,
                                "Maximum SlowFilterRange that is currently supported is %d",
                                slowfilterrange);
        }
        if (slowfilterrange < SLOWFILTERRANGE_MIN) {
            slowfilterrange = SLOWFILTERRANGE_MIN;

            Pixie_Print_Warning(PIXIE_FUNC,
                                "Minimum SlowFilterRange that is currently supported is %d",
                                slowfilterrange);
        }

        // Update the DSP parameter SlowFilterRange
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                slowfilterrange;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&slowfilterrange, 1, SlowFilterRange_Address[ModNum], MOD_WRITE, ModNum);

        /*******************************************/
        //	We need to recompute FIFO settings
        /*******************************************/
        fastfilterrange =
                Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
        for (k = 0; k < NUMBER_OF_CHANNELS; k++) {
            // Get the current TraceDelay
            paflength = Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
            triggerdelay =
                    Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
            tracedelay = paflength - (unsigned int) ((double) triggerdelay / pow(2.0, (double) fastfilterrange));

            // Update FIFO settings (TriggerDelay and PAFLength)
            Pixie_ComputeFIFO(tracedelay, ModNum, k);
        }

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed after downloading SlowFilterRange, retval=%d",
                              retval);
            return (-3);
        }

        /*******************************************/
        //	We also need to update baseline cut value
        /*******************************************/
        for (k = 0; k < NUMBER_OF_CHANNELS; k++) {
            retval = Pixie16BLcutFinder(ModNum, k, &baselinecut);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "BLcutFinder failed in module %d after downloading SlowFilterRange, retval=%d",
                                  k, retval);
                return (-4);
            }
        }
    } else if (strcmp(ModParName, "FAST_FILTER_RANGE") == 0) {
        // Get the last fast filter range
        LastFastFilterRange =
                Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];

        // Check fast filter range limit
        fastfilterrange = ModParData;

        if (fastfilterrange > FASTFILTERRANGE_MAX) {
            fastfilterrange = FASTFILTERRANGE_MAX;

            Pixie_Print_Warning(PIXIE_FUNC,
                                "Maximum FastFilterRange that is currently supported is %d",
                                fastfilterrange);
        }
#if FASTFILTERRANGE_MIN > 0
        if (fastfilterrange < FASTFILTERRANGE_MIN) {
            fastfilterrange = FASTFILTERRANGE_MIN;

            Pixie_Print_Warning(PIXIE_FUNC,
                                "Minimum FastFilterRange that is currently supported is %d",
                                fastfilterrange);
        }
#endif

        // Update the new DSP parameter FastFilterRange
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                fastfilterrange;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&fastfilterrange, 1, FastFilterRange_Address[ModNum], MOD_WRITE, ModNum);

        /*******************************************/
        //	We need to recompute FIFO settings
        /*******************************************/
        for (k = 0; k < NUMBER_OF_CHANNELS; k++) {
            // Get the current TraceDelay
            paflength = Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
            triggerdelay =
                    Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
            tracedelay = paflength - (unsigned int) ((double) triggerdelay / pow(2.0, (double) LastFastFilterRange));

            // Update FIFO settings (TriggerDelay and PAFLength)
            Pixie_ComputeFIFO(tracedelay, ModNum, k);
        }

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed after downloading FastFilterRange, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "FastTrigBackplaneEna") == 0) {
        // Update the new DSP parameter FastTrigBackplaneEna
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackplaneEna_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, FastTrigBackplaneEna_Address[ModNum], MOD_WRITE, ModNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed after downloading FastTrigBackplaneEna, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "CrateID") == 0) {
        // Update the new DSP parameter CrateID
        Pixie_Devices[ModNum].DSP_Parameter_Values[CrateID_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, CrateID_Address[ModNum], MOD_WRITE, ModNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "ProgramFippi failed after downloading CrateID, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "SlotID") == 0) {
        // Update the new DSP parameter SlotID
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlotID_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, SlotID_Address[ModNum], MOD_WRITE, ModNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "ProgramFippi failed after downloading SlotID, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "ModID") == 0) {
        // Update the new DSP parameter ModID
        Pixie_Devices[ModNum].DSP_Parameter_Values[ModID_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, ModID_Address[ModNum], MOD_WRITE, ModNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "ProgramFippi failed after downloading ModID, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "TrigConfig0") == 0) {
        // Update the new DSP parameter TrigConfig0
        Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, TrigConfig_Address[ModNum], MOD_WRITE, ModNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed after downloading TrigConfig0, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "TrigConfig1") == 0) {
        // Update the new DSP parameter TrigConfig1
        Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 1 - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, TrigConfig_Address[ModNum] + 1, MOD_WRITE, ModNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed after downloading TrigConfig1, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "TrigConfig2") == 0) {
        // Update the new DSP parameter TrigConfig2
        Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 2 - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, TrigConfig_Address[ModNum] + 2, MOD_WRITE, ModNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed after downloading TrigConfig2, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "TrigConfig3") == 0) {
        // Update the new DSP parameter TrigConfig3
        Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 3 - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, TrigConfig_Address[ModNum] + 3, MOD_WRITE, ModNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed after downloading TrigConfig3, retval=%d",
                              retval);
            return (-3);
        }
    } else if (strcmp(ModParName, "HOST_RT_PRESET") == 0) {
        // Update the new DSP parameter HRTP
        Pixie_Devices[ModNum].DSP_Parameter_Values[HRTP_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ModParData, 1, HRTP_Address[ModNum], MOD_WRITE, ModNum);

        // When changing HostRunTimePreset in one Pixie module, we need to broadcast it to all other modules as well
        Pixie_Broadcast("HOST_RT_PRESET", ModNum);
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "invalid module parameter name %s", ModParName);
        return (-2);
    }

    return (0);
}


/**
 * @ingroup CONFIGURATION
 * @brief Read a module parameter from a Pixie module
 *
 * #### List of module parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | MODULE_CSRA | bit pattern | user set | MODCSRA |
 * | MODULE_CSRB | bit pattern | user set | MODCSRB |
 * | MODULE_FORMAT | None | auto DSP | MODFORMAT |
 * | MAX_EVENTS | None | user set/auto DSP | MAXEVENTS |
 * | SYNCH_WAIT | logic (0, 1) | user set | SYNCHWAIT |
 * | IN_SYNCH | logic (0, 1) | user/DSP set | INSYNCH |
 * | SLOW_FILTER_RANGE | None | user set | SLOWFILTERRANGE |
 * | FAST_FILTER_RANGE | None | user set | FASTFILTERRANGE |
 * | FastTrigBackplaneEna | bit pattern | user set | FASTTRIGBACKPLANEENA |
 * | CrateID | None | user set | CRATEID |
 * | SlotID | None | user set | SLOTID |
 * | ModID | None | user set | MODID |
 * | TrigConfig0 | None | user set | TRIGCONFIG[0] |
 * | TrigConfig1 | None | user set | TRIGCONFIG[1] |
 * | TrigConfig2 | None | user set | TRIGCONFIG[2] |
 * | TrigConfig3 | None | user set | TRIGCONFIG[3] |
 * | HOST_RT_PRESET | seconds | user set | HOSTRUNTIMEPRESET |
 *
 * @param[in] ModParName: The name of the module parameter that we'll read
 * @param[out] ModParData: Contains the value of the requested parameters.
 * @param[in] ModNum: The module number we'll write to. Numbering starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid module parameter name
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglModPar(const char* ModParName,  // the name of the module parameter
                                                          unsigned int* ModParData,  // the module parameter value to be read from the module
                                                          unsigned short ModNum)  // module number
{

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Read module parameter

    if (strcmp(ModParName, "MODULE_NUMBER") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, ModNum_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter ModNum
        Pixie_Devices[ModNum].DSP_Parameter_Values[ModNum_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "MODULE_CSRA") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, ModCSRA_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter ModCSRA
        Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRA_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "MODULE_CSRB") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, ModCSRB_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter ModCSRB
        Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "MODULE_FORMAT") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, ModFormat_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter ModFormat
        Pixie_Devices[ModNum].DSP_Parameter_Values[ModFormat_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "MAX_EVENTS") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, MaxEvents_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter MaxEvents
        Pixie_Devices[ModNum].DSP_Parameter_Values[MaxEvents_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "SYNCH_WAIT") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, SynchWait_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter SynchWait
        Pixie_Devices[ModNum].DSP_Parameter_Values[SynchWait_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "IN_SYNCH") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, InSynch_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter InSynch
        Pixie_Devices[ModNum].DSP_Parameter_Values[InSynch_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "SLOW_FILTER_RANGE") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, SlowFilterRange_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter SlowFilterRange
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "FAST_FILTER_RANGE") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter FastFilterRange
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "FastTrigBackplaneEna") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, FastTrigBackplaneEna_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter FastTrigBackplaneEna
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackplaneEna_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                *ModParData;
    } else if (strcmp(ModParName, "CrateID") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, CrateID_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter CrateID
        Pixie_Devices[ModNum].DSP_Parameter_Values[CrateID_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "SlotID") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, SlotID_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter SlotID
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlotID_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "ModID") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, ModID_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter ModID
        Pixie_Devices[ModNum].DSP_Parameter_Values[ModID_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "TrigConfig0") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, TrigConfig_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter TrigConfig0
        Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "TrigConfig1") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, TrigConfig_Address[ModNum] + 1, MOD_READ, ModNum);

        // Update module parameter TrigConfig1
        Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 1 - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "TrigConfig2") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, TrigConfig_Address[ModNum] + 2, MOD_READ, ModNum);

        // Update module parameter TrigConfig2
        Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 2 - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "TrigConfig3") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, TrigConfig_Address[ModNum] + 3, MOD_READ, ModNum);

        // Update module parameter TrigConfig3
        Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 3 - DATA_MEMORY_ADDRESS] = *ModParData;
    } else if (strcmp(ModParName, "HOST_RT_PRESET") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(ModParData, 1, HRTP_Address[ModNum], MOD_READ, ModNum);

        // Update module parameter HostRTPreset
        Pixie_Devices[ModNum].DSP_Parameter_Values[HRTP_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "invalid module parameter name %s", ModParName);
        return (-2);
    }
    return (0);
}


/**
 * @ingroup CONFIGURATION
 * @brief Write a channel parameter to a Pixie module.
 *
 * #### List of Channel Parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | TRIGGER_RISETIME | &mu;s | user set | FASTLENGTH |
 * | TRIGGER_FLATTOP | &mu;s | user set | FASTGAP |
 * | TRIGGER_THRESHOLD | ADC units | user set | FASTTHRESH |
 * | ENERGY_RISETIME | &mu;s | user set | SLOWLENGTH |
 * | ENERGY_FLATTOP | &mu;s | user set | SLOWGAP |
 * | TAU | &mu;s | user set | PREAMPTAU |
 * | TRACE_LENGTH | &mu;s | user set | TRACELENGTH |
 * | TRACE_DELAY | &mu;s | user set | TRIGGERDELAY, PAFLENGTH |
 * | VOFFSET | V | user set | OFFSETDAC |
 * | XDT | &mu;s | user set | XWAIT |
 * | BASELINE_PERCENT | % | user set | BASELINEPERCENT |
 * | EMIN | None | user set | ENERGYLOW |
 * | BINFACTOR | None | user set | LOG2EBIN |
 * | BASELINE_AVERAGE | None | user set | LOG2BWEIGHT |
 * | CHANNEL_CSRA | bit pattern | user set | CHANCSRA |
 * | CHANNEL_CSRB | bit pattern | user set | CHANCSRB |
 * | BLCUT | None | user set/auto API | BLCUT |
 * | INTEGRATOR | None | user set | INTEGRATOR |
 * | FASTTRIGBACKLEN | &mu;s | user set | FASTTRIGBACKLEN |
 * | CFDDelay | &mu;s | user set | CFDDELAY |
 * | CFDScale | None | user set | CFDSCALE |
 * | CFDThresh | ADC units | user set | CFDTHRESH |
 * | QDCLen0 | &mu;s | user set | QDCLEN0 |
 * | QDCLen1 | &mu;s | user set | QDCLEN1 |
 * | QDCLen2 | &mu;s | user set | QDCLEN2 |
 * | QDCLen3 | &mu;s | user set | QDCLEN3 |
 * | QDCLen4 | &mu;s | user set | QDCLEN4 |
 * | QDCLen5 | &mu;s | user set | QDCLEN5 |
 * | QDCLen6 | &mu;s | user set | QDCLEN6 |
 * | QDCLen7 | &mu;s | user set | QDCLEN7 |
 * | ExtTrigStretch | &mu;s | user set | EXTTRIGSTRETCH |
 * | VetoStretch | &mu;s | user set | VETOSTRETCH |
 * | MultiplicityMaskL | bit pattern | user set | MULTIPLICITYMASKL |
 * | MultiplicityMaskH | bit pattern | user set | MULTIPLICITYMASKH |
 * | ExternDelayLen | &mu;s | user set | EXTERNDELAYLEN |
 * | FtrigoutDelay | &mu;s | user set | FTRIGOUTDELAY |
 * | ChanTrigStretch | &mu;s | user set | CHANTRIGSTRETCH |
 *
 * @param[in] ChanParName: The name of the channel parameter that we'll write
 * @param[in] ChanParData: The value that we'll write to the provided channel
 * @param[in] ModNum: The module number we'll write to. Counting starts at 0.
 * @param[in] ChanNum: The channel number we'll write to. Counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Invalid channel parameter name
 * @retval -4 - Programing Fippi failed downloading channel parameter
 * @retval -5 - BLcutFinder failed downloading channel parameter
 * @retval -6 - SetDACs failed downloading channel parameter
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglChanPar(const char* ChanParName,
                                                            double ChanParData,
                                                            unsigned short ModNum,
                                                            unsigned short ChanNum)
{
    unsigned int FL, FG, SL, SG, FastFilterRange, SlowFilterRange, FifoLength;
    unsigned int fastthresh, peaksample, peaksep, preamptau, tracelength, tracedelay;
    unsigned int paflength, triggerdelay, offsetdac;
    unsigned int xwait, lastxwait;
    unsigned int baselinepercent, energylow, log2ebin, newchancsra, oldchancsra, chancsrb;
    unsigned int baselinecut, fasttrigbacklen, baselineaverage;
    int retval;
    unsigned int cfddelay, cfdscale, qdclen, exttrigstretch, vetostretch, externdelaylen, multiplicitymaskl,
            multiplicitymaskh, ftrigoutdelay;
    unsigned int chantrigstretch, cfdthresh, integrator;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }
    // Check if ChanNum is valid
    if (ChanNum >= NUMBER_OF_CHANNELS) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie channel number %d", ChanNum);
        return (-2);
    }

    // Write channel parameter

    if (strcmp(ChanParName, "TRIGGER_RISETIME") == 0) {

        // Calculate fast length
        FastFilterRange =
                Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            FL = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS /
                                      pow(2.0, (double) FastFilterRange));
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            FL = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2) /
                                      pow(2.0, (double) FastFilterRange));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            FL = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5) /
                                      pow(2.0, (double) FastFilterRange));

        // Check fast length limit
        FG = Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
        if ((FL + FG) > FASTFILTER_MAX_LEN) {
            FL = FASTFILTER_MAX_LEN - FG;
        }
        if (FL < MIN_FASTLENGTH_LEN) {
            FL = MIN_FASTLENGTH_LEN;
            if ((FL + FG) > FASTFILTER_MAX_LEN) {
                FG = FASTFILTER_MAX_LEN - MIN_FASTLENGTH_LEN;
            }
        }

        // Update DSP parameter FastLength
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FL;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&FL, 1, (unsigned int) (FastLength_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Update DSP parameter FastGap
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FG;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&FG, 1, (unsigned int) (FastGap_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply FastLength settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading TriggerRiseTime, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "TRIGGER_FLATTOP") == 0) {

        // Calculate fast gap
        FastFilterRange =
                Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            FG = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS /
                                      pow(2.0, (double) FastFilterRange));
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            FG = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2) /
                                      pow(2.0, (double) FastFilterRange));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            FG = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5) /
                                      pow(2.0, (double) FastFilterRange));

        // Check fast gap limit
        FL = Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
        if ((FL + FG) > FASTFILTER_MAX_LEN) {
            FG = FASTFILTER_MAX_LEN - FL;
        }

        // Update DSP parameter FastGap
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FG;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&FG, 1, (unsigned int) (FastGap_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply FastGap settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading TriggerFlatTop, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "TRIGGER_THRESHOLD") == 0) {

        // Calculate FastThresh
        FL = Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            fastthresh = (unsigned int) (ChanParData * (double) FL);
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            fastthresh = (unsigned int) (ChanParData * (double) FL * 2.0);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            fastthresh = (unsigned int) (ChanParData * (double) FL * 5.0);

        // Check FastThresh limit
        if (fastthresh >= FAST_THRESHOLD_MAX) {
            fastthresh =
                    (unsigned int) (((double) FAST_THRESHOLD_MAX / (double) FL - 0.5) * (double) FL);  // in ADC counts
        }

        // Update DSP parameter FastThresh
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                fastthresh;

        // Download to the selected Pixie module
        Pixie16IMbufferIO(&fastthresh, 1, (unsigned int) (FastThresh_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply FastThresh settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading TriggerThreshold, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if ((strcmp(ChanParName, "ENERGY_RISETIME") == 0) || (strcmp(ChanParName, "ENERGY_FLATTOP") == 0)) {
        // Get the current TraceDelay
        FastFilterRange =
                Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
        paflength =
                Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
        triggerdelay = Pixie_Devices[ModNum]
                               .DSP_Parameter_Values[TriggerDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
        tracedelay = paflength - (unsigned int) ((double) triggerdelay / pow(2.0, (double) FastFilterRange));

        // Get the current SlowFilterRange
        SlowFilterRange =
                Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];

        if (strcmp(ChanParName, "ENERGY_RISETIME") == 0) {

            // Calculate slow length
            if (Module_Information[ModNum].Module_ADCMSPS == 100)
                SL = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS /
                                          pow(2.0, (double) SlowFilterRange));
            else if (Module_Information[ModNum].Module_ADCMSPS == 250)
                SL = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2) /
                                          pow(2.0, (double) SlowFilterRange));
            else if (Module_Information[ModNum].Module_ADCMSPS == 500)
                SL = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5) /
                                          pow(2.0, (double) SlowFilterRange));

            // Check slow length limit
            SG = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
            if ((SL + SG) > SLOWFILTER_MAX_LEN) {
                SL = SLOWFILTER_MAX_LEN - SG;
            }
            if (SL < MIN_SLOWLENGTH_LEN) {
                SL = MIN_SLOWLENGTH_LEN;
                if ((SL + SG) > SLOWFILTER_MAX_LEN) {
                    SG = SLOWFILTER_MAX_LEN - MIN_SLOWLENGTH_LEN;
                }
            }
        } else if (strcmp(ChanParName, "ENERGY_FLATTOP") == 0) {

            // Calculate slow gap
            if (Module_Information[ModNum].Module_ADCMSPS == 100)
                SG = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS /
                                          pow(2.0, (double) SlowFilterRange));
            else if (Module_Information[ModNum].Module_ADCMSPS == 250)
                SG = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2) /
                                          pow(2.0, (double) SlowFilterRange));
            else if (Module_Information[ModNum].Module_ADCMSPS == 500)
                SG = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5) /
                                          pow(2.0, (double) SlowFilterRange));

            // Check slow gap limit
            SL = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
            if ((SL + SG) > SLOWFILTER_MAX_LEN) {
                SG = SLOWFILTER_MAX_LEN - SL;
            }
            if (SG < MIN_SLOWGAP_LEN) {
                SG = MIN_SLOWGAP_LEN;
                if ((SL + SG) > SLOWFILTER_MAX_LEN) {
                    SL = SLOWFILTER_MAX_LEN - MIN_SLOWGAP_LEN;
                }
            }
        }

        // Update DSP parameter SlowLength
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlowLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SL;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&SL, 1, (unsigned int) (SlowLength_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Update DSP parameter SlowGap
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlowGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SG;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&SG, 1, (unsigned int) (SlowGap_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Re-calculate PeakSample and PeakSep
        switch (SlowFilterRange) {
            case 1:
                peaksample = SL + SG - 3;
                break;
            case 2:
                peaksample = SL + SG - 2;
                break;
            case 3:
                peaksample = SL + SG - 2;
                break;
            case 4:
                peaksample = SL + SG - 1;
                break;
            case 5:
                peaksample = SL + SG;
                break;
            case 6:
                peaksample = SL + SG + 1;
                break;
            default:
                peaksample = SL + SG - 2;
                break;
        }

        peaksep = SL + SG;

        // Update DSP parameter PeakSample
        Pixie_Devices[ModNum].DSP_Parameter_Values[PeakSample_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                peaksample;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&peaksample, 1, (unsigned int) (PeakSample_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Update DSP parameter PeakSep
        Pixie_Devices[ModNum].DSP_Parameter_Values[PeakSep_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = peaksep;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&peaksep, 1, (unsigned int) (PeakSep_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Update FIFO settings (TriggerDelay and PAFLength)
        Pixie_ComputeFIFO(tracedelay, ModNum, ChanNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading EnergyRiseTime or EnergyFlatTop, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }

        // Update baseline cut value
        retval = Pixie16BLcutFinder(ModNum, ChanNum, &baselinecut);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "BLcutFinder failed in module %d channel %d after downloading EnergyRiseTime or EnergyFlatTop, retval=%d",
                              ModNum, ChanNum, retval);
            return (-5);
        }
    } else if (strcmp(ChanParName, "TAU") == 0) {

        // Calculate PreampTau
        preamptau = Decimal2IEEEFloating(ChanParData);

        // Update DSP parameter PreampTau
        Pixie_Devices[ModNum].DSP_Parameter_Values[PreampTau_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                preamptau;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&preamptau, 1, (unsigned int) (PreampTau_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Use Program_FiPPI to recompute the coefficients
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading Tau, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }

        // Update baseline cut value
        retval = Pixie16BLcutFinder(ModNum, ChanNum, &baselinecut);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "BLcutFinder failed in module %d channel %d after downloading Tau, retval=%d",
                              ModNum, ChanNum, retval);
            return (-5);
        }
    } else if (strcmp(ChanParName, "TRACE_LENGTH") == 0) {

        // Get the current FastFilterRange
        FastFilterRange =
                Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];

        // Compute the requested TraceLength
        tracelength = (unsigned int) (ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS /
                                      pow(2.0, (double) FastFilterRange));

        if (Module_Information[ModNum].Module_ADCMSPS == 500) {
            // Ensure TraceLength is multiple of 10 to fit 10-to-5*2 packing in the FPGA
            tracelength = ((unsigned int) (tracelength / 10)) * 10;
            if (tracelength < TRACELEN_MIN_500MHZADC) {
                // Enforce a minimum value
                tracelength = TRACELEN_MIN_500MHZADC;
            }
        } else if ((Module_Information[ModNum].Module_ADCMSPS == 250) ||
                   (Module_Information[ModNum].Module_ADCMSPS == 100)) {
            // Ensure TraceLength is an even number to fit 2-to-1 packing in the FPGA
            tracelength = APP32_ClrBit(0, tracelength);
            if (tracelength < TRACELEN_MIN_250OR100MHZADC) {
                // Enforce a minimum value
                tracelength = TRACELEN_MIN_250OR100MHZADC;
            }
        }

        // Check if TraceLength exceeds FifoLength
        FifoLength = Pixie_Devices[ModNum].DSP_Parameter_Values[FIFOLength_Address[ModNum] - DATA_MEMORY_ADDRESS];
        if (tracelength > FifoLength) {
            tracelength = FifoLength;
        }

        // Update DSP parameter TraceLength
        Pixie_Devices[ModNum].DSP_Parameter_Values[TraceLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                tracelength;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&tracelength, 1, (unsigned int) (TraceLength_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply TraceLength settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading TraceLength, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "TRACE_DELAY") == 0) {

        // Get the current FastFilterRange
        FastFilterRange =
                Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];

        // Check if TraceDelay exceeds TraceLength
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            tracedelay = (unsigned int) (ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS /
                                         pow(2.0, (double) FastFilterRange));
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            tracedelay = (unsigned int) (ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2) /
                                         pow(2.0, (double) FastFilterRange));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            tracedelay = (unsigned int) (ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5) /
                                         pow(2.0, (double) FastFilterRange));
        else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d, no valid tracedelay",
                              ModNum, ChanNum);
            return (-4);
        }

        tracelength =
                Pixie_Devices[ModNum].DSP_Parameter_Values[TraceLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
        if (tracedelay > tracelength) {
            tracedelay = (unsigned int) ((double) tracelength / 2.0);
        }

        // Check if TraceDelay exceeds TRACEDELAY_MAX
        if (tracedelay > TRACEDELAY_MAX) {
            tracedelay = TRACEDELAY_MAX;
        }

        // Update FIFO settings (TriggerDelay and PAFLength)
        Pixie_ComputeFIFO(tracedelay, ModNum, ChanNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading TraceDelay, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "VOFFSET") == 0) {

        offsetdac = (unsigned int) (65536.0 * (ChanParData / DAC_VOLTAGE_RANGE + 1.0 / 2.0));

        // Check limit
        if (offsetdac > 65535) {
            offsetdac = 65535;  // a 16-bit DAC
        }

        // Update DSP parameter OffsetDAC
        Pixie_Devices[ModNum].DSP_Parameter_Values[OffsetDAC_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                offsetdac;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&offsetdac, 1, (unsigned int) (OffsetDAC_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Set DACs to apply the new DAC settings
        retval = Pixie16SetDACs(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "SetDACs failed in module %d channel %d after downloading OffsetDACs, retval=%d",
                              ModNum, ChanNum, retval);
            return (-6);
        }
    } else if (strcmp(ChanParName, "XDT") == 0) {
        // Get the last Xwait
        lastxwait = Pixie_Devices[ModNum].DSP_Parameter_Values[Xwait_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
        // Compute the new Xwait
        xwait = (unsigned int) ROUND(ChanParData * (double) DSP_CLOCK_MHZ);
        if ((Module_Information[ModNum].Module_ADCMSPS == 100) || (Module_Information[ModNum].Module_ADCMSPS == 500)) {
            // For 100 MSPS RevB/C/D or RevF, or 500 MSPS RevF, xwait should be multiples of 6

            if (xwait < 6)  // xwait should be at least 6
            {
                xwait = 6;
            }

            if (xwait > lastxwait)  // increase Xwait
            {
                xwait = (unsigned int) (ceil((double) xwait / 6.0) * 6.0);
            } else  // decrease Xwait
            {
                xwait = (unsigned int) (floor((double) xwait / 6.0) * 6.0);
            }
        } else {
            // For 250 RevF, xwait should be multiples of 8

            if (xwait < 8)  // xwait should be at least 8
            {
                xwait = 8;
            }

            if (xwait > lastxwait)  // increase Xwait
            {
                xwait = (unsigned int) (ceil((double) xwait / 8.0) * 8.0);
            } else  // decrease Xwait
            {
                xwait = (unsigned int) (floor((double) xwait / 8.0) * 8.0);
            }
        }

        // Update DSP parameter Xwait
        Pixie_Devices[ModNum].DSP_Parameter_Values[Xwait_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = xwait;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&xwait, 1, (unsigned int) (Xwait_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

    } else if (strcmp(ChanParName, "BASELINE_PERCENT") == 0) {

        // Get the new BaselinePercent
        baselinepercent = (unsigned int) ChanParData;

        // Check limit
        if (baselinepercent < 1) {
            baselinepercent = 1;
        } else if (baselinepercent > 99) {
            baselinepercent = 99;
        }

        // Update DSP parameter BaselinePercent
        Pixie_Devices[ModNum].DSP_Parameter_Values[BaselinePercent_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                baselinepercent;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&baselinepercent, 1, (unsigned int) (BaselinePercent_Address[ModNum] + ChanNum), MOD_WRITE,
                          ModNum);

    } else if (strcmp(ChanParName, "EMIN") == 0) {

        // Get the new EnergyLow
        energylow = (unsigned int) ChanParData;

        // Update DSP parameter EnergyLow
        Pixie_Devices[ModNum].DSP_Parameter_Values[EnergyLow_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                energylow;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&energylow, 1, (unsigned int) (EnergyLow_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

    } else if (strcmp(ChanParName, "BINFACTOR") == 0) {

        // Get the new Log2Ebin
        log2ebin = (unsigned int) ChanParData;

        // Check limit
        if (log2ebin < 1) {
            log2ebin = 1;
        }
        if (log2ebin > 6) {
            log2ebin = 6;
        }

        // Convert to a signed negative number
        log2ebin = (unsigned int) (pow(2.0, 32.0) - (double) log2ebin);

        // Update DSP parameter Log2Ebin
        Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Ebin_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = log2ebin;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&log2ebin, 1, (unsigned int) (Log2Ebin_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

    } else if (strcmp(ChanParName, "BASELINE_AVERAGE") == 0) {

        // Get the new baselineaverage
        baselineaverage = (unsigned int) ChanParData;

        // Check limit
        if (baselineaverage > 16) {
            baselineaverage = 16;
        }

        // Convert to a signed negative number
        if (baselineaverage > 0) {
            baselineaverage = (unsigned int) (pow(2.0, 32.0) - (double) baselineaverage);
        }

        // Update DSP parameter Log2Bweight
        Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                baselineaverage;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&baselineaverage, 1, (unsigned int) (Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE,
                          ModNum);

    } else if (strcmp(ChanParName, "CHANNEL_CSRA") == 0) {

        // Get the new ChanCSRa
        newchancsra = (unsigned int) ChanParData;
        oldchancsra =
                Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRa_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];

        // Update DSP parameter ChanCSRa
        Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRa_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                newchancsra;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&newchancsra, 1, (unsigned int) (ChanCSRa_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading ChanCSRA, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }

        // Set DACs to apply the new DAC settings
        retval = Pixie16SetDACs(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "SetDACs failed in module %d channel %d after downloading ChanCSRA, retval=%d",
                              ModNum, ChanNum, retval);
            return (-6);
        }

        // Check if we need to update baseline cut value (only needed if Vgain changed)
        if (APP32_TstBit(CCSRA_ENARELAY, newchancsra) != APP32_TstBit(CCSRA_ENARELAY, oldchancsra)) {
            retval = Pixie16BLcutFinder(ModNum, ChanNum, &baselinecut);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "BLcutFinder failed in module %d channel %d after downloading ChanCSRA, retval=%d",
                                  ModNum, ChanNum, retval);
                return (-5);
            }
        }
    } else if (strcmp(ChanParName, "CHANNEL_CSRB") == 0) {

        // Get the new ChanCSRb
        chancsrb = (unsigned int) ChanParData;

        // Update DSP parameter ChanCSRb
        Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRb_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chancsrb;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&chancsrb, 1, (unsigned int) (ChanCSRb_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

    } else if (strcmp(ChanParName, "BLCUT") == 0) {

        // Get the new BLcut
        baselinecut = (unsigned int) ChanParData;

        // Update DSP parameter BLcut
        Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselinecut;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&baselinecut, 1, (unsigned int) (BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

    } else if (strcmp(ChanParName, "INTEGRATOR") == 0) {

        // Get the new INTEGRATOR
        integrator = (unsigned int) ChanParData;

        // Check limit
        if (integrator > 7) {
            integrator = 7;
        }

        // Update DSP parameter INTEGRATOR
        Pixie_Devices[ModNum].DSP_Parameter_Values[Integrator_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                integrator;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&integrator, 1, (unsigned int) (Integrator_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply FastTrigBackLen settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading Integrator, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "FASTTRIGBACKLEN") == 0) {

        // Get the new FastTrigBackLen
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            fasttrigbacklen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            fasttrigbacklen =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            fasttrigbacklen =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));

        // Range check for FastTrigBackLen
        if ((Module_Information[ModNum].Module_ADCMSPS == 100) || (Module_Information[ModNum].Module_ADCMSPS == 500)) {
            if (fasttrigbacklen < FASTTRIGBACKLEN_MIN_100MHZFIPCLK) {
                fasttrigbacklen = FASTTRIGBACKLEN_MIN_100MHZFIPCLK;
            }
        } else if (Module_Information[ModNum].Module_ADCMSPS == 250) {
            if (fasttrigbacklen < FASTTRIGBACKLEN_MIN_125MHZFIPCLK) {
                fasttrigbacklen = FASTTRIGBACKLEN_MIN_125MHZFIPCLK;
            }
        }

        if (fasttrigbacklen > FASTTRIGBACKLEN_MAX) {
            fasttrigbacklen = FASTTRIGBACKLEN_MAX;
        }

        // Update DSP parameter FastTrigBackLen
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                fasttrigbacklen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&fasttrigbacklen, 1, (unsigned int) (FastTrigBackLen_Address[ModNum] + ChanNum), MOD_WRITE,
                          ModNum);

        // Program FiPPI to apply FastTrigBackLen settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading FastTrigBackLen, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "CFDDelay") == 0) {
        // Get the new CFDDelay
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            cfddelay = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            cfddelay = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            cfddelay = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));

        // Range check for CFDDelay
        if (cfddelay < CFDDELAY_MIN) {
            cfddelay = CFDDELAY_MIN;
        }
        if (cfddelay > CFDDELAY_MAX) {
            cfddelay = CFDDELAY_MAX;
        }

        // Update DSP parameter CFDDelay
        Pixie_Devices[ModNum].DSP_Parameter_Values[CFDDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfddelay;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&cfddelay, 1, (unsigned int) (CFDDelay_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply CFDDelay settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading CFDDelay, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "CFDScale") == 0) {
        // Get the new CFDScale
        cfdscale = (unsigned int) ChanParData;

        // Range check for the value of CFDScale
        if (cfdscale > CFDSCALE_MAX) {
            cfdscale = CFDSCALE_MAX;
        }

        // Update DSP parameter CFDScale
        Pixie_Devices[ModNum].DSP_Parameter_Values[CFDScale_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfdscale;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&cfdscale, 1, (unsigned int) (CFDScale_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply CFDScale settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading CFDScale, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "CFDThresh") == 0) {
        // Get the new CFDThresh
        cfdthresh = (unsigned int) ChanParData;

        // Range check for the value of CFDThresh
        if ((cfdthresh >= CFDTHRESH_MIN) && (cfdthresh <= CFDTHRESH_MAX)) {
            // Update DSP parameter CFDThresh
            Pixie_Devices[ModNum].DSP_Parameter_Values[CFDThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                    cfdthresh;
            // Download to the selected Pixie module
            Pixie16IMbufferIO(&cfdthresh, 1, (unsigned int) (CFDThresh_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

            // Program FiPPI to apply CFDThresh settings to the FPGA
            retval = Pixie16ProgramFippi(ModNum);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "ProgramFippi failed in module %d channel %d after downloading CFDThresh, retval=%d",
                                  ModNum, ChanNum, retval);
                return (-4);
            }
        } else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Value of CFDThresh being set was out of its valid range");
        }
    } else if (strcmp(ChanParName, "QDCLen0") == 0) {
        // Get the new QDCLen0
        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            qdclen = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));
        else
            qdclen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);

        // Range check for QDC length
        if (qdclen < QDCLEN_MIN) {
            qdclen = QDCLEN_MIN;
        }
        if (qdclen > QDCLEN_MAX) {
            qdclen = QDCLEN_MAX;
        }

        // Update DSP parameter QDCLen0
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen0_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen0_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply QDCLen0 settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading QDCLen0, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "QDCLen1") == 0) {
        // Get the new QDCLen1
        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            qdclen = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));
        else
            qdclen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);

        // Range check for QDC length
        if (qdclen < QDCLEN_MIN) {
            qdclen = QDCLEN_MIN;
        }
        if (qdclen > QDCLEN_MAX) {
            qdclen = QDCLEN_MAX;
        }

        // Update DSP parameter QDCLen1
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen1_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen1_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply QDCLen1 settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading QDCLen1, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "QDCLen2") == 0) {
        // Get the new QDCLen2
        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            qdclen = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));
        else
            qdclen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);

        // Range check for QDC length
        if (qdclen < QDCLEN_MIN) {
            qdclen = QDCLEN_MIN;
        }
        if (qdclen > QDCLEN_MAX) {
            qdclen = QDCLEN_MAX;
        }

        // Update DSP parameter QDCLen2
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen2_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen2_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply QDCLen2 settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading QDCLen2, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "QDCLen3") == 0) {
        // Get the new QDCLen3
        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            qdclen = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));
        else
            qdclen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);

        // Range check for QDC length
        if (qdclen < QDCLEN_MIN) {
            qdclen = QDCLEN_MIN;
        }
        if (qdclen > QDCLEN_MAX) {
            qdclen = QDCLEN_MAX;
        }

        // Update DSP parameter QDCLen3
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen3_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen3_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply QDCLen3 settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading QDCLen3, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "QDCLen4") == 0) {
        // Get the new QDCLen4
        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            qdclen = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));
        else
            qdclen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);

        // Range check for QDC length
        if (qdclen < QDCLEN_MIN) {
            qdclen = QDCLEN_MIN;
        }
        if (qdclen > QDCLEN_MAX) {
            qdclen = QDCLEN_MAX;
        }

        // Update DSP parameter QDCLen4
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen4_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen4_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply QDCLen4 settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading QDCLen4, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "QDCLen5") == 0) {
        // Get the new QDCLen5
        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            qdclen = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));
        else
            qdclen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);

        // Range check for QDC length
        if (qdclen < QDCLEN_MIN) {
            qdclen = QDCLEN_MIN;
        }
        if (qdclen > QDCLEN_MAX) {
            qdclen = QDCLEN_MAX;
        }

        // Update DSP parameter QDCLen5
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen5_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen5_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply QDCLen5 settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading QDCLen5, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "QDCLen6") == 0) {
        // Get the new QDCLen6
        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            qdclen = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));
        else
            qdclen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);

        // Range check for QDC length
        if (qdclen < QDCLEN_MIN) {
            qdclen = QDCLEN_MIN;
        }
        if (qdclen > QDCLEN_MAX) {
            qdclen = QDCLEN_MAX;
        }

        // Update DSP parameter QDCLen6
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen6_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen6_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply QDCLen6 settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading QDCLen6, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "QDCLen7") == 0) {
        // Get the new QDCLen7
        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            qdclen = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));
        else
            qdclen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);

        // Range check for QDC length
        if (qdclen < QDCLEN_MIN) {
            qdclen = QDCLEN_MIN;
        }
        if (qdclen > QDCLEN_MAX) {
            qdclen = QDCLEN_MAX;
        }

        // Update DSP parameter QDCLen7
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen7_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen7_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply QDCLen7 settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading QDCLen7, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "ExtTrigStretch") == 0) {
        // Get the new ExtTrigStretch
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            exttrigstretch = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            exttrigstretch =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            exttrigstretch =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));

        // Range check for ExtTrigStretch
        if (exttrigstretch < EXTTRIGSTRETCH_MIN) {
            exttrigstretch = EXTTRIGSTRETCH_MIN;
        }
        if (exttrigstretch > EXTTRIGSTRETCH_MAX) {
            exttrigstretch = EXTTRIGSTRETCH_MAX;
        }

        // Update DSP parameter ExtTrigStretch
        Pixie_Devices[ModNum].DSP_Parameter_Values[ExtTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                exttrigstretch;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&exttrigstretch, 1, (unsigned int) (ExtTrigStretch_Address[ModNum] + ChanNum), MOD_WRITE,
                          ModNum);

        // Program FiPPI to apply ExtTrigStretch settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading ExtTrigStretch, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "VetoStretch") == 0) {
        // Get the new VetoStretch
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            vetostretch = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            vetostretch = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            vetostretch = (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));

        // Range check for VetoStretch
        if (vetostretch < VETOSTRETCH_MIN) {
            vetostretch = VETOSTRETCH_MIN;
        }
        if (vetostretch > VETOSTRETCH_MAX) {
            vetostretch = VETOSTRETCH_MAX;
        }

        // Update DSP parameter VetoStretch
        Pixie_Devices[ModNum].DSP_Parameter_Values[VetoStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                vetostretch;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&vetostretch, 1, (unsigned int) (VetoStretch_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

        // Program FiPPI to apply VetoStretch settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading VetoStretch, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "MultiplicityMaskL") == 0) {
        // Get the new MultiplicityMaskL
        multiplicitymaskl = (unsigned int) ChanParData;

        // Update DSP parameter MultiplicityMaskL
        Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskL_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                multiplicitymaskl;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&multiplicitymaskl, 1, (unsigned int) (MultiplicityMaskL_Address[ModNum] + ChanNum),
                          MOD_WRITE, ModNum);

        // Program FiPPI to apply MultiplicityMaskL settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading MultiplicityMaskL, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "MultiplicityMaskH") == 0) {
        // Get the new MultiplicityMaskH
        multiplicitymaskh = (unsigned int) ChanParData;

        // Update DSP parameter MultiplicityMaskH
        Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskH_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                multiplicitymaskh;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&multiplicitymaskh, 1, (unsigned int) (MultiplicityMaskH_Address[ModNum] + ChanNum),
                          MOD_WRITE, ModNum);

        // Program FiPPI to apply MultiplicityMaskH settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading MultiplicityMaskH, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "ExternDelayLen") == 0) {
        // Get the new ExternDelayLen
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            externdelaylen = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            externdelaylen =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            externdelaylen =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));

        // Range check for ExternDelayLen
#if EXTDELAYLEN_MIN > 0
        if (externdelaylen < EXTDELAYLEN_MIN) {
            externdelaylen = EXTDELAYLEN_MIN;
        }
#endif

        if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC) ||
            (Module_Information[ModNum].Module_Rev == 0xD)) {
            if (externdelaylen > EXTDELAYLEN_MAX_REVBCD) {
                externdelaylen = EXTDELAYLEN_MAX_REVBCD;
            }

        } else if (Module_Information[ModNum].Module_Rev == 0xF) {
            if (externdelaylen > EXTDELAYLEN_MAX_REVF) {
                externdelaylen = EXTDELAYLEN_MAX_REVF;
            }
        }

        // Update DSP parameter ExternDelayLen
        Pixie_Devices[ModNum].DSP_Parameter_Values[ExternDelayLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                externdelaylen;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&externdelaylen, 1, (unsigned int) (ExternDelayLen_Address[ModNum] + ChanNum), MOD_WRITE,
                          ModNum);

        // Program FiPPI to apply ExternDelayLen settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading ExternDelayLen, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "FtrigoutDelay") == 0) {
        // Get the new FtrigoutDelay
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            ftrigoutdelay = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            ftrigoutdelay =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            ftrigoutdelay =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));

        // Range check for FtrigoutDelay
#if FASTTRIGBACKDELAY_MIN > 0
        if (ftrigoutdelay < FASTTRIGBACKDELAY_MIN) {
            ftrigoutdelay = FASTTRIGBACKDELAY_MIN;
        }
#endif

        if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC) ||
            (Module_Information[ModNum].Module_Rev == 0xD)) {
            if (ftrigoutdelay > FASTTRIGBACKDELAY_MAX_REVBCD) {
                ftrigoutdelay = FASTTRIGBACKDELAY_MAX_REVBCD;
            }
        } else if (Module_Information[ModNum].Module_Rev == 0xF) {
            if (ftrigoutdelay > FASTTRIGBACKDELAY_MAX_REVF) {
                ftrigoutdelay = FASTTRIGBACKDELAY_MAX_REVF;
            }
        }

        // Update DSP parameter FtrigoutDelay
        Pixie_Devices[ModNum].DSP_Parameter_Values[FtrigoutDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                ftrigoutdelay;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&ftrigoutdelay, 1, (unsigned int) (FtrigoutDelay_Address[ModNum] + ChanNum), MOD_WRITE,
                          ModNum);

        // Program FiPPI to apply FtrigoutDelay settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading FtrigoutDelay, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else if (strcmp(ChanParName, "ChanTrigStretch") == 0) {
        // Get the new ChanTrigStretch
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            chantrigstretch = (unsigned int) ROUND(ChanParData * (double) Module_Information[ModNum].Module_ADCMSPS);
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            chantrigstretch =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 2));
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            chantrigstretch =
                    (unsigned int) ROUND(ChanParData * (double) (Module_Information[ModNum].Module_ADCMSPS / 5));

        // Range check for ChanTrigStretch
        if (chantrigstretch < CHANTRIGSTRETCH_MIN) {
            chantrigstretch = CHANTRIGSTRETCH_MIN;
        }
        if (chantrigstretch > CHANTRIGSTRETCH_MAX) {
            chantrigstretch = CHANTRIGSTRETCH_MAX;
        }

        // Update DSP parameter ChanTrigStretch
        Pixie_Devices[ModNum].DSP_Parameter_Values[ChanTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                chantrigstretch;
        // Download to the selected Pixie module
        Pixie16IMbufferIO(&chantrigstretch, 1, (unsigned int) (ChanTrigStretch_Address[ModNum] + ChanNum), MOD_WRITE,
                          ModNum);

        // Program FiPPI to apply ChanTrigStretch settings to the FPGA
        retval = Pixie16ProgramFippi(ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "ProgramFippi failed in module %d channel %d after downloading ChanTrigStretch, retval=%d",
                              ModNum, ChanNum, retval);
            return (-4);
        }
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "invalid channel parameter name %s", ChanParName);
        return (-3);
    }

    return (0);
}


/**
 * @ingroup CONFIGURATION
 * @brief Read a channel parameter from a module.
 *
 * #### List of Channel Parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | TRIGGER_RISETIME | &mu;s | user set | FASTLENGTH |
 * | TRIGGER_FLATTOP | &mu;s | user set | FASTGAP |
 * | TRIGGER_THRESHOLD | ADC units | user set | FASTTHRESH |
 * | ENERGY_RISETIME | &mu;s | user set | SLOWLENGTH |
 * | ENERGY_FLATTOP | &mu;s | user set | SLOWGAP |
 * | TAU | &mu;s | user set | PREAMPTAU |
 * | TRACE_LENGTH | &mu;s | user set | TRACELENGTH |
 * | TRACE_DELAY | &mu;s | user set | TRIGGERDELAY, PAFLENGTH |
 * | VOFFSET | V | user set | OFFSETDAC |
 * | XDT | &mu;s | user set | XWAIT |
 * | BASELINE_PERCENT | % | user set | BASELINEPERCENT |
 * | EMIN | None | user set | ENERGYLOW |
 * | BINFACTOR | None | user set | LOG2EBIN |
 * | BASELINE_AVERAGE | None | user set | LOG2BWEIGHT |
 * | CHANNEL_CSRA | bit pattern | user set | CHANCSRA |
 * | CHANNEL_CSRB | bit pattern | user set | CHANCSRB |
 * | BLCUT | None | user set/auto API | BLCUT |
 * | INTEGRATOR | None | user set | INTEGRATOR |
 * | FASTTRIGBACKLEN | &mu;s | user set | FASTTRIGBACKLEN |
 * | CFDDelay | &mu;s | user set | CFDDELAY |
 * | CFDScale | None | user set | CFDSCALE |
 * | CFDThresh | ADC units | user set | CFDTHRESH |
 * | QDCLen0 | &mu;s | user set | QDCLEN0 |
 * | QDCLen1 | &mu;s | user set | QDCLEN1 |
 * | QDCLen2 | &mu;s | user set | QDCLEN2 |
 * | QDCLen3 | &mu;s | user set | QDCLEN3 |
 * | QDCLen4 | &mu;s | user set | QDCLEN4 |
 * | QDCLen5 | &mu;s | user set | QDCLEN5 |
 * | QDCLen6 | &mu;s | user set | QDCLEN6 |
 * | QDCLen7 | &mu;s | user set | QDCLEN7 |
 * | ExtTrigStretch | &mu;s | user set | EXTTRIGSTRETCH |
 * | VetoStretch | &mu;s | user set | VETOSTRETCH |
 * | MultiplicityMaskL | bit pattern | user set | MULTIPLICITYMASKL |
 * | MultiplicityMaskH | bit pattern | user set | MULTIPLICITYMASKH |
 * | ExternDelayLen | &mu;s | user set | EXTERNDELAYLEN |
 * | FtrigoutDelay | &mu;s | user set | FTRIGOUTDELAY |
 * | ChanTrigStretch | &mu;s | user set | CHANTRIGSTRETCH |
 *
 * @param[in] ChanParName: The name of the channel parameter that we'll read
 * @param[out] ChanParData: Contains the value we read from the channel.
 * @param[in] ModNum: The module number we'll read from. Counting starts at 0.
 * @param[in] ChanNum: The channel number we'll read from. Counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Invalid channel parameter name
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanPar(const char* ChanParName,
                                                           double* ChanParData,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum)
{

    unsigned int FL, FG, SL, SG, FastFilterRange, SlowFilterRange;
    unsigned int fastthresh, preamptau, tracelength;
    unsigned int paflength, triggerdelay, offsetdac;
    unsigned int xwait;
    unsigned int baselinepercent, energylow, log2ebin, chancsra, chancsrb;
    unsigned int baselinecut, fasttrigbacklen, baselineaverage;
    unsigned int cfddelay, cfdscale, qdclen, exttrigstretch, vetostretch, externdelaylen, multiplicitymaskl,
            multiplicitymaskh, ftrigoutdelay;
    unsigned int chantrigstretch, cfdthresh, integrator;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }
    // Check if ChanNum is valid
    if (ChanNum >= NUMBER_OF_CHANNELS) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie channel number %d", ChanNum);
        return (-2);
    }

    // Read channel parameter

    if (strcmp(ChanParName, "TRIGGER_RISETIME") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&FL, 1, (unsigned int) (FastLength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FL;

        Pixie16IMbufferIO(&FastFilterRange, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                FastFilterRange;

        // Update channel parameter TriggerRiseTime
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) FL * pow(2.0, (double) FastFilterRange) /
                           (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) FL * pow(2.0, (double) FastFilterRange) /
                           (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) FL * pow(2.0, (double) FastFilterRange) /
                           (double) (Module_Information[ModNum].Module_ADCMSPS / 5);

    } else if (strcmp(ChanParName, "TRIGGER_FLATTOP") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&FG, 1, (unsigned int) (FastGap_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FG;

        Pixie16IMbufferIO(&FastFilterRange, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                FastFilterRange;

        // Update channel parameter TriggerFlatTop
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) FG * pow(2.0, (double) FastFilterRange) /
                           (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) FG * pow(2.0, (double) FastFilterRange) /
                           (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) FG * pow(2.0, (double) FastFilterRange) /
                           (double) (Module_Information[ModNum].Module_ADCMSPS / 5);

    } else if (strcmp(ChanParName, "TRIGGER_THRESHOLD") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&fastthresh, 1, (unsigned int) (FastThresh_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                fastthresh;

        Pixie16IMbufferIO(&FL, 1, (unsigned int) (FastLength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FL;

        // Update channel parameter TriggerThreshold
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) fastthresh / (double) FL;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) fastthresh / ((double) FL * 2.0);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) fastthresh / ((double) FL * 5.0);

    } else if (strcmp(ChanParName, "ENERGY_RISETIME") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&SL, 1, (unsigned int) (SlowLength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlowLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SL;

        Pixie16IMbufferIO(&SlowFilterRange, 1, SlowFilterRange_Address[ModNum], MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                SlowFilterRange;

        // Update channel parameter EnergyRiseTime
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) SL * pow(2.0, (double) SlowFilterRange) /
                           (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) SL * pow(2.0, (double) SlowFilterRange) /
                           (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) SL * pow(2.0, (double) SlowFilterRange) /
                           (double) (Module_Information[ModNum].Module_ADCMSPS / 5);

    } else if (strcmp(ChanParName, "ENERGY_FLATTOP") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&SG, 1, (unsigned int) (SlowGap_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlowGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SG;

        Pixie16IMbufferIO(&SlowFilterRange, 1, SlowFilterRange_Address[ModNum], MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                SlowFilterRange;

        // Update channel parameter EnergyFlatTop
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) SG * pow(2.0, (double) SlowFilterRange) /
                           (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) SG * pow(2.0, (double) SlowFilterRange) /
                           (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) SG * pow(2.0, (double) SlowFilterRange) /
                           (double) (Module_Information[ModNum].Module_ADCMSPS / 5);

    } else if (strcmp(ChanParName, "TAU") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&preamptau, 1, (unsigned int) (PreampTau_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[PreampTau_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                preamptau;

        // Update channel parameter PreampTau
        *ChanParData = IEEEFloating2Decimal(preamptau);

    } else if (strcmp(ChanParName, "TRACE_LENGTH") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&tracelength, 1, (unsigned int) (TraceLength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[TraceLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                tracelength;
        Pixie16IMbufferIO(&FastFilterRange, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                FastFilterRange;

        // Update channel parameter TraceLength
        *ChanParData = (double) tracelength / (double) Module_Information[ModNum].Module_ADCMSPS *
                       pow(2.0, (double) FastFilterRange);

    } else if (strcmp(ChanParName, "TRACE_DELAY") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&paflength, 1, (unsigned int) (PAFlength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                paflength;
        Pixie16IMbufferIO(&triggerdelay, 1, (unsigned int) (TriggerDelay_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                triggerdelay;
        Pixie16IMbufferIO(&FastFilterRange, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] =
                FastFilterRange;

        // Update channel parameter TraceDelay
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData =
                    (double) (paflength - (unsigned int) ((double) triggerdelay / pow(2.0, (double) FastFilterRange))) /
                    (double) Module_Information[ModNum].Module_ADCMSPS * pow(2.0, (double) FastFilterRange);
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData =
                    (double) (paflength - (unsigned int) ((double) triggerdelay / pow(2.0, (double) FastFilterRange))) /
                    (double) (Module_Information[ModNum].Module_ADCMSPS / 2) * pow(2.0, (double) FastFilterRange);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData =
                    (double) (paflength - (unsigned int) ((double) triggerdelay / pow(2.0, (double) FastFilterRange))) /
                    (double) (Module_Information[ModNum].Module_ADCMSPS / 5) * pow(2.0, (double) FastFilterRange);

    } else if (strcmp(ChanParName, "VOFFSET") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&offsetdac, 1, (unsigned int) (OffsetDAC_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[OffsetDAC_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                offsetdac;

        // Update channel parameter DCoffset
        *ChanParData = (double) offsetdac / 65536.0 * DAC_VOLTAGE_RANGE - DAC_VOLTAGE_RANGE / 2.0;

    } else if (strcmp(ChanParName, "XDT") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&xwait, 1, (unsigned int) (Xwait_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[Xwait_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = xwait;

        // Update channel parameter Xdt
        *ChanParData = (double) xwait / (double) DSP_CLOCK_MHZ;
    } else if (strcmp(ChanParName, "BASELINE_PERCENT") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&baselinepercent, 1, (unsigned int) (BaselinePercent_Address[ModNum] + ChanNum), MOD_READ,
                          ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[BaselinePercent_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                baselinepercent;

        // Update channel parameter BaselinePercent
        *ChanParData = (double) baselinepercent;

    } else if (strcmp(ChanParName, "EMIN") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&energylow, 1, (unsigned int) (EnergyLow_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[EnergyLow_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                energylow;

        // Update channel parameter EnergyLow
        *ChanParData = (double) energylow;

    } else if (strcmp(ChanParName, "BINFACTOR") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&log2ebin, 1, (unsigned int) (Log2Ebin_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Ebin_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = log2ebin;

        // Update channel parameter BinFactor
        *ChanParData = (double) (pow(2.0, 32.0) - log2ebin);

    } else if (strcmp(ChanParName, "BASELINE_AVERAGE") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&baselineaverage, 1, (unsigned int) (Log2Bweight_Address[ModNum] + ChanNum), MOD_READ,
                          ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                baselineaverage;

        // Update channel parameter Baseline Average
        if (baselineaverage == 0) {
            *ChanParData = 0.0;
        } else {
            *ChanParData = (double) (pow(2.0, 32.0) - baselineaverage);
        }

    } else if (strcmp(ChanParName, "CHANNEL_CSRA") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&chancsra, 1, (unsigned int) (ChanCSRa_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRa_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chancsra;

        // Update channel parameter ChanCSRA
        *ChanParData = (double) chancsra;

    } else if (strcmp(ChanParName, "CHANNEL_CSRB") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&chancsrb, 1, (unsigned int) (ChanCSRb_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRb_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chancsrb;

        // Update channel parameter ChanCSRB
        *ChanParData = (double) chancsrb;

    } else if (strcmp(ChanParName, "BLCUT") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&baselinecut, 1, (unsigned int) (BLcut_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselinecut;

        // Update channel parameter BaselineCut
        *ChanParData = (double) baselinecut;

    } else if (strcmp(ChanParName, "INTEGRATOR") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&integrator, 1, (unsigned int) (Integrator_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[Integrator_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                integrator;

        // Update channel parameter Integrator
        *ChanParData = (double) integrator;

    } else if (strcmp(ChanParName, "FASTTRIGBACKLEN") == 0) {

        // Read from the selected Pixie module
        Pixie16IMbufferIO(&fasttrigbacklen, 1, (unsigned int) (FastTrigBackLen_Address[ModNum] + ChanNum), MOD_READ,
                          ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                fasttrigbacklen;

        // Update channel parameter FastTrigBackLen
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) fasttrigbacklen / (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) fasttrigbacklen / (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) fasttrigbacklen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);

    } else if (strcmp(ChanParName, "CFDDelay") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&cfddelay, 1, (unsigned int) (CFDDelay_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[CFDDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfddelay;

        // Update channel parameter CFDDelay
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) cfddelay / (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) cfddelay / (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) cfddelay / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);

    } else if (strcmp(ChanParName, "CFDScale") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&cfdscale, 1, (unsigned int) (CFDScale_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[CFDScale_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfdscale;

        // Update channel parameter CFDScale
        *ChanParData = (double) cfdscale;
    } else if (strcmp(ChanParName, "CFDThresh") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&cfdthresh, 1, (unsigned int) (CFDThresh_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[CFDThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                cfdthresh;

        // Update channel parameter CFDThresh
        *ChanParData = (double) cfdthresh;
    } else if (strcmp(ChanParName, "QDCLen0") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen0_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen0_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;

        // Update channel parameter QDCLen0
        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) qdclen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
        else
            *ChanParData = (double) qdclen / (double) Module_Information[ModNum].Module_ADCMSPS;
    } else if (strcmp(ChanParName, "QDCLen1") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen1_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen1_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;

        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) qdclen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
        else
            *ChanParData = (double) qdclen / (double) Module_Information[ModNum].Module_ADCMSPS;
    } else if (strcmp(ChanParName, "QDCLen2") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen2_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen2_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;

        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) qdclen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
        else
            *ChanParData = (double) qdclen / (double) Module_Information[ModNum].Module_ADCMSPS;
    } else if (strcmp(ChanParName, "QDCLen3") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen3_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen3_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;

        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) qdclen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
        else
            *ChanParData = (double) qdclen / (double) Module_Information[ModNum].Module_ADCMSPS;
    } else if (strcmp(ChanParName, "QDCLen4") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen4_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen4_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;

        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) qdclen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
        else
            *ChanParData = (double) qdclen / (double) Module_Information[ModNum].Module_ADCMSPS;
    } else if (strcmp(ChanParName, "QDCLen5") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen5_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen5_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;

        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) qdclen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
        else
            *ChanParData = (double) qdclen / (double) Module_Information[ModNum].Module_ADCMSPS;
    } else if (strcmp(ChanParName, "QDCLen6") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen6_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen6_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;

        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) qdclen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
        else
            *ChanParData = (double) qdclen / (double) Module_Information[ModNum].Module_ADCMSPS;
    } else if (strcmp(ChanParName, "QDCLen7") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&qdclen, 1, (unsigned int) (QDCLen7_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen7_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;

        if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) qdclen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
        else
            *ChanParData = (double) qdclen / (double) Module_Information[ModNum].Module_ADCMSPS;
    } else if (strcmp(ChanParName, "ExtTrigStretch") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&exttrigstretch, 1, (unsigned int) (ExtTrigStretch_Address[ModNum] + ChanNum), MOD_READ,
                          ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[ExtTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                exttrigstretch;

        // Update channel parameter ExtTrigStretch
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) exttrigstretch / (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) exttrigstretch / (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) exttrigstretch / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
    } else if (strcmp(ChanParName, "VetoStretch") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&vetostretch, 1, (unsigned int) (VetoStretch_Address[ModNum] + ChanNum), MOD_READ, ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[VetoStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                vetostretch;

        // Update channel parameter VetoStretch
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) vetostretch / (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) vetostretch / (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) vetostretch / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
    } else if (strcmp(ChanParName, "MultiplicityMaskL") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&multiplicitymaskl, 1, (unsigned int) (MultiplicityMaskL_Address[ModNum] + ChanNum), MOD_READ,
                          ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskL_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                multiplicitymaskl;

        // Update channel parameter MultiplicityMaskL
        *ChanParData = (double) multiplicitymaskl;
    } else if (strcmp(ChanParName, "MultiplicityMaskH") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&multiplicitymaskh, 1, (unsigned int) (MultiplicityMaskH_Address[ModNum] + ChanNum), MOD_READ,
                          ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskH_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                multiplicitymaskh;

        // Update channel parameter MultiplicityMaskH
        *ChanParData = (double) multiplicitymaskh;
    } else if (strcmp(ChanParName, "ExternDelayLen") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&externdelaylen, 1, (unsigned int) (ExternDelayLen_Address[ModNum] + ChanNum), MOD_READ,
                          ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[ExternDelayLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                externdelaylen;

        // Update channel parameter ExternDelayLen
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) externdelaylen / (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) externdelaylen / (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) externdelaylen / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
    } else if (strcmp(ChanParName, "FtrigoutDelay") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&ftrigoutdelay, 1, (unsigned int) (FtrigoutDelay_Address[ModNum] + ChanNum), MOD_READ,
                          ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[FtrigoutDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                ftrigoutdelay;

        // Update channel parameter FtrigoutDelay
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) ftrigoutdelay / (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) ftrigoutdelay / (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) ftrigoutdelay / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
    } else if (strcmp(ChanParName, "ChanTrigStretch") == 0) {
        // Read from the selected Pixie module
        Pixie16IMbufferIO(&chantrigstretch, 1, (unsigned int) (ChanTrigStretch_Address[ModNum] + ChanNum), MOD_READ,
                          ModNum);
        Pixie_Devices[ModNum].DSP_Parameter_Values[ChanTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] =
                chantrigstretch;

        // Update channel parameter ChanTrigStretch
        if (Module_Information[ModNum].Module_ADCMSPS == 100)
            *ChanParData = (double) chantrigstretch / (double) Module_Information[ModNum].Module_ADCMSPS;
        else if (Module_Information[ModNum].Module_ADCMSPS == 250)
            *ChanParData = (double) chantrigstretch / (double) (Module_Information[ModNum].Module_ADCMSPS / 2);
        else if (Module_Information[ModNum].Module_ADCMSPS == 500)
            *ChanParData = (double) chantrigstretch / (double) (Module_Information[ModNum].Module_ADCMSPS / 5);
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "invalid channel parameter name %s", ChanParName);
        return (-3);
    }

    return (0);
}

/**
 * @defgroup DATA_PROCESSING Data Processing
 * @ingroup PUBLIC_API
 * A group of functions used to process data received from the Pixie modules.
 */

/**
 * @ingroup DATA_PROCESSING
 * @brief Retrieve a channel's histogram data from a Pixie module.
 *
 * Use this function to read out the histogram data from a Pixie-16 module’s histogram memory.
 * Before calling this function, the host code should allocate appropriate amount of memory to
 * store the histogram data. The default histogram length is 32768. Histogram data are 32-bit
 * unsigned integers.
 *
 * @param[out] Histogram: The histogram data that we read from the module.
 * @param[in] NumWords: The number of words we'll read out of the histogram memory.
 * @param[in] ModNum: The module number that we want the histogram from
 * @param[in] ChanNum: The channel number that we'd like to read from
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Failed to get the histogram data. If this happens reboot the module.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromModule(unsigned int* Histogram,
                                                                    unsigned int NumWords,
                                                                    unsigned short ModNum,
                                                                    unsigned short ChanNum)
{
    int retval;  // return values
    unsigned int Histo_Address;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }
    if (ChanNum >= NUMBER_OF_CHANNELS) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie channel number %d", ChanNum);
        return (-2);
    }

    Histo_Address = MAX_HISTOGRAM_LENGTH * ChanNum + HISTOGRAM_MEMORY_ADDRESS;
    retval = Pixie_Main_Memory_IO(Histogram, Histo_Address, NumWords, MOD_READ, ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC,
                          "failed to get histogram data from module %d, retval=%d",
                          ModNum, retval);
        return (-3);
    }

    return (0);
}


/**
 * @ingroup DATA_PROCESSING
 * @brief Retrieve statistics data from a module.
 *
 * Use this function to read out statistics data from a Pixie-16 module. Before calling this
 * function, the host code should allocate appropriate amount of memory to store the statistics
 * data. The number of statistics data for each module is fixed at 448. Statistics data are 32-bit
 * unsigned integers.
 *
 * @param[out] Statistics: Pointer to a data block with size 448 32-bit unsigned integers to hold
 *    the data read from the module.
 * @param[in] ModNum: The module number that we want statistics data from.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get statistics data. If this happens, then reboot the module.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadStatisticsFromModule(unsigned int* Statistics,
                                                                     unsigned short ModNum)
{
    int retval;  // return values
    unsigned short nWords, k;  // number of words
    unsigned int DSP_address;  // Start address in DSP memory

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    nWords = N_DSP_PAR - DSP_IO_BORDER;

    // Check if running in OFFLINE mode
    if (Offline == 1) {
        for (k = 0; k < nWords; k += 1) {
            Statistics[k] = Pixie_Devices[ModNum].DSP_Parameter_Values[DSP_IO_BORDER + k];
        }
    } else {
        DSP_address = DATA_MEMORY_ADDRESS + DSP_IO_BORDER;
        retval = Pixie_DSP_Memory_IO(Statistics, DSP_address, nWords, MOD_READ, ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "failed to get statistics data from module %d, retval=%d",
                              ModNum, retval);
            return (-2);
        }
    }

    return (0);
}


/**
 * @ingroup DATA_PROCESSING
 * @brief Retrieve histogram data from a Pixie module and then save the data to a file.
 *
 * Use this function to read histogram data from a Pixie-16 module and save the histogram data to
 * a file with file name specified by the user. First this function saves the histogram data to a
 * binary file, and it then saves the histogram data to an ASCII file with run statistics data
 * appended to the end of the ASCII file. **Existing files will be overwritten.**
 *
 * @param[in] FileName: The file name for the file containing the histogram data
 * @param[in] ModNum: The module number we'll read histogram data from. Counting from 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get histogram data from module
 * @retval -3 - Failed to allocate memory to store histogram data for binary file
 * @retval -4 - Failed to open histogram data file
 * @retval -5 - Failed to open mca ascii output file
 * @retval -6 - Failed to allocate memory to store histogram data for ascii text file
 * @retval -7 - Failed to read histogram data from file
 * @retval -8 - Failed to read run statistics data from module
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveHistogramToFile(const char* FileName,
                                                                unsigned short ModNum)
{
    FILE* HistFile = NULL;
    unsigned int* histdata;
    unsigned int histo_addr;
    unsigned short k;
    int retval;
    unsigned int index;
    unsigned int* MCAData[NUMBER_OF_CHANNELS];
    unsigned short ModuleNumber, ChannelNumber;
    size_t strlength;
    FILE* outfil_mca;
    char outfilename[1024];
    unsigned int run_statistics[448];
    double realtime, livetime, icr, ocr;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    HistFile = fopen(FileName, "wb");
    if (HistFile != NULL) {
        // Allocate memory
        if ((histdata = (unsigned int*) malloc(sizeof(unsigned int) * MAX_HISTOGRAM_LENGTH)) != NULL) {
            for (k = 0; k < NUMBER_OF_CHANNELS; k++) {
                histo_addr = MAX_HISTOGRAM_LENGTH * k + HISTOGRAM_MEMORY_ADDRESS;
                retval = Pixie_Main_Memory_IO(histdata, histo_addr, MAX_HISTOGRAM_LENGTH, MOD_READ, ModNum);
                if (retval < 0) {
                    // Release memory
                    free(histdata);

                    // Close HistFile
                    fclose(HistFile);

                    Pixie_Print_Error(PIXIE_FUNC,
                                      "failed to get histogram data from module %d, retval=%d",
                                      ModNum, retval);
                    return (-2);
                }

                fwrite(histdata, sizeof(unsigned int), MAX_HISTOGRAM_LENGTH, HistFile);
            }

            // Release memory
            free(histdata);
        } else {
            // Close HistFile
            fclose(HistFile);

            Pixie_Print_Error(PIXIE_FUNC,
                              "failed to allocate memory to store list-mode data for module %d",
                              ModNum);
            return (-3);
        }

        // Close HistFile
        fclose(HistFile);
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "failed to open histogram data file %s", FileName);
        return (-4);
    }


    // Save MCA data to files in ASCII text format

    // Generate histogram output file name
    strlength = strlen(FileName);

    if (strlength >= (sizeof(outfilename) - 4)) {
        Pixie_Print_Error(PIXIE_FUNC, "file name too long");
        return (-5);
    }

    memset(outfilename, 0, sizeof(outfilename));
    strncpy(outfilename, FileName, sizeof(outfilename) - 4);
    outfilename[strlength - 3] = '\0';
    strcat(outfilename, "asc");
    outfil_mca = fopen(outfilename, "w");
    if (outfil_mca == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "open mca output file %s failed", outfilename);
        return (-5);
    }

    for (ChannelNumber = 0; ChannelNumber < NUMBER_OF_CHANNELS; ChannelNumber++) {
        MCAData[ChannelNumber] = (unsigned int*) malloc(sizeof(unsigned int) * MAX_HISTOGRAM_LENGTH);
        if (MCAData[ChannelNumber] == NULL) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to allocate memory for MCA data");

            for (index = 0; index < ChannelNumber; index++) { free(MCAData[index]); }

            // Close histogram output file
            fclose(outfil_mca);

            return (-6);
        }
    }

    ModuleNumber = 0;
    for (ChannelNumber = 0; ChannelNumber < NUMBER_OF_CHANNELS; ChannelNumber++) {
        retval = Pixie16ReadHistogramFromFile(FileName, MCAData[ChannelNumber], MAX_HISTOGRAM_LENGTH, ModuleNumber,
                                              ChannelNumber);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "Pixie16ReadHistogramFromFile failed, retval=%d",
                              retval);
            fclose(outfil_mca);
            for (index = 0; index < NUMBER_OF_CHANNELS; index++) { free(MCAData[index]); }
            return (-7);
        }
    }

    // Write MCA data
    for (index = 0; index < NUMBER_OF_CHANNELS; index++) { fprintf(outfil_mca, "Channel#%d\t", index); }
    fprintf(outfil_mca, "\n");

    for (index = 0; index < MAX_HISTOGRAM_LENGTH; index++) {
        for (ChannelNumber = 0; ChannelNumber < NUMBER_OF_CHANNELS; ChannelNumber++) {
            fprintf(outfil_mca, "%d\t", MCAData[ChannelNumber][index]);
        }

        fprintf(outfil_mca, "\n");
    }

    for (index = 0; index < NUMBER_OF_CHANNELS; index++) { free(MCAData[index]); }


    // Append run statistics data to the ASCII MCA data file

    // Read raw run statistics data from the module
    retval = Pixie16ReadStatisticsFromModule(run_statistics, ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Pixie16ReadStatisticsFromModule failed, retval=%d",
                          retval);
        fclose(outfil_mca);
        return (-8);
    }

    // Compute real time
    realtime = Pixie16ComputeRealTime(run_statistics, ModNum);

    // Write real time to file
    fprintf(outfil_mca, "\nReal Time [s] = %f\n", realtime);

    // Write live time to file
    fprintf(outfil_mca, "\nLive Time [s]\n");

    for (index = 0; index < NUMBER_OF_CHANNELS; index++) { fprintf(outfil_mca, "Channel#%d\t", index); }
    fprintf(outfil_mca, "\n");

    for (index = 0; index < NUMBER_OF_CHANNELS; index++) {
        ChannelNumber = (unsigned short) index;
        livetime = Pixie16ComputeLiveTime(run_statistics, ModNum, ChannelNumber);
        fprintf(outfil_mca, "%f\t", livetime);
    }
    fprintf(outfil_mca, "\n");

    // Write input count rate to file
    fprintf(outfil_mca, "\nInput Count Rate [cps]\n");

    for (index = 0; index < NUMBER_OF_CHANNELS; index++) { fprintf(outfil_mca, "Channel#%d\t", index); }
    fprintf(outfil_mca, "\n");

    for (index = 0; index < NUMBER_OF_CHANNELS; index++) {
        ChannelNumber = (unsigned short) index;
        icr = Pixie16ComputeInputCountRate(run_statistics, ModNum, ChannelNumber);
        fprintf(outfil_mca, "%f\t", icr);
    }
    fprintf(outfil_mca, "\n");

    // Write output count rate to file
    fprintf(outfil_mca, "\nOutput Count Rate [cps]\n");

    for (index = 0; index < NUMBER_OF_CHANNELS; index++) { fprintf(outfil_mca, "Channel#%d\t", index); }
    fprintf(outfil_mca, "\n");

    for (index = 0; index < NUMBER_OF_CHANNELS; index++) {
        ChannelNumber = (unsigned short) index;
        ocr = Pixie16ComputeOutputCountRate(run_statistics, ModNum, ChannelNumber);
        fprintf(outfil_mca, "%f\t", ocr);
    }
    fprintf(outfil_mca, "\n");

    // Close MCA ASCII data file
    fclose(outfil_mca);

    return (0);
}


 /**
 * @defgroup LIST_MODE_DATA_PROCESSING List-Mode Data Processing
 * @ingroup PUBLIC_API
 * A group of functions used to process List-Mode data from the modules
 */

 /**
  * @ingroup LIST_MODE_DATA_PROCESSING
  * @brief Parse list-mode events from the data file to get the number of events for each module.
  *
  * The data file must be written with the Pixie16SaveExternalFIFODataToFile to ensure the proper
  * format.
  *
  * @see Pixie16SaveExternalFIFODataToFile
  *
  * @param[in] FileName: An absolute path to the file that we'll parse for events written with
  *     Pixie16SaveExternalFIFODataToFile.
  * @param[out] ModuleEvents: A pointer to an array containing the number of events each module
  *     recorded.
  * @returns A status code indicating the result of the operation
  * @retval  0 - Success
  * @retval -1 - Null pointer *ModuleEvents
  * @retval -2 - Failed to open list-mode data file
  * @retval -3 - Failed to seek to the end of the file.
  * @retval -4 - Failed to seek to beginning of file.
  * @retval -5 - Failed to read event from file
  * @retval -6 - Failed to find next event in file.
  */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetModuleEvents(const char* FileName,
                                                            unsigned int* ModuleEvents)
{

    unsigned int eventdata, eventlength;
    unsigned int TotalWords, TotalSkippedWords;
    FILE* ListModeFile = NULL;
    int retval;

    // Check if ModuleEvents is valid
    if (ModuleEvents == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "Null pointer *ModuleEvents");
        return (-1);
    }

    // Open the list-mode file
    ListModeFile = fopen(FileName, "rb");
    if (ListModeFile != NULL) {
        // Get file length
        retval = fseek(ListModeFile, 0, SEEK_END);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed listmode seek, error=%d",
                              errno);
            (void) fclose(ListModeFile);
            return (-3);
        }

        TotalWords = (ftell(ListModeFile) + 1) / 4;

        // Point ListModeFile to the beginning of file
        retval = fseek(ListModeFile, 0, SEEK_SET);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed listmode seek, error=%d",
                              errno);
            (void) fclose(ListModeFile);
            return (-4);
        }

        // Initialize indicator and counter
        TotalSkippedWords = 0;

        do {
          retval = Pixie16_fread(&eventdata, 4, 1, ListModeFile);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed listmode read, error=%d",
                                  errno);
                (void) fclose(ListModeFile);
                return (-5);
            }
            eventlength = (eventdata & 0x7FFE0000) >> 17;
            TotalSkippedWords += eventlength;
            retval = fseek(ListModeFile, (eventlength - 1) * 4, SEEK_CUR);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed listmode seek, error=%d",
                                  errno);
                (void) fclose(ListModeFile);
                return (-6);
            }
            ModuleEvents[0]++;
        } while (TotalSkippedWords < TotalWords);

        (void) fclose(ListModeFile);
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "can't open list-mode data file %s", FileName);
        return (-2);
    }

    return (0);
}


/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Retrieve the detailed information of each event in the list-mode data file
 *
 * Use this function to retrieve the detailed information (except waveforms) of each event in the
 * list-mode data file for the designated module.
 * Before calling this function to get the individual events information, another function
 * Pixie16GetModuleEvents should be called first to determine the number of events that have been
 * recorded for each module. If the number of events for a given module is nEvents, a memory
 * block `*EventInformation` should be allocated with a length of (`nEvents*68`):
 * ```
 * EventInformation = (unsigned int *)malloc(sizeof(unsigned int) * nEvents * 68);
 * ```
 * where 68 is the length of the information records of each event (energy, timestamps, etc.) and
 * has the following structure.
 *
 * | Index | Value |
 * |-|-|
 * | EventInformation [0] | Event number  |
 * | EventInformation [1] | Channel number |
 * | EventInformation [2] | Slot number |
 * | EventInformation [3] | Crate number |
 * | EventInformation [4] | Header length |
 * | EventInformation [5] | Event length |
 * | EventInformation [6] | Finish code |
 * | EventInformation [7] | Event timestamp (lower 32-bit) |
 * | EventInformation [8] | Event timestamp (upper 16-bit) |
 * | EventInformation [9] | Event energy |
 * | EventInformation [10] | Trace length |
 * | EventInformation [11] | Trace location |
 * | EventInformation [67:12] | Not used |
 *
 * @see Pixie16SaveExternalFIFODataToFile
 * @see Pixie16GetModuleEvents
 *
 * @param[in] FileName: Absolute path to the file containing list-mode data written with
 *     Pixie16SaveExternalFIFODataToFile
 * @param[out] EventInformation: Pointer to an array with size `number_of_events * 68`.
 * @param[in] ModuleNumber: The module number that we'll retrieve information about
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Null pointer *EventInformation
 * @retval -2 - Target module number is invalid
 * @retval -3 - Failed to open list-mode data file
 * @retval -4 - Failed to seek to the end of the file.
 * @retval -5 - Failed to seek to beginning of file.
 * @retval -6 - Failed to read Word 0 from the event
 * @retval -7 - Failed to read Word 1 from the event
 * @retval -8 - Failed to read Word 2 from the event
 * @retval -9 - Failed to read Word 3 from the event
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetEventsInfo(const char* FileName,
                                                          unsigned int* EventInformation,
                                                          unsigned short ModuleNumber)
{

    unsigned int eventdata, headerlength, eventlength;
    unsigned int TotalWords, TotalSkippedWords, NumEvents;
    FILE* ListModeFile = NULL;
    int retval;

    // Check if EventInformation is valid
    if (EventInformation == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "Null pointer *EventInformation");
        return (-1);
    }

    if (ModuleNumber >= PRESET_MAX_MODULES) {
        Pixie_Print_Error(PIXIE_FUNC, "Target module number is invalid %d", ModuleNumber);
        return (-2);
    }

    // Open the list-mode file
    ListModeFile = fopen(FileName, "rb");
    if (ListModeFile != NULL) {
        // Get file length
        retval = fseek(ListModeFile, 0, SEEK_END);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed listmode seek for module %d, error=%d",
                              ModuleNumber, errno);
            (void) fclose(ListModeFile);
            return (-4);
        }
        TotalWords = (ftell(ListModeFile) + 1) / 4;
        // Point ListModeFile to the beginning of file
        retval = fseek(ListModeFile, 0, SEEK_SET);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed listmode seek for module %d, error=%d",
                              ModuleNumber, errno);
            (void) fclose(ListModeFile);
            return (-5);
        }

        // Initialize indicator and counter
        TotalSkippedWords = 0;
        NumEvents = 0;

        do {
            retval = Pixie16_fread(&eventdata, 4, 1, ListModeFile);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed listmode read for module %d, error=%d",
                                  ModuleNumber, errno);
                (void) fclose(ListModeFile);
                return (-6);
            }

            // Event #
            EventInformation[EVENT_INFO_LENGTH * NumEvents] = NumEvents;
            // Channel #
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 1] = (eventdata & 0xF);
            // Slot #
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 2] = (eventdata & 0xF0) >> 4;
            // Crate #
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 3] = (eventdata & 0xF00) >> 8;
            // Header length
            headerlength = (eventdata & 0x1F000) >> 12;
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 4] = (eventdata & 0x1F000) >> 12;
            // Event length
            eventlength = (eventdata & 0x7FFE0000) >> 17;
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 5] = (eventdata & 0x7FFE0000) >> 17;
            // Finish code
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 6] = (eventdata & 0x80000000) >> 31;

            retval = Pixie16_fread(&eventdata, 4, 1, ListModeFile);
            // EventTime_Low
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 7] = eventdata;

            retval = Pixie16_fread(&eventdata, 4, 1, ListModeFile);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed listmode read for module %d, error=%d",
                                  ModuleNumber, errno);
                (void) fclose(ListModeFile);
                return (-7);
            }

            // EventTime_High
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 8] = (eventdata & 0xFFFF);

            retval = Pixie16_fread(&eventdata, 4, 1, ListModeFile);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed listmode read for module %d, error=%d",
                                  ModuleNumber, errno);
                (void) fclose(ListModeFile);
                return (-8);
            }

            // Event Energy
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 9] = (eventdata & 0xFFFF);
            // Trace Length
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 10] = (eventdata & 0x7FFF0000) >> 16;
            // Trace location
            EventInformation[EVENT_INFO_LENGTH * NumEvents + 11] = TotalSkippedWords + headerlength;

            TotalSkippedWords += eventlength;
            NumEvents++;

            retval = fseek(ListModeFile, (eventlength - 4) * 4, SEEK_CUR);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC, "failed listmode seek for module %d, error=%d",
                                  ModuleNumber, errno);
                (void) fclose(ListModeFile);
                return (-9);
            }
        } while (TotalSkippedWords < TotalWords);

        (void) fclose(ListModeFile);
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "can't open list-mode data file %s", FileName);
        return (-3);
    }

    return (0);
}


/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Retrieve a trace from list-mode data in a file.
 *
 * It uses the trace length and file location information obtained from function
 * Pixie16GetEventsInfo for the selected event. This function will only retrieve a single trace at
 * a time.
 *
 * @see Pixie16GetEventsInfo
 * @see Pixie16SaveExternalFIFODataToFile
 *
 * @param[in] FileName: Absolute path to the list-mode data file written with
 *     Pixie16SaveExternalFIFODataToFile
 * @param[out] Trace_Data: A pointer to an array of 16-bit unsigned integers that will hold the
 *     traces. The array should have enough elements to hold NumWords entries.
 * @param[in] NumWords: The number of 16-bit unsigned integers we'll read from the data file.
 * @param[in] FileLocation: The location of the trace in the file.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to open list-mode data file
 * @retval -2 - Failed to seek to the provided trace location
 * @retval -3 - Failed to read the trace from the file
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadListModeTrace(const char* FileName,
                                                              unsigned short* Trace_Data,
                                                              unsigned short NumWords,
                                                              unsigned int FileLocation)
{
    FILE* ListModeFile = NULL;

    // Open the list-mode file
    ListModeFile = fopen(FileName, "rb");
    if (ListModeFile != NULL) {
        int retval;

        // Position ListModeFile to the requested trace location
        retval = fseek(ListModeFile, FileLocation * 4, SEEK_SET);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed listmode data seek, error=%d",
                              errno);
            (void) fclose(ListModeFile);
            return (-2);
        }

        // Read trace
        retval = Pixie16_fread(Trace_Data, 2, NumWords, ListModeFile);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed listmode read, error=%d",
                              errno);
            (void) fclose(ListModeFile);
            return (-3);
        }

        (void) fclose(ListModeFile);

    } else {
        Pixie_Print_Error(PIXIE_FUNC, "can't open list-mode file %s", FileName);
        return (-1);
    }

    return (0);
}


/**
* @defgroup HISTOGRAM_DATA_PROCESSING Histogram Data Processing
* @ingroup PUBLIC_API
* A group of functions used to process Histogram data from the modules
*/

/**
 * @ingroup HISTOGRAM_DATA_PROCESSING
 * @brief Retrieve histogram data from a file.
 *
 * Use this function to read histogram data from a histogram data file. Before calling this
 * function, the host code should allocate appropriate amount of memory to store the histogram
 * data. The default histogram length is 32768. Histogram data are 32-bit unsigned integers.
 *
 * @see Pixie16SaveHistogramToFile
 *
 * @param[in] FileName: Absolute path to the list-mode data file written with
 *     Pixie16SavehistogramToFile
 * @param[out] Histogram: Pointer to an array of 32-bit unsigned integers to hold the
 *     histogram data read from the file. It should contain `NumWords` elements.
 * @param[in] NumWords: The number of words to be read out.
 * @param[in] ModNum: The module number that we'll process with counting starting at 0.
 * @param[in] ChanNum: The channel number that we'll process with counting starting at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to open the histogram file
 * @retval -2 - No histogram data is available for this channel
 * @retval -3 - Failed to seek to the end of the histogram file
 * @retval -4 - Failed to seek to the beginning of the histogram file
 * @retval -5 - Failed to seek to the histogram data in the file
 * @retval -6 - Failed to read the histogram data from the file.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromFile(const char* FileName,
                                                                  unsigned int* Histogram,
                                                                  unsigned int NumWords,
                                                                  unsigned short ModNum,
                                                                  unsigned short ChanNum)
{

    unsigned int Histo_Address, TotalWords;
    FILE* HistogramFile = NULL;

    // Open the histogram data file
    HistogramFile = fopen(FileName, "rb");
    if (HistogramFile != NULL) {
        int retval;

        // Get file length
        retval = fseek(HistogramFile, 0, SEEK_END);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed histogram seek, error=%d",
                              errno);
            (void) fclose(HistogramFile);
            return (-3);
        }
        TotalWords = (ftell(HistogramFile) + 1) / 4;
        // Point HistogramFile to the beginning of file
        retval = fseek(HistogramFile, 0, SEEK_SET);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed histogram seek, error=%d",
                              errno);
            (void) fclose(HistogramFile);
            return (-4);
        }

        Histo_Address = MAX_HISTOGRAM_LENGTH * ChanNum;
        if (Histo_Address > (TotalWords - NumWords)) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "no histogram data is available in file %s for channel %d of module %d",
                              FileName, ChanNum, ModNum);
            (void) fclose(HistogramFile);
            return (-2);
        }

        // Read the data

        // Point HistogramFile to the right location of the histogram data file
        retval = fseek(HistogramFile, Histo_Address * 4, SEEK_CUR);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed histogram seek, error=%d",
                              errno);
            (void) fclose(HistogramFile);
            return (-5);
        }

        retval = Pixie16_fread(Histogram, 4, NumWords, HistogramFile);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed histogram read, error=%d",
                              errno);
            (void) fclose(HistogramFile);
            return (-6);
        }

        // Close the file
        (void) fclose(HistogramFile);
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "failed to open histogram file %s", FileName);
        return (-1);
    }

    return (0);
}



/**
 * @ingroup CONFIGURATION
 * @brief Save DSP parameters to a settings file
 *
 * Use this function to save DSP parameters to a settings file. It will first read the values of
 * DSP parameters on each Pixie-16 module and then write them to the settings file. Each module has
 * exactly 1280 DSP parameter values (32-bit unsigned integers), and depending on the value of
 * PRESET_MAX_MODULES (defined in pixie16app_defs.h), the settings file should have exactly
 * `(1280 * PRESET_MAX_MODULES * 4)` bytes when stored on the computer hard drive.
 *
 * @param[in] FileName: Absolute path to where we will write the DSP parameter file.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to read DSP parameter values from the Pixie-16 modules
 * @retval -2 - Failed to open the DSP parameters file
 * @retval -3 - Failed to write DSP parameter values
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveDSPParametersToFile(const char* FileName)
{
    unsigned short ModNum;
    FILE* DSPSettingsFile = NULL;
    int retval;

    // Open the DSP parameters file
    DSPSettingsFile = fopen(FileName, "wb");
    if (DSPSettingsFile != NULL) {
        // Get the current DSP parameter values from the Pixie-16 board
        for (ModNum = 0; ModNum < Number_Modules; ModNum++) {
            retval = Pixie16IMbufferIO(Pixie_Devices[ModNum].DSP_Parameter_Values, N_DSP_PAR, DATA_MEMORY_ADDRESS,
                                       MOD_READ, ModNum);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "failed to read DSP parameter values from module %d, retval = %d",
                                  ModNum, retval);
                (void) fclose(DSPSettingsFile);
                return (-1);
            }
        }

        // Write DSP parameter values to the settings file
        for (ModNum = 0; ModNum < PRESET_MAX_MODULES; ModNum++) {
          retval = (int) fwrite(Pixie_Devices[ModNum].DSP_Parameter_Values, sizeof(unsigned int), N_DSP_PAR, DSPSettingsFile);
            if (retval != N_DSP_PAR) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "failed to write DSP parameter values from module %d, error=%d",
                                  ModNum, errno);
                (void) fclose(DSPSettingsFile);
                return (-3);
            }
        }

        // Close the file
        (void) fclose(DSPSettingsFile);
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "failed to open DSP parameters file %s", FileName);
        return (-2);
    }

    return (0);
}


/**
 * @ingroup CONFIGURATION
 * @brief Load DSP parameters from a settings file
 *
 * Use this function to read DSP parameters from a settings file and then download the settings to
 * Pixie-16 modules that are installed in the system. Each module has exactly 1280 DSP parameter
 * values (32-bit unsigned integers), and depending on the value of PRESET_MAX_MODULES
 * (defined in pixie16app_defs.h), the settings file should have exactly
 * `(1280 * PRESET_MAX_MODULES * 4)` bytes when stored on the computer hard drive.
 * @param[in] FileName: Absolute path to where we will write the DSP parameter file.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Size of DSPParFile is invalid
 * @retval -2 - Failed to program Fippi in module
 * @retval -3 - Failed to set DACs in module
 * @retval -4 - Failed to open the DSP parameters file
 * @retval -5 - Failed to seek to the end of the file
 * @retval -6 - Failed to seek to the beginning of the file
 * @retval -7 - Failed to read settings from the file
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16LoadDSPParametersFromFile(const char* FileName)
{
    unsigned short k;
    unsigned int TotalWords;
    int retval;
    FILE* DSPSettingsFile = NULL;

    // Open DSP parameters file
    if ((DSPSettingsFile = fopen(FileName, "rb")) != NULL)  // Make sure DSPSettingsFile is opened successfully
    {
        // Check if file size is consistent with predefined length (N_DSP_PAR * PRESET_MAX_MODULES)
        retval = fseek(DSPSettingsFile, 0, SEEK_END);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "failed seek Fippi settings, error=%d",
                              errno);
            (void) fclose(DSPSettingsFile);
            return (-5);
        }

        TotalWords = (ftell(DSPSettingsFile) + 1) / 4;
        if (TotalWords != (N_DSP_PAR * PRESET_MAX_MODULES)) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "size of DSPParFile is invalid. Check DSPParFile name %s",
                              FileName);
            (void) fclose(DSPSettingsFile);
            return (-1);
        }

        // Point configfil to the beginning of file
        retval = fseek(DSPSettingsFile, 0, SEEK_SET);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "failed seek Fippi settings, error=%d",
                              errno);
            (void) fclose(DSPSettingsFile);
            return (-6);
        }

        // Read DSP parameters
        for (k = 0; k < PRESET_MAX_MODULES; k++) {
            retval = Pixie16_fread(&Pixie_Devices[k].DSP_Parameter_Values[0], sizeof(unsigned int), N_DSP_PAR, DSPSettingsFile);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "failed to read Fippi settings in module %d, error=%d",
                                  k, errno);
                (void) fclose(DSPSettingsFile);
                return (-7);
            }
            // Force correct module number
            Pixie_Devices[k].DSP_Parameter_Values[0] = k;
        }

        fclose(DSPSettingsFile);

        // Download to all modules
        /// TODO: Add a flag here to turn off downloading parameters to modules if we're offline.
        for (k = 0; k < Number_Modules; k++) {
            Pixie_DSP_Memory_IO(&Pixie_Devices[k].DSP_Parameter_Values[0], DATA_MEMORY_ADDRESS, DSP_IO_BORDER,
                                MOD_WRITE, k);

            // Always re-program fippi after downloading DSP parameters
            retval = Pixie16ProgramFippi(k);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "failed to program Fippi in module %d, retval=%d",
                                  k, retval);
                return (-2);
            }

            retval = Pixie16SetDACs(k);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "failed to set DACs in module %d, retval=%d", k,
                                  retval);
                return (-3);
            }
        }
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "failed to open DSP parameters file %s", FileName);
        return (-4);
    }

    return (0);
}


/**
 * @ingroup CONFIGURATION
 * @brief Copy DSP parameters from one module to other modules.
 *
 * Use this function to copy DSP parameters from one module to the others that are installed in
 * the system.
 *
 * @param[in] BitMask: A bit pattern that designates what items should be copied from the source
 *     module to the destination module(s). For example, a value of 4097
 *     (2<sup>0</sup> + 2<sup>12</sup>) will copy the filter parameters and QDC.
 *     | Bit | Item |
 *     |-|-|
 *     | 0 | Filter (energy and trigger) |
 *     | 1 | Analog signal conditioning (polarity, dc-offset,   gain/attenuation) |
 *     | 2 | Histogram control (minimum energy, binning factor) |
 *     | 3 | Decay time |
 *     | 4 | Pulse shape analysis (trace length and trace delay) |
 *     | 5 | Baseline control (baseline cut, baseline percentage) |
 *     | 7 | Channel CSRA register (good channel, trigger enabled, etc.)   |
 *     | 8 | CFD trigger (CFD delay, scaling factor) |
 *     | 9 | Trigger stretch lengths (veto, external trigger, etc.) |
 *     | 10 | FIFO delays (analog input delay, fast trigger output delay,   etc.) |
 *     | 11 | Multiplicity (bit masks, thresholds, etc.) |
 *     | 12 | QDC (QDC sum lengths) |
 * @param[in] SourceModule: The module that we'll read the settings from, starts counting at 0.
 * @param[in] SourceChannel: The channel we'll read settings from, starts counting at 0.
 * @param[in] DestinationMask: A pointer to an array that indicates the channel and module whose
 *     settings will be copied from the source channel and module. For instance, if there are 5
 *     modules (total 80 channels) in the system, DestinationMask would be defined as
 *     `DestinationMask[80]`, where `DestinationMask[0]` to `DestinationMask[15]` would select
 *     channel 0 to 15 of module 0, `DestinationMask[16]` to `DestinationMask[31]` would select
 *     channel 0 to 15 of module 1, and so on. If a given channel i is to be copied,
 *     then `DestinationMask[i]` should be set to 1, otherwise, it should be set to 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to program Fippi in a module
 * @retval -2 - Failed to set DACs in a module
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CopyDSPParameters(unsigned short BitMask,  // copy items bit mask
                         unsigned short SourceModule,  // source module
                         unsigned short SourceChannel,  // source channel
                         unsigned short* DestinationMask)  // the destination module and channel bit mask
{
    unsigned short i, j, k;
    int retval;

    // Copy settings to the destination modules and channels
    for (i = 0; i < Number_Modules; i++) {
        for (j = 0; j < NUMBER_OF_CHANNELS; j++) {
            if (DestinationMask[i * NUMBER_OF_CHANNELS + j] == 1) {
                Pixie_CopyDSPParameters(BitMask, SourceModule, SourceChannel, i, j);
            }
        }
    }

    // Download to all modules
    for (k = 0; k < Number_Modules; k++) {
        Pixie_DSP_Memory_IO(&Pixie_Devices[k].DSP_Parameter_Values[0], DATA_MEMORY_ADDRESS, DSP_IO_BORDER, MOD_WRITE,
                            k);

        // Always re-program fippi after downloading DSP parameters
        retval = Pixie16ProgramFippi(k);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to program Fippi in module %d, retval=%d", k,
                              retval);
            return (-1);
        }

        retval = Pixie16SetDACs(k);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to set DACs in module %d, retval=%d", k,
                              retval);
            return (-2);
        }
    }

    return (0);
}


/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Read list-mode data from the external FIFO of module and save the data to a file.
 *
 * This function first checks the status of the external FIFO of a Pixie-16 module. If the FIFO
 * data exceeds the size of EXTFIFO_READ_THRESH then we read list-mode data (32-bit unsigned
 * integers) from the external FIFO. This function essentially encapsulates both functions
 * Pixie16CheckExternalFIFOStatus and Pixie16ReadDataFromExternalFIFO within one function. The
 * number of words that are read from the external FIFO is recorded in variable `*nFIFOWords`.
 *
 * @see Pixie16CheckExternalFIFOStatus
 * @see Pixie16ReadDataFromExternalFIFO
 * @see EXTFIFO_READ_THRESH
 *
 * @param[in] FileName: Absolute path to the file that we'll write data into
 * @param[out] nFIFOWords: The number of words read from the FIFO and written to the file.
 * @param[in] ModNum: The module number for the module whose external FIFO we want to read.
 * @param[in] EndOfRunRead: Provide a 1 if this is the final read of a run, 0 otherwise. This is
 *     necessary since the external FIFO needs special treatment when the host reads the last few
 *     words from the external FIFO due to its pipelined structure.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to allocate memory to store list-mode data
 * @retval -3 - Failed to open list-mode data file
 * @retval -4 - Failed to read external FIFO status
 * @retval -5 - Failed to read data from external FIFO
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveExternalFIFODataToFile(const char* FileName,
                                                                       unsigned int* nFIFOWords,
                                                                       unsigned short ModNum,
                                                                       unsigned short EndOfRunRead)
{
    unsigned int nWords;
    FILE* ListFile = NULL;
    unsigned int* lmdata;
    int retval;

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        *nFIFOWords = 0;
        return (-1);
    }

    // Allocate memory
    if ((lmdata = (unsigned int*) malloc(sizeof(unsigned int) * EXTERNAL_FIFO_LENGTH)) == NULL) {
        Pixie_Print_Error(PIXIE_FUNC,
                          "failed to allocate memory to store list-mode data for module %d",
                          ModNum);
        *nFIFOWords = 0;
        return (-2);
    }

    ListFile = fopen(FileName, "ab");
    if (ListFile != NULL) {
        // Check how many words are in the external FIFO
        retval = Pixie_Read_ExtFIFOStatus(&nWords, ModNum);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "failed to read external FIFO status in module %d, retval = %d",
                              ModNum, retval);
            fclose(ListFile);
            free(lmdata);
            *nFIFOWords = 0;
            return (-4);
        }

        // Read data out from the external FIFO if the the number of words exceeds threshold
        // if EndOfRunRead == 1, check if nWords is greater than 0
        if (((EndOfRunRead == 0) && (nWords > EXTFIFO_READ_THRESH)) || ((EndOfRunRead == 1) && (nWords > 0))) {
            retval = Pixie_ExtFIFO_Read(lmdata, nWords, ModNum);
            if (retval < 0) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "failed to read data from external FIFO in module %d, retval = %d",
                                  ModNum, retval);
                fclose(ListFile);
                free(lmdata);
                *nFIFOWords = 0;
                return (-5);
            } else {
                fwrite(lmdata, 4, nWords, ListFile);
                *nFIFOWords = nWords;
            }
        } else {
            *nFIFOWords = 0;
        }
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "failed to open list-mode data file %s", FileName);
        free(lmdata);
        *nFIFOWords = 0;
        return (-3);
    }

    fclose(ListFile);
    free(lmdata);

    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Read from or write to a register in a module.
 * @param[in] ModNum: The module number that we'll be performing IO with. Numbered starting at 0.
 * @param[in] address: The memory address that we'd like to access on the module.
 * @param[in] direction: The direction of the access. Either MOD_READ (1) or MOD_WRITE (0)
 * @param[in,out] value: A pointer to a 32-bit unsigned integer (or array of integers) that holds
 *     the IO data.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RegisterIO(unsigned short ModNum,
                                                       unsigned int address,
                                                       unsigned short direction,
                                                       unsigned int* value)
{
    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    Pixie_Register_IO(ModNum, address, direction, value);
    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Read the value of the CSR of a module.
 *
 * @note Direct reading or writing by the host is not recommended!!

 * Use this function to read the host Control & Status Register (CSR) value. This register is
 * unrelated to the DSP parameters ModCSRA/B, ChanCSRA/B. It is used to control the operation of
 * the module and read directly by the host.
 *
 * @param[in] ModNum: The module number that we want to query
 * @param[out] CSR: Pointer to a 32-bit unsigned integer to hold the CSR value.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadCSR(unsigned short ModNum, unsigned int* CSR) {
    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    Pixie_ReadCSR(ModNum, CSR);
    return (0);
}


/**
 * @ingroup PUBLIC_API
 * @brief Write a value to the CSR of a module.
 *
 * @note Direct reading or writing by the host is not recommended!!
 *
 * Use this function to write a value to the host Control & Status Register (CSR). This register
 * is unrelated to the DSP parameters ModCSRA/B, ChanCSRA/B. It is used to control the operation
 * of the module and read directly by the host.
 *
 * @param[in] ModNum
 * @param[in] CSR: A 32-bit unsigned integer that we'll write to the CSR.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteCSR(unsigned short ModNum, unsigned int CSR) {
    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    Pixie_WrtCSR(ModNum, CSR);
    return (0);
}


/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Read the number of 32-bit words that the external FIFO currently has.
 *
 * Use this function to check the status of the external FIFO of a Pixie-16 module while a
 * list-mode data acquisition run is in progress. The function returns the number of words
 * (32-bit) that the external FIFO currently has. If the number of words is greater than a
 * user-set threshold, function Pixie16ReadDataFromExternalFIFO can then be used to read the data
 * from the external FIFO. The threshold can be set by the user to either minimize reading
 * overhead or to read data out of the FIFO as quickly as possible. The Pixie-16 API
 * (pixie16app_defs.h) has defined a threshold with value of 1024 for external FIFO read out
 * (EXTFIFO_READ_THRESH).
 *
 * @see Pixie16ReadDataFromExternalFIFO
 *
 * @param[out] nFIFOWords: The number of 32-bit unsigned integer words contained in the module's
 *     External FIFO
 * @param[in] ModNum: The module number to read from. Numbering starts counting at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckExternalFIFOStatus(unsigned int* nFIFOWords, unsigned short ModNum) {
    int retval;  // return values
    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    retval = Pixie_Read_ExtFIFOStatus(nFIFOWords, ModNum);
    return (retval);
}


/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Read data from the external FIFO.
 *
 * This function reads list mode data from the external FIFO of a Pixie-16 module. The data are
 * 32-bit unsigned integers. Normally, function Pixie16CheckExternalFIFOStatus is called first to
 * see how many words the external FIFO currently has, and then this function is called to read
 * the data from the FIFO.
 *
 * @see Pixie16CheckExternalFIFOStatus
 *
 * @param[out] ExtFIFO_Data: A pointer to a memory block containing enough space for `nFIFOWords`
 *    worth of 32-bit unsigned integers.
 * @param[in] nFIFOWords: The number of words that we're going to be reading.
 * @param[in] ModNum: The module number that we'll read from. Numbering starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to read data from external FIFO
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data,
                                                                     unsigned int nFIFOWords,
                                                                     unsigned short ModNum)
{
    int retval;  // return values

    // Check if ModNum is valid
    if (ModNum >= Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "invalid Pixie module number %d", ModNum);
        return (-1);
    }

    retval = Pixie_ExtFIFO_Read(ExtFIFO_Data, nFIFOWords, ModNum);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC,
                          "failed to read data from external FIFO in module %d, retval=%d",
                          ModNum, retval);
        return (-2);
    }

    return (0);
}


/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Compute the fast-filter response of a trace listed in a list-mode data file.
 *
 * Use this function to compute fast filter response of a single event in the list-mode data file.
 * The algorithm implemented in this offline analysis function is equivalent to
 * the one implemented in the Pixie-16 hardware. So this function can be used to analyze how the
 * leading edge fast trigger filter and the CFD filter implemented in the hardware respond to the
 * shape of recorded traces. By analyzing the response of these filters, it is possible to
 * optimize the performance of the leading-edge trigger or CFD trigger by adjusting the
 * fast-filter and CFD parameters offline. Such optimized parameters can then be saved to a
 * settings file to be used for online data acquisition.
 *
 * This function uses the globally defined configuration arrays to obtain the filter parameters.
 * These parameters need to be loaded from a configuration file using
 * Pixie16LoadDSPParametersFromFile. Users will also need to set the module's offline variant with
 * the function Pixie16SetOfflineVariant.
 *
 * @see Pixie16SaveExternalFIFODataToFile
 * @see Pixie16SetOfflineVariant
 * @see Pixie16LoadDSPParametersFromFile
 *
 * @param[in] FileName: Absolute path to a file containing list-mode data written by
 *     Pixie16SaveExternalFIFODataToFile
 * @param[in] ModuleNumber: The module number that we'd like to analyze. Numbering starts at 0.
 * @param[in] ChannelNumber: The channel that we'd like to analyze. Numbering starts at 0.
 * @param[in] FileLocation: The memory address of the beginning of the trace in the file.
 * @param[in] RcdTraceLength: The length of the trace in the file. Maximum of 32768.
 * @param[in] RcdTrace: The recorded trace that we'll be analyzing. Maximum length of 32768.
 * @param[out] fastfilter: Pointer to an array of 32-bit unsigned integers with length matching
 *    `RcdTraceLength`. Maximum length of 32768.
 * @param[out] cfd: Pointer to an array of 32-bit unsigned integers with length matching
 *    `RcdTraceLength`. Maximum length of 32768.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Null pointer *RcdTrace
 * @retval -2 - Null pointer *fastfilter
 * @retval -3 - Null pointer *cfd
 * @retval -4 - Target module number is invalid
 * @retval -5 - Trace length is too short
 * @retval -6 - Failed to open list-mode data file
 * @retval -7 - Failed to seek to the trace in the file
 * @retval -8 - Failed to read the trace from the file
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ComputeFastFiltersOffline(const char* FileName,
                                                                      unsigned short ModuleNumber,
                                                                      unsigned short ChannelNumber,
                                                                      unsigned int FileLocation,
                                                                      unsigned short RcdTraceLength,
                                                                      unsigned short* RcdTrace,
                                                                      double* fastfilter,
                                                                      double* cfd)
{

    FILE* ListModeFile = NULL;
    unsigned int FastLen, FastGap, FastFilterRange, CFD_Delay, CFD_W;
    unsigned int fsum0[32768], fsum1[32768];
    unsigned int offset, x, y;
    double cfdscale;
    unsigned short B, D;
    int retval;

    // Check if RcdTrace is valid
    if (RcdTrace == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "Null pointer *RcdTrace");
        return (-1);
    }

    // Check if fastfilter is valid
    if (fastfilter == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "Null pointer *fastfilter");
        return (-2);
    }

    // Check if cfd is valid
    if (cfd == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "Null pointer *cfd");
        return (-3);
    }

    if (ModuleNumber >= PRESET_MAX_MODULES) {
        Pixie_Print_Error(PIXIE_FUNC, "Target module number is invalid %d", ModuleNumber);
        return (-4);
    }

    // Retrieve channel parameters
    FastFilterRange = Pixie_Devices[ModuleNumber]
                              .DSP_Parameter_Values[FastFilterRange_Address[ModuleNumber] - DATA_MEMORY_ADDRESS];
    FastLen = Pixie_Devices[ModuleNumber]
                      .DSP_Parameter_Values[FastLength_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS] *
              (unsigned int) pow(2.0, (double) FastFilterRange);
    FastGap = Pixie_Devices[ModuleNumber]
                      .DSP_Parameter_Values[FastGap_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS] *
              (unsigned int) pow(2.0, (double) FastFilterRange);
    CFD_Delay = Pixie_Devices[ModuleNumber]
                        .DSP_Parameter_Values[CFDDelay_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS];
    CFD_W = Pixie_Devices[ModuleNumber]
                    .DSP_Parameter_Values[CFDScale_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS];

    // Scale up fast filter and CFD parameters for 250 MHz and 500 MHz modules
    if (Module_Information[ModuleNumber].Module_ADCMSPS == 250) {
        FastLen *= 2;
        FastGap *= 2;
        CFD_Delay *= 2;
    } else if (Module_Information[ModuleNumber].Module_ADCMSPS == 500) {
        FastLen *= 5;
        FastGap *= 5;
        //for 500 MHz modules, fixed CFD parmeters were used, so there is no need for scale-up of CFD_Delay
    }

    // Check if trace length is sufficiently long
    if (RcdTraceLength < ((2 * FastLen + FastGap) * 2)) {
        Pixie_Print_Error(PIXIE_FUNC, "the length of recorded trace is too short");
        return (-5);
    }

    // Open the list-mode file
    ListModeFile = fopen(FileName, "rb");
    if (ListModeFile != NULL) {
        // Position ListModeFile to the requested trace location
        retval = fseek(ListModeFile, FileLocation * 4, SEEK_SET);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to seek listmode file for module %d, error=%d",
                              ModuleNumber, errno);
            (void) fclose(ListModeFile);
            return (-7);
        }

        // Read trace
        retval = Pixie16_fread(RcdTrace, 2, RcdTraceLength, ListModeFile);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failed to read listmode file for module %d, error=%d",
                              ModuleNumber, errno);
            (void) fclose(ListModeFile);
            return (-8);
        }

        // Close file
        (void) fclose(ListModeFile);

        // Compute fast filter response
        offset = 2 * FastLen + FastGap - 1;
        for (x = offset; x < RcdTraceLength; x++) {
            fsum0[x] = 0;
            for (y = (x - offset); y < (x - offset + FastLen); y++) { fsum0[x] += RcdTrace[y]; }
            fsum1[x] = 0;
            for (y = (x - offset + FastLen + FastGap); y < (x - offset + 2 * FastLen + FastGap); y++) {
                fsum1[x] += RcdTrace[y];
            }
            fastfilter[x] = ((double) fsum1[x] - (double) fsum0[x]) / (double) FastLen;
        }

        // Extend the value of fastfilter[offset] to all non-computed ones from index 0 to offset-1
        for (x = 0; x < offset; x++) { fastfilter[x] = fastfilter[offset]; }

        // Compute CFD values - 100 MHz and 250 MHz modules use different algorithm than 500 MHz modules

        if ((Module_Information[ModuleNumber].Module_ADCMSPS == 100) ||
            (Module_Information[ModuleNumber].Module_ADCMSPS == 250)) {
            //////--- 100 MHz and 250 MHz modules ---//////
            // Decide CFD Scale value
            cfdscale = 1.0 - (double) CFD_W * 0.125;

            // Compute CFD
            for (x = CFD_Delay; x < RcdTraceLength; x++) {
                cfd[x] = (-fastfilter[x - CFD_Delay] + fastfilter[x] * cfdscale) * (double) FastLen;
            }

            // Extend the value of cfd[CFD_Delay] to all non-computed ones from index 0 to CFD_Delay-1
            for (x = 0; x < CFD_Delay; x++) { cfd[x] = cfd[CFD_Delay]; }
        } else if (Module_Information[ModuleNumber].Module_ADCMSPS == 500) {
            //////---500 MHz modules ---//////
            // fixed CFD parameter values: w = 1.0, B = 5. D = 5, L = 1
            B = 5;
            D = 5;
            for (x = (B + D); x < (unsigned int) (RcdTraceLength - 1); x++) {
                cfd[x] = (RcdTrace[x] + RcdTrace[x + 1]) - (RcdTrace[x - B] + RcdTrace[x - B + 1]) -
                         (RcdTrace[x - D] + RcdTrace[x - D + 1]) + (RcdTrace[x - B - D] + RcdTrace[x - B - D + 1]);
            }

            // Extend the value of cfd[CFD_Delay] to all non-computed ones
            for (x = 0; x < (unsigned int) (B + D); x++) { cfd[x] = cfd[B + D]; }
            cfd[RcdTraceLength - 1] = cfd[RcdTraceLength - 2];
        } else  //invalid module variant
        {
            for (x = 0; x < RcdTraceLength; x++) { cfd[x] = 0.0; }
        }
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "can't open list-mode file %s", FileName);
        return (-6);
    }

    return (0);
}


/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Compute the slow-filter response of a trace listed in a list-mode data file.
 *
 * Use this function to compute slow-filter response of a single event in the list-mode data file.
 * The algorithm implemented in this offline analysis function is equivalent to
 * the one implemented in the Pixie-16 hardware. So this function can be used to analyze how the
 * leading edge fast trigger filter and the CFD filter implemented in the hardware respond to the
 * shape of recorded traces. By analyzing the response of these filters, it is possible to
 * optimize the performance of the leading-edge trigger or CFD trigger by adjusting the
 * fast-filter and CFD parameters offline. Such optimized parameters can then be saved to a
 * settings file to be used for online data acquisition.
 *
 * This function uses the globally defined configuration arrays to obtain the filter parameters.
 * These parameters need to be loaded from a configuration file using
 * Pixie16LoadDSPParametersFromFile. Users will also need to set the module's offline variant with
 * the function Pixie16SetOfflineVariant.
 *
 * @see Pixie16SaveExternalFIFODataToFile
 * @see Pixie16SetOfflineVariant
 * @see Pixie16LoadDSPParametersFromFile
 *
 * @param[in] FileName: Absolute path to a file containing list-mode data written by
 *     Pixie16SaveExternalFIFODataToFile
 * @param[in] ModuleNumber: The module number that we'd like to analyze. Numbering starts at 0.
 * @param[in] ChannelNumber: The channel that we'd like to analyze. Numbering starts at 0.
 * @param[in] FileLocation: The memory address of the beginning of the trace in the file.
 * @param[in] RcdTraceLength: The length of the trace in the file. Maximum of 32768.
 * @param[in] RcdTrace: The recorded trace that we'll be analyzing. Maximum length of 32768.
 * @param[out] slowfilter: Pointer to an array of 32-bit unsigned integers with length matching
 *    `RcdTraceLength`. Maximum length of 32768.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Null pointer *RcdTrace
 * @retval -2 - Null pointer *slowfilter
 * @retval -3 - Target module number is invalid
 * @retval -4 - Trace length is too short
 * @retval -5 - Failed to open list-mode data file
 * @retval -6 - Failed to seek to the trace in the file
 * @retval -7 - Failed to read the trace from the file
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ComputeSlowFiltersOffline(const char* FileName,
                                                                      unsigned short ModuleNumber,
                                                                      unsigned short ChannelNumber,
                                                                      unsigned int FileLocation,
                                                                      unsigned short RcdTraceLength,
                                                                      unsigned short* RcdTrace,
                                                                      double* slowfilter)
{

    FILE* ListModeFile = NULL;
    unsigned int SlowLen, SlowGap, SlowFilterRange, PreampTau_IEEE;
    unsigned int esum0[32768], esum1[32768], esum2[32768];
    unsigned int offset, x, y;
    double preamptau, deltaT;
    double b1, c0, c1, c2;
    unsigned int bsum0, bsum1, bsum2;
    double baseline;
    double coef_scaling_factor;

    // Check if RcdTrace is valid
    if (RcdTrace == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "Null pointer *RcdTrace");
        return (-1);
    }

    // Check if slowfilter is valid
    if (slowfilter == NULL) {
        Pixie_Print_Error(PIXIE_FUNC, "Null pointer *slowfilter");
        return (-2);
    }

    if (ModuleNumber >= PRESET_MAX_MODULES) {
        Pixie_Print_Error(PIXIE_FUNC, "Target module number is invalid %d", ModuleNumber);
        return (-3);
    }

    // Retrieve channel parameters

    SlowFilterRange = Pixie_Devices[ModuleNumber]
                              .DSP_Parameter_Values[SlowFilterRange_Address[ModuleNumber] - DATA_MEMORY_ADDRESS];
    SlowLen = Pixie_Devices[ModuleNumber]
                      .DSP_Parameter_Values[SlowLength_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS] *
              (unsigned int) pow(2.0, (double) SlowFilterRange);
    SlowGap = Pixie_Devices[ModuleNumber]
                      .DSP_Parameter_Values[SlowGap_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS] *
              (unsigned int) pow(2.0, (double) SlowFilterRange);
    PreampTau_IEEE =
            Pixie_Devices[ModuleNumber]
                    .DSP_Parameter_Values[PreampTau_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS];
    preamptau = IEEEFloating2Decimal(PreampTau_IEEE);

    // Scale up slow filter parameters for 250 MHz and 500 MHz modules
    if (Module_Information[ModuleNumber].Module_ADCMSPS == 250) {
        SlowLen *= 2;
        SlowGap *= 2;
    } else if (Module_Information[ModuleNumber].Module_ADCMSPS == 500) {
        SlowLen *= 5;
        SlowGap *= 5;
    }

    // Check if trace length is sufficiently long
    if (RcdTraceLength < ((2 * SlowLen + SlowGap) * 2)) {
        Pixie_Print_Error(PIXIE_FUNC, "the length of recorded trace is too short");
        return (-4);
    }

    // Open the list-mode file
    ListModeFile = fopen(FileName, "rb");
    if (ListModeFile != NULL) {
        int retval;

        // Position ListModeFile to the requested trace location
        retval = fseek(ListModeFile, FileLocation * 4, SEEK_SET);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failure seek listmode data, error=%d",
                              errno);
            (void) fclose(ListModeFile);
            return (-6);
        }

        // Read trace
        retval = Pixie16_fread(RcdTrace, 2, RcdTraceLength, ListModeFile);
        if (retval < 0) {
            Pixie_Print_Error(PIXIE_FUNC, "failure read listmode data, error=%d",
                              errno);
            (void) fclose(ListModeFile);
            return (-7);
        }

        // Close file
        (void) fclose(ListModeFile);

        //Assign coef_scaling_factor based on module's ADC bits
        if (Module_Information[ModuleNumber].Module_ADCBits == 12)
            coef_scaling_factor = 16.0;
        else if (Module_Information[ModuleNumber].Module_ADCBits == 14)
            coef_scaling_factor = 4.0;
        else if (Module_Information[ModuleNumber].Module_ADCBits == 16)
            coef_scaling_factor = 1.0;
        else {
            Pixie_Print_Error(PIXIE_FUNC, "invalid ADC number of bits");
            return (-4);
        }

        // Compute slow filter coefficients
        deltaT = 1.0 / ((double) Module_Information[ModuleNumber].Module_ADCMSPS);
        b1 = exp(-1.0 * deltaT / preamptau);
        c0 = -(1.0 - b1) * pow(b1, (double) SlowLen) * coef_scaling_factor / (1.0 - pow(b1, (double) SlowLen));
        c1 = (1.0 - b1) * coef_scaling_factor;
        c2 = (1.0 - b1) * coef_scaling_factor / (1.0 - pow(b1, (double) SlowLen));

        // Compute baseline
        bsum0 = 0;
        for (y = 0; y < SlowLen; y++) { bsum0 += RcdTrace[y]; }
        bsum1 = 0;
        for (y = SlowLen; y < (SlowLen + SlowGap); y++) { bsum1 += RcdTrace[y]; }
        bsum2 = 0;
        for (y = (SlowLen + SlowGap); y < (2 * SlowLen + SlowGap); y++) { bsum2 += RcdTrace[y]; }
        baseline = c0 * (double) bsum0 + c1 * (double) bsum1 + c2 * (double) bsum2;

        // Compute slow filter response
        offset = 2 * SlowLen + SlowGap - 1;
        for (x = offset; x < RcdTraceLength; x++) {
            esum0[x] = 0;
            for (y = (x - offset); y < (x - offset + SlowLen); y++) { esum0[x] += RcdTrace[y]; }
            esum1[x] = 0;
            for (y = (x - offset + SlowLen); y < (x - offset + SlowLen + SlowGap); y++) { esum1[x] += RcdTrace[y]; }
            esum2[x] = 0;
            for (y = (x - offset + SlowLen + SlowGap); y < (x - offset + 2 * SlowLen + SlowGap); y++) {
                esum2[x] += RcdTrace[y];
            }
            slowfilter[x] = c0 * (double) esum0[x] + c1 * (double) esum1[x] + c2 * (double) esum2[x] - baseline;
        }

        // Extend the value of slowfilter[offset] to all non-computed ones from index 0 to offset-1
        for (x = 0; x < offset; x++) { slowfilter[x] = slowfilter[offset]; }
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "can't open list-mode file %s", FileName);
        return (-5);
    }

    return (0);
}


/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Set a module's offline variant to be used for offline analysis
 *
 * This function sets the offline variant for the provided module number. This function **must**
 * be called before Pixie16InitSystem to ensure that the correct variant is set.
 *
 * @param[in] ModuleNumber: The module number that we want to set
 * @param[in] ModuleOfflineVariant: The variant of the module that we'd like to use for offline
 *     analysis.
 *     | Variant ID | Rev | SN | ADC Bits | ADC Sampling |
 *     |-|-|-|-|-|
 *     | 1 | 0xd | 275 | 12 | 100 |
 *     | 2 | 0xf | 1000 | 14 | 100 |
 *     | 3 | 0xf | 1000 | 12 | 250 |
 *     | 4 | 0xf | 1000 | 14 | 250 |
 *     | 5 | 0xf | 1000 | 12 | 500 |
 *     | 6 | 0xf | 1000 | 14 | 500 |
 *     | 7 | 0xf | 1000 | 16 | 250 |
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Module number is invalid
 * @retval -2 - module offline variant is invalid
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SetOfflineVariant(unsigned short ModuleNumber,
                                                              unsigned short ModuleOfflineVariant)
{
    //check if module number is valid
    if (ModuleNumber >= PRESET_MAX_MODULES) {
        Pixie_Print_Error(PIXIE_FUNC, "module number is invalid %d", ModuleNumber);
        return (-1);
    }

    //check if offline variant is valid
    if (ModuleOfflineVariant > 7) {
        Pixie_Print_Error(PIXIE_FUNC, "variant is invalid %d", ModuleOfflineVariant);
        return (-2);
    }

    Module_Information[ModuleNumber].Module_OfflineVariant = ModuleOfflineVariant;

    return (0);
}
