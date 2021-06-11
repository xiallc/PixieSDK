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
/// @file pixie16sys.c
/// @brief This file contains all the Pixie16 library functions that can be accessed
/// 		externally by the user application library code.
/// @author H. Tan
/// @date July 31, 2006

#include <math.h>
#include <string.h>

#include <PlxApi.h>

#include "def21160.h"
#include "pixie16sys_common.h"
#include "pixie16sys_defs.h"
#include "pixie16sys_export.h"
#include "pixie16sys_globals.h"


#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
#include <windows.h>
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
#include <ctype.h>
#include <unistd.h>
#endif

/****************************************************************
*	Pixie_InitSystem:
*		Initialize Pixie16 system by mapping PXI slots to Pixie16
*		modules and assigning virtual addresses to the PLX9054 chips
*		of those modules.
*
*		Return Value:
*			 0 - Successful
*			-1 - Failed to measure host computer speed (ns per cycle)
*			-2 - Can't find any PLX device in the system
*           -3 - Can't find all Pixie-16 modules that were specified
*           -4 - Could not open a PCI Device when trying find all devices
*           -5 - Unable to map a PCI BAR into user virtual space
*           -6 - Could not read chassis slot number in which a module is installed
*           -7 - Failed to find PCI device when trying to read its chassis slot number
*			-8 - Unable to unmap the PCI BAR while closing those all opened PCI devices
*			-9 - Unable to close the PLX9054 device while closing those all opened PCI devices
*			-10- Can't match a module with one found by the PLX driver
*			-11- Could not open PCI Device after finishing mapping modules
*           -12- Could not close PCI device for all opened modules
*           -13- Could not find PCI device after finishing mapping modules
*           -20- Found an unsupported revision during initialization
*
****************************************************************/

int Pixie_InitSystem(unsigned short NumModules, unsigned short* PXISlotMap,
                     unsigned short OfflineMode) {
    PLX_DEVICE_KEY DeviceKey;
    PLX_STATUS rc;

    char sbuffer[100] = {0};   // Temporary buffer
    unsigned char PlxBusNum[SYS_MAX_NUM_MODULES], PlxDeviceNum[SYS_MAX_NUM_MODULES];
    unsigned char Bus_Number[SYS_MAX_NUM_MODULES] = { 0 };        // PCI bus number for each module
    unsigned char Device_Number[SYS_MAX_NUM_MODULES] = { 0 };     // PCI device number for each module
    unsigned char Slot_Number[SYS_MAX_NUM_MODULES];
    unsigned char TotalPlx9054Devices, PlxModIndex[SYS_MAX_NUM_MODULES], DeviceSearchCount;
    unsigned int i;
    unsigned short k, m;
    unsigned char pci_device_found_earlier;
    int retval;

    // First, update a few global variables
    SYS_Number_Modules = NumModules;  // the total number modules in the crate
    SYS_Offline = OfflineMode;        // SYS_Offline = 1: offline mode; SYS_Offline = 0: Online mode

    if (SYS_Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    // Measure host computer speed (ns per cycle)
    retval = get_ns_per_cycle(&Ns_Per_Cycle);
    if (retval < 0) {
        Pixie_Print_Error(PIXIE_FUNC,
                          "Failed to measure host computer speed (ns per cycle), retval = %d",
                          retval);
        return (-1);
    }

    Pixie_Print_Debug(PIXIE_FUNC, "Host computer speed (ns per cycle) = %f", Ns_Per_Cycle);

    // Find all the PLX devices installed in the system
    TotalPlx9054Devices = 0;
    pci_device_found_earlier = 0;
    DeviceSearchCount = 0;
    do {
        // Clear key structure to find first device
        memset(&DeviceKey, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));

        // Specify Vendor ID (PLX Vendor ID is 0x10b5)
        DeviceKey.VendorId = 0x10b5;
        // Specify Device ID (PLX Device ID is 0x9054)
        DeviceKey.DeviceId = 0x9054;

        rc = PlxPci_DeviceFind(&DeviceKey, DeviceSearchCount);
        if (rc == ApiSuccess) {
            // Check if the device has been found earlier
            if (TotalPlx9054Devices > 0) {
                // Clear flag prior to searching
                pci_device_found_earlier = 0;

                for (k = 0; k < TotalPlx9054Devices; k++) {
                    if ((DeviceKey.bus == PlxBusNum[k]) && (DeviceKey.slot == PlxDeviceNum[k])) {
                        pci_device_found_earlier = 1;
                    }
                }
            } else {
                // First found PLX device
                pci_device_found_earlier = 0;
            }

            // Only update PLX devices if not found previously
            if (pci_device_found_earlier == 0) {
                // Record PLX device information
                PlxBusNum[TotalPlx9054Devices] = DeviceKey.bus;
                PlxDeviceNum[TotalPlx9054Devices] = DeviceKey.slot;
                Pixie_Print_Debug(PIXIE_FUNC,
                                  "Device ID = 0x%04x, Vendor ID = 0x%04x, Bus Number = 0x%02x, Device number = 0x%02x",
                                  DeviceKey.DeviceId, DeviceKey.VendorId, DeviceKey.bus, DeviceKey.slot);
                // Increment found Plx9054 devices by 1
                TotalPlx9054Devices++;
            }

            // Increment device search count by 1
            DeviceSearchCount++;
        } else {
            break;
        }
    } while (1);

    if (TotalPlx9054Devices == 0) {
        Pixie_Print_Error(PIXIE_FUNC,
                          "Can't find any PLX devices, PlxPci_DeviceFind rc=%d",
                          rc);
        return (-2);
    }

    if (TotalPlx9054Devices < SYS_Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC,
                        "Can't find all Pixie-16 modules that were specified (#found = %d, #specified = %d)",
                        TotalPlx9054Devices, SYS_Number_Modules);
        return (-3);
    }

    // Open all found PLX9054 devices
    for (k = 0; k < TotalPlx9054Devices; k++) {
        // Clear key structure
        memset(&DeviceKey, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));

        // Specify Vendor ID (PLX Vendor ID is 0x10b5)
        DeviceKey.VendorId = 0x10b5;
        // Specify Device ID (PLX Device ID is 0x9054)
        DeviceKey.DeviceId = 0x9054;

        // Open each PLX9054 device
        rc = PlxPci_DeviceFind(&DeviceKey, k);
        if (rc == ApiSuccess) {
            rc = PlxPci_DeviceOpen(&DeviceKey, &SYS_hDevice[k]);
            if (rc != ApiSuccess) // Print error if failure
            {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "Could not open PCI Device Number (%d) at Bus Number %d; rc=%d",
                                  Device_Number[k], Bus_Number[k], rc);

                // Before return, we need to close those PCI devices that are already opened
                for (m = 0; m < k; m++) {
                    // Unmaps a previously mapped PCI BAR from user virtual space
                    rc = PlxPci_PciBarUnmap(&SYS_hDevice[m], (VOID**) &VAddr[m]);
                    if (rc != ApiSuccess) {
                        Pixie_Print_Error(PIXIE_FUNC,
                                          "Unable to unmap the PCI BAR for PLX9054 Device #%d; rc=%d",
                                          m, rc);
                    }

                    // Release the PLX device
                    rc = PlxPci_DeviceClose(&SYS_hDevice[m]);
                    if (rc != ApiSuccess) {
                        Pixie_Print_Error(PIXIE_FUNC,
                                          "Unable to close the PLX9054 device #%d; rc=%d",
                                          m, rc);
                    }
                }

                return (-4);
            } else {
                Pixie_Print_Debug(PIXIE_FUNC,
                                  "(Pixie_InitSystem): Successfully opened Device Number (%d) at Bus Number %d",
                                  DeviceKey.slot, DeviceKey.bus);
                Pixie_Print_Debug(PIXIE_FUNC,
                                  "Device ID = 0x%04x, Vendor ID = 0x%04x, Bus Number = 0x%02x, Device number = 0x%02x",
                                  DeviceKey.DeviceId, DeviceKey.VendorId, DeviceKey.bus, DeviceKey.slot);

                // Map a PCI BAR into user virtual space and return the virtual address
                // for the opened PCI device. For PLX 9054, Space 0 is at PCI BAR 2.
                rc = PlxPci_PciBarMap(&SYS_hDevice[k], 2, (VOID**) &VAddr[k]);
                if (rc != ApiSuccess) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "Unable to map a PCI BAR and obtain a virtual address for Device %d; rc=%d",
                                      k, rc);

                    // Before return, we need to close those PCI devices that are already opened
                    for (m = 0; m < k; m++) {
                        // Unmaps a previously mapped PCI BAR from user virtual space
                        rc = PlxPci_PciBarUnmap(&SYS_hDevice[m], (VOID**) &VAddr[m]);
                        if (rc != ApiSuccess) {
                            Pixie_Print_Error(PIXIE_FUNC,
                                              "Unable to unmap the PCI BAR for PLX9054 device #%d; rc=%d",
                                              m, rc);
                        }

                        // Release the PLX device
                        rc = PlxPci_DeviceClose(&SYS_hDevice[m]);
                        if (rc != ApiSuccess) {
                            Pixie_Print_Error(PIXIE_FUNC,
                                              "Unable to close the PLX9054 device #%d; rc=%d",
                                              m, rc);
                        }
                    }

                    return (-5);
                } else {
                    Pixie_Print_Debug(PIXIE_FUNC, "VAddr[%d][%d]=0x%lx", Bus_Number[k], k, VAddr[k]);
                }

                // Read the slot number in which this PLX9054 device is installed
                retval = PCF8574_Read_One_Byte(k, sbuffer);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "Could not read chassis slot number for Device %d; retval=%d",
                                      k, retval);

                    // Before return, we need to close those PCI devices that are already opened
                    for (m = 0; m < k; m++) {
                        // Unmaps a previously mapped PCI BAR from user virtual space
                        rc = PlxPci_PciBarUnmap(&SYS_hDevice[m], (VOID**) &VAddr[m]);
                        if (rc != ApiSuccess) {
                            Pixie_Print_Error(PIXIE_FUNC,
                                              "Unable to unmap the PCI BAR for PLX9054 device #%d; rc=%d",
                                              m, rc);
                        }

                        // Release the PLX device
                        rc = PlxPci_DeviceClose(&SYS_hDevice[m]);
                        if (rc != ApiSuccess) {
                            Pixie_Print_Error(PIXIE_FUNC,
                                              "Unable to close the PLX9054 device #%d; rc=%d",
                                              m, rc);
                        }
                    }

                    return (-6);
                } else {
                    Slot_Number[k] = (unsigned char) ((sbuffer[0] & 0xF8) / 8);
                }
                Pixie_Print_Debug(PIXIE_FUNC, "Device # %d Slot_Number = %d", k,
                                  Slot_Number[k]);
            }
        } else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "PlxPci_DeviceFind failed for PCI Device Number (%d) at Bus Number %d; rc=%d",
                              Device_Number[k], Bus_Number[k], rc);

            // Before return, we need to close those PCI devices that are already opened
            for (m = 0; m < k; m++) {
                // Unmaps a previously mapped PCI BAR from user virtual space
                rc = PlxPci_PciBarUnmap(&SYS_hDevice[m], (VOID**) &VAddr[m]);
                if (rc != ApiSuccess) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "Unable to unmap the PCI BAR for PLX9054 Device #%d; rc=%d",
                                      m, rc);
                }

                // Release the PLX device
                rc = PlxPci_DeviceClose(&SYS_hDevice[m]);
                if (rc != ApiSuccess) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "Unable to close the PLX9054 device #%d; rc=%d",
                                      m, rc);
                }
            }

            return (-7);
        }
    }

    // Close those all opened PCI devices
    for (m = 0; m < TotalPlx9054Devices; m++) {
        // Unmaps a previously mapped PCI BAR from user virtual space
        rc = PlxPci_PciBarUnmap(&SYS_hDevice[m], (VOID**) &VAddr[m]);
        if (rc != ApiSuccess) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Unable to unmap the PCI BAR for PLX9054 device #%d; rc=%d",
                              m, rc);
            return (-8);
        }

        // Release the PLX device
        rc = PlxPci_DeviceClose(&SYS_hDevice[m]);
        if (rc != ApiSuccess) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Unable to close the PLX9054 device #%d; rc=%d", m,
                              rc);
            return (-9);
        }
    }

    // Map user specified modules to modules found by PlxPci_DeviceFind using the matched slot number
    for (k = 0; k < SYS_Number_Modules; k++) {
        i = 0;
        PlxModIndex[k] = SYS_MAX_NUM_MODULES;
        do {
            if (((unsigned char) *(PXISlotMap + k)) == Slot_Number[i]) {
                PlxModIndex[k] = (unsigned char) i;
                break;
            }
            i++;
        } while (i < TotalPlx9054Devices);
        if (PlxModIndex[k] == SYS_MAX_NUM_MODULES) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Can't match module # %d with one found by the PLX driver",
                              k);
            return (-10);
        }
    }

    // Open PCI devices for all modules requested by the user
    for (k = 0; k < SYS_Number_Modules; k++) {
        // Clear key structure
        memset(&DeviceKey, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));

        // Specify Vendor ID (PLX Vendor ID is 0x10b5)
        DeviceKey.VendorId = 0x10b5;
        // Specify Device ID (PLX Device ID is 0x9054)
        DeviceKey.DeviceId = 0x9054;

        // Use the previously mapped PlxModIndex as the Device Number in the search
        rc = PlxPci_DeviceFind(&DeviceKey, PlxModIndex[k]);
        if (rc == ApiSuccess) {
            rc = PlxPci_DeviceOpen(&DeviceKey, &SYS_hDevice[k]);
            if (rc != ApiSuccess) // Print error if failure
            {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "Could not open PCI Device Number (%d) at Bus Number %d; rc=%d",
                                  Device_Number[k], Bus_Number[k], rc);

                // Before return, we need to close those PCI devices that are already opened
                for (m = 0; m < k; m++) {
                    retval = Pixie_ClosePCIDevices(m);
                    if (retval < 0) {
                        Pixie_Print_Error(PIXIE_FUNC,
                                          "Could not unmap PCI BAR for Module=%d; rc=%d",
                                          m, rc);
                    }
                }

                return (-11);
            } else {
                Pixie_Print_Debug(PIXIE_FUNC,
                                  "Successfully opened Device Number (%d) at Bus Number %d for Module # %d",
                                  Device_Number[k], Bus_Number[k], k);
                Pixie_Print_Debug(PIXIE_FUNC,
                                  "Device ID = 0x%04x, Vendor ID = 0x%04x, Bus Number = 0x%02x, Device Number = 0x%02x",
                                  DeviceKey.DeviceId, DeviceKey.VendorId, DeviceKey.bus, DeviceKey.slot);

                // Map a PCI BAR into user virtual space and return the virtual address
                // for the opened PCI device. For PLX 9054, Space 0 is at PCI BAR 2.
                rc = PlxPci_PciBarMap(&SYS_hDevice[k], 2, (VOID**) &VAddr[k]);
                if (rc != ApiSuccess) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "Unable to map a PCI BAR and obtain a virtual address for Module=%d; rc=%d",
                                      k, rc);

                    // Before return, we need to close those PCI devices that are already opened
                    for (m = 0; m < k; m++) {
                        retval = Pixie_ClosePCIDevices(m);
                        if (retval < 0) {
                            Pixie_Print_Error(PIXIE_FUNC,
                                              "Could not close PCI device for Module=%d; rc=%d",
                                              m, rc);
                        }
                    }

                    return (-12);
                } else {
                    Pixie_Print_Debug(PIXIE_FUNC, "VAddr[%d][%d]=0x%lx", Bus_Number[k], k, VAddr[k]);
                }
            }
        } else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "PlxPci_DeviceFind failed for PCI Device Number (%d) at Bus Number %d; rc=%d",
                              Device_Number[k], Bus_Number[k], rc);

            // Before return, we need to close those PCI devices that are already opened
            for (m = 0; m < k; m++) {
                retval = Pixie_ClosePCIDevices(m);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC,
                                      "Could not close PCI device for Module=%d; rc=%d",
                                      m, rc);
                }
            }

            return (-13);
        }
    }
    return (0);
}


/****************************************************************
*	Pixie_ClosePCIDevices:
*		Unmap PCI BAR and close PLX PCI devices.
*
*		Return Value:
*			 0 - successful
*			-1 - Unable to unmap the PCI BAR
*			-2 - Unable to close the PCI device
*
****************************************************************/

int Pixie_ClosePCIDevices(unsigned short ModNum) {
    PLX_STATUS rc;
    unsigned short k;

    if (SYS_Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    if (ModNum == SYS_Number_Modules)  // Close PCI devices in all modules
    {
        for (k = 0; k < SYS_Number_Modules; k += 1) {
            // Unmaps a previously mapped PCI BAR from user virtual space
            rc = PlxPci_PciBarUnmap(&SYS_hDevice[k], (VOID**) &VAddr[k]);
            if (rc != ApiSuccess) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "Unable to unmap the PCI BAR for module %d; rc=%d",
                                  k, rc);
                return (-1);
            }

            // Release the PLX device
            rc = PlxPci_DeviceClose(&SYS_hDevice[k]);
            if (rc != ApiSuccess) {
                Pixie_Print_Error(PIXIE_FUNC,
                                  "Unable to close the PLX device for module %d; rc=%d",
                                  k, rc);
                return (-2);
            }
        }
    } else if (ModNum < SYS_Number_Modules)  // Close PCI devices in only one module
    {
        // Unmaps a previously mapped PCI BAR from user virtual space
        rc = PlxPci_PciBarUnmap(&SYS_hDevice[ModNum], (VOID**) &VAddr[ModNum]);
        if (rc != ApiSuccess) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Unable to unmap the PCI BAR for module %d; rc=%d",
                              ModNum, rc);
            return (-1);
        }

        // Release the PLX device
        rc = PlxPci_DeviceClose(&SYS_hDevice[ModNum]);
        if (rc != ApiSuccess) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Unable to close the PLX device for module %d; rc=%d",
                              ModNum, rc);
            return (-2);
        }
    }

    return (0);
}


/****************************************************************
*	Pixie_Boot_CompFPGA:
*		Download system chip FPGA configuration.
*
*		Return Value:
*			 0 - boot successful
*			-2 - Clearing system FPGA MM timed out
*			-3 - Downloading system FPGA timed out
*
****************************************************************/

int Pixie_Boot_CompFPGA(unsigned short ModNum, unsigned int* Com_FPGA_conf,
                        unsigned int NumComFPGAconf) {
    unsigned int buffer[8];
    unsigned int k, counter0, counter1;

    if (SYS_Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    // Initialize counter0 to 0
    counter0 = 0;

    // Set communication FPGA Program*=0 to clear it
StartSys:
    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    buffer[0] &= 0xFFFFF000;
    buffer[0] |= 0x00000551;
    Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);

    // Set communication FPGA Program*=1 to start configuration
    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    buffer[0] &= 0xFFFFF000;
    buffer[0] |= 0x00000553;
    Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);

    // A very short wait - 200 us
    wait_for_a_short_time((int) (200000.0 / Ns_Per_Cycle));

    // Initialize counter1 to 0
    counter1 = 0;

ReadINIT:
    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    if ((buffer[0] & 0x01) != 0x01)  // if yes, finished clearing communication FPGA
    {
        counter1++;
        if (counter1 < 100) {
            // A very short wait - 200 us
            wait_for_a_short_time((int) (200000.0 / Ns_Per_Cycle));
            goto ReadINIT;
        } else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Clearing communication FPGA in module %d timed out.",
                              ModNum);
            return (-2);
        }
    } else {
        for (k = 0; k < NumComFPGAconf; k++) {
            // Downloading communication FPGA configuration
            Pixie_Register_IO(ModNum, CFG_DATACS, SYS_MOD_WRITE, &Com_FPGA_conf[k]);

            // A very short wait - 2 us
            wait_for_a_short_time((int) (2000.0 / Ns_Per_Cycle));
        }
    }

    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    if ((buffer[0] & 0x03) != 0x03)  // if yes, download communication FPGA successful
    {
        counter0++;
        if (counter0 < 10) {
            goto StartSys;
        } else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Downloading communication FPGA to module %d timed out",
                              ModNum);
            return (-3);
        }
    } else {
        Pixie_Print_Info(PIXIE_FUNC, "Downloaded communication FPGA successfully in module %d", ModNum);
    }

    return (0);
}


/****************************************************************
*	Pixie_Boot_FIPPI function:
*		Download RTPU FPGA (FIPPI).
*
*		Return Value:
*			0  - boot successful
*			-2 - Clearing FIPPI configuration timed out
*			-3 - Downloading FIP0&1 FPGA timed out
*
****************************************************************/

int
Pixie_Boot_FIPPI(unsigned short ModNum, unsigned int* SP_FPGA_conf, unsigned int NumSPFPGAconf) {
    unsigned int buffer[4];
    unsigned int k, counter0, counter1;

    if (SYS_Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    // Initialize counter0 to 0
    counter0 = 0;

    // Set all SP FPGA Chip Program*=0 to clear configuration
StartFip12:
    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    buffer[0] &= 0xFFFFF000;
    buffer[0] |= 0x00000052;  // Only booting FiPPI for 8 channels

    Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);

    // Set all SP FPGA Chip Program*=1 to start configuration
    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    buffer[0] &= 0xFFFFF000;
    buffer[0] |= 0x000000F2;

    Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);

    // A very short wait - 200 us
    wait_for_a_short_time((int) (200000.0 / Ns_Per_Cycle));

    // Initialize counter to 0
    counter1 = 0;

ReadINITf12:
    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    if ((buffer[0] & 0x53) != 0x53)  // if yes, finished clearing FIPPI configuration
    {
        counter1++;
        if (counter1 < 100) {
            Pixie_Print_Info(PIXIE_FUNC, "Clearing SP FPGAs 1&2, CFG_RDCS=0x%x", buffer[0]);

            // A very short wait - 200 us
            wait_for_a_short_time((int) (200000.0 / Ns_Per_Cycle));
            goto ReadINITf12;
        } else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Clearing FIPPI configuration 1&2 timed out in module %d",
                              ModNum);
            return (-2);
        }
    } else {
        for (k = 0; k < NumSPFPGAconf; k++) {
            Pixie_Register_IO(ModNum, CFG_DATACS, SYS_MOD_WRITE, &SP_FPGA_conf[k]);

            // A very short wait - 2 us
            wait_for_a_short_time((int) (2000.0 / Ns_Per_Cycle));
        }
    }

    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    if ((buffer[0] & 0xA3) != 0xA3)  // if yes, download all SP FPGA successful
    {
        counter0++;
        if (counter0 < 10) {
            Pixie_Print_Info(PIXIE_FUNC, "Downloading SP FPGAs 1&2, CFG_RDCS=0x%x",
                             buffer[0]);
            goto StartFip12;
        } else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Downloading SP FPGAs 1&2 timed out in module %d",
                              ModNum);
            return (-3);
        }
    } else {
        Pixie_Print_Info(PIXIE_FUNC, "Downloaded SP FPGAs 1&2 successfully in module %d", ModNum);
    }

    // Initialize counter0 to 0
    counter0 = 0;

    // Set all SP FPGA Chip Program*=0 to clear configuration
StartFip34:
    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    buffer[0] &= 0xFFFFF000;
    buffer[0] |= 0x000005A2;  // Temporarily only booting FiPPI for 8 channels

    // Write to an offset from the PCI-to-Local space window
    Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);

    // Set all SP FPGA Chip Program*=1 to start configuration
    buffer[0] &= 0xFFFFF000;
    buffer[0] |= 0x00000FA2;

    Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);

    // A very short wait - 200 us
    wait_for_a_short_time((int) (200000.0 / Ns_Per_Cycle));

    // Initialize counter to 0
    counter1 = 0;

ReadINITf34:
    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    if ((buffer[0] & 0x5A3) != 0x5A3)  // if yes, finished clearing FIPPI configuration
    {
        counter1++;
        if (counter1 < 100) {
            Pixie_Print_Info(PIXIE_FUNC, "Clearing SP FPGAs 3&4, CFG_RDCS=0x%x", buffer[0]);

            // A very short wait - 200 us
            wait_for_a_short_time((int) (200000.0 / Ns_Per_Cycle));
            goto ReadINITf34;
        } else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Clearing FIPPI configuration 3&4 timed out in module %d",
                              ModNum);
            return (-2);
        }
    } else {
        for (k = 0; k < NumSPFPGAconf; k++) {
            Pixie_Register_IO(ModNum, CFG_DATACS, SYS_MOD_WRITE, &SP_FPGA_conf[k]);

            // A very short wait - 2 us
            wait_for_a_short_time((int) (2000.0 / Ns_Per_Cycle));
        }
    }

    Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
    if ((buffer[0] & 0xAA3) != 0xAA3)  // if yes, download all SP FPGA successful
    {
        counter0++;
        if (counter0 < 10) {
            Pixie_Print_Info(PIXIE_FUNC, "Downloading SP FPGAs 3&4, CFG_RDCS=0x%x",
                             buffer[0]);
            goto StartFip34;
        } else {
            Pixie_Print_Error(PIXIE_FUNC,
                              "Downloading SP FPGAs 3&4 timed out in module %d",
                              ModNum);
            return (-3);
        }
    } else {
        Pixie_Print_Info(PIXIE_FUNC, "Downloaded SP FPGAs 3&4 successfully in module %d", ModNum);
    }

#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
    Sleep(10);
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
    usleep(10000);
#endif

    Pixie_Register_IO(ModNum, CFG_DCMRST, SYS_MOD_WRITE, buffer);  // reset DCM

    return (0);
}


/****************************************************************
*	Pixie_Boot_DSP:
*		Download DSP code.
*
*		Return Value:
*			 0 - boot DSP successful
*			-1 - Can not set timer resolution to 1 ms
*			-2 - Set DSP SYSCON in module failed
*			-3 - Set DSP DMAC10 in module failed
*			-4 - tagnum is invalid
*			-5 - DSP can't finish initialization on reset
*
****************************************************************/

int Pixie_Boot_DSP(unsigned short ModNum, unsigned int* DSP_code, unsigned int NumDSPCodeWords) {
    unsigned int buffer[0x5];  // Temporary buffer
    unsigned int i, count;  // Index and counters
    unsigned int tagnum, trytimes, wordcount, CSR;
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
    DWORD start;
#endif
    unsigned short bootsuccess, looptimes;

    if (SYS_Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
    // Set timer resolution to 1 ms
    if (timeBeginPeriod(1) != TIMERR_NOERROR) {
        Pixie_Print_Error(PIXIE_FUNC, "Can not set timer resolution to 1 ms");
        return (-1);
    }
#endif

    //********************************************
    // Try a limited number of times until the DSP
    // is booted successfully even though under normal
    // circumstances only one try will be sufficient
    //********************************************
    looptimes = 0;
    bootsuccess = 0;
    do {
        //********************************************
        // DSP codes download, with ready enabled,
        // no burst, 32 to 48 bits packing
        //********************************************

        // Reset DSP
        Pixie_ReadCSR(ModNum, &CSR);
        CSR = SYS32_SetBit(DSPDOWNLOAD, CSR);
        CSR = SYS32_SetBit(DSPRESET, CSR);
        Pixie_WrtCSR(ModNum, CSR);

        // Wait for 1 ms
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
        start = timeGetTime();
        while (timeGetTime() < (start + 1))
            ;
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
        usleep(1000);
#endif

        Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done for reset

        // Wait for 1 ms
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
        start = timeGetTime();
        while (timeGetTime() < (start + 1))
            ;
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
        usleep(1000);
#endif

        buffer[0] = SYSCON;
        Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);  // SYSCON address
        Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request

        // A short wait - 1 ms
        wait_for_a_short_time((int) (1000000.0 / Ns_Per_Cycle));

        trytimes = 0;

RetryS:

        Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_READ, buffer);  // read data from DSP SYSCON
        if (buffer[0] != 0x10) {
            // A short wait - 1 ms
            wait_for_a_short_time((int) (1000000.0 / Ns_Per_Cycle));

            trytimes++;
            if (trytimes > 5) {
                Pixie_Print_Error(PIXIE_FUNC, "Set DSP SYSCON in module #%d failed, SYSCON=0x%x", ModNum,
                                  buffer[0]);
                return (-2);
            }

            goto RetryS;
        } else {
            trytimes = 0;

RetryD:

            buffer[0] = DMAC10;
            Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);
            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_READ,
                              buffer);  // read data from DSP DMAC10
            if (buffer[0] != 0xA1) {
                trytimes++;
                if (trytimes > 5) {
                    Pixie_Print_Error(PIXIE_FUNC, "Set DSP DMAC10 in module #%d failed, DMAC10=0x%x", ModNum,
                                      buffer[0]);
                    return (-3);
                }

                goto RetryD;
            } else {
                // count will keep track the number of DSP_code words that have been downloaded
                count = 0;

                buffer[0] = EPB0;
                Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);  //  DSP EPB0

                // Download the boot kernel
                for (i = 0; i < 256; i++) {
                    Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                      &DSP_code[i * 3]);  // write to EPB0
                    Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                      &DSP_code[i * 3 + 1]);  // write to EPB0
                    Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                      &DSP_code[i * 3 + 2]);  // write to EPB0

                    count += 3;
                }

                Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // De-assert HBR
                do {
                    tagnum = DSP_code[count];

                    switch (tagnum) {

                        case ZERO_DM16:
                        case ZERO_PM16:
                        case ZERO_DM32:
                        case ZERO_PM32:
                        case ZERO_DM40:
                        case ZERO_PM40:
                        case ZERO_PM48:
                        case ZERO_DM64:
                        case ZERO_PM64:

                            Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                              buffer);  // HBR request

                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 1]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 2]);  // write to EPB0

                            Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

                            count += 3;
                            // wordcount shows the number of DSP_code word for this section
                            wordcount = DSP_code[count];

                            Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                              buffer);  // HBR request

                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 1]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 2]);  // write to EPB0

                            Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

                            count += 3;

                            // Wait for 2 ms
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
                            start = timeGetTime();
                            while (timeGetTime() < (start + 2))
                                ;
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
                            usleep(2000);
#endif

                            break;

                        case INIT_DM16:
                        case INIT_PM16:
                        case INIT_DM32:
                        case INIT_PM32:
                        case INIT_PM48:

                            Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                              buffer);  // HBR request

                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 1]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 2]);  // write to EPB0

                            Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

                            count += 3;
                            // wordcount shows the number of DSP_code word for this section
                            wordcount = DSP_code[count];

                            Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                              buffer);  // HBR request

                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 1]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 2]);  // write to EPB0

                            Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

                            count += 3;

                            for (i = 0; i < wordcount; i++) {

                                Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                                  buffer);  // HBR request

                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 3]);  // write to EPB0
                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 3 + 1]);  // write to EPB0
                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 3 + 2]);  // write to EPB0

                                Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE,
                                                  buffer);  // HBR done
                            }

                            count += 3 * wordcount;

                            break;

                        case INIT_DM64:
                        case INIT_PM64:

                            Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                              buffer);  // HBR request

                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 1]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 2]);  // write to EPB0

                            Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

                            count += 3;
                            // wordcount shows the number of DSP_code word for this section
                            wordcount = DSP_code[count];

                            Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                              buffer);  // HBR request

                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 1]);  // write to EPB0
                            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                              &DSP_code[count + 2]);  // write to EPB0

                            Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

                            count += 3;

                            for (i = 0; i < wordcount; i++) {

                                Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                                  buffer);  // HBR request

                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 6]);  // write to EPB0
                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 6 + 1]);  // write to EPB0
                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 6 + 2]);  // write to EPB0

                                Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE,
                                                  buffer);  // HBR done

                                Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                                  buffer);  // HBR request

                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 6 + 3]);  // write to EPB0
                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 6 + 4]);  // write to EPB0
                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 6 + 5]);  // write to EPB0

                                Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE,
                                                  buffer);  // HBR done
                            }

                            count += 6 * wordcount;

                            break;


                        case FINAL_INIT:

                            Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE,
                                              buffer);  // HBR request

                            for (i = 0; i < 258; i++) {
                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 3]);  // write to EPB0
                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 3 + 1]);  // write to EPB0
                                Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE,
                                                  &DSP_code[count + i * 3 + 2]);  // write to EPB0
                            }

                            count += 258 * 3;

                            Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

                            break;

                        default:
                            Pixie_Print_Error(PIXIE_FUNC, "tagnum=%u, check if it is valid", tagnum);
                            return (-4);
                    }

                } while (count < NumDSPCodeWords);
            }
        }

        //********************************************
        // Wait until DSP variable PowerUpInitDone is
        // set to 1 by the DSP after it completes its
        // initialization tasks on hard RESET. This
        // should prevent premature downloading of DSP
        // parameters by the host, which would cause
        // corruption of DSP internal memory.
        //********************************************
        trytimes = 0;
        do {
            Pixie_DSP_Memory_IO(buffer, POWERUPINITDONE_ADDRESS, 1, SYS_MOD_READ, ModNum);
            if (buffer[0] == 1) {
                bootsuccess = 1;
                break;
            }
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
            Sleep(1);
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
            usleep(1000);
#endif
            trytimes++;
        } while (trytimes < 10);
        if (bootsuccess == 0) {
            Pixie_Print_Error(PIXIE_FUNC,
                              "PowerUpInitDone stuck at 0 in the DSP of module %d - can't finish reset initialization",
                              ModNum);
        }

        looptimes++;
    } while ((bootsuccess == 0) && (looptimes < 10));

#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
    timeEndPeriod(1);
#endif

    if (bootsuccess == 1) {
        if (looptimes > 1) {
            Pixie_Print_Warning(PIXIE_FUNC, "After %d tries, downloaded DSP code in module #%d successfully",
                                looptimes, ModNum);
        } else {
            Pixie_Print_Info(PIXIE_FUNC, "Downloaded DSP code in module #%d successfully", ModNum);
        }
        return (0);
    } else {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to download DSP code in module #%d after %d tries", ModNum,
                          looptimes);
        return (-5);
    }
}
