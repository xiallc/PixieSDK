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

/** @file tools.c
 * @brief This file contains several tool functions.
 * @note This file will be deprecated July 31, 2023
 */

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pixie16sys_common.h>
#include <pixie16sys_defs.h>
#include <pixie16sys_export.h>
#include <xia_common.h>

#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
#include <windows.h>
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#define SECOND 1000 * 1000 * 1000 /* nanoseconds/second */
#endif


#if PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI && defined(USE_USLEEP)
/****************************************************************
*	USleep:
*		Block for the specified number of nanoseconds.
*		nanoseconds is the the number of nanoseconds to sleep.
*
****************************************************************/

static void USleep(double nanoseconds) {
    // determine if we need to sleep for seconds
    uint32_t sec = (uint32_t)(nanoseconds / (SECOND));

    // determine the left over nanoseconds
    uint32_t remainder = nanoseconds - sec * SECOND;

    // convert to nanoseconds
    struct timespec sleepAmount = {sec, remainder};
    struct timespec sleepRemaining;

    // This loop deals with the case that nanosleep is interrupted by
    // a signal. In that case, sleepRemaining will be set to the unslept
    // time.
    while (nanosleep(&sleepAmount, &sleepRemaining) != 0) { sleepAmount = sleepRemaining; }
}
#endif


/****************************************************************
*	get_ns_per_cycle:
*		Measure the approximate speed of the host computer. It reports
*		how many nanoseconds are needed for executing one line of code.
*		This number is required for controlling the Firmware and DSP code
*		downloading.
*
*		Return Value:
*			 0 - Success
*			-1 - Failed to obtain timer resolution
*			-2 - Failed to set timer resolution to 1 ms
*			-3 - Minimum timer resolution is greater than 1 ms
*
****************************************************************/

PIXIE16SYS_EXPORT int PIXIE16SYS_API get_ns_per_cycle(double* ns_per_cycle) {
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
    TIMECAPS resolution;
    DWORD start, finish, duration;
    char ErrMSG[MAX_ERRMSG_LENGTH];
    unsigned int NumCycles;
    int count;
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI && (!defined(USE_USLEEP))
    struct timeval start_time, end_time;
    double start_count, end_count, duration;
    unsigned int NumCycles;
    int count;
#endif

#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
    if (timeGetDevCaps(&resolution, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
        sprintf(ErrMSG, "*ERROR* (get_ns_per_cycle): failed to obtain timer resolution");
        Pixie_Print_MSG(ErrMSG);
        return (-1);
    }

    if (resolution.wPeriodMin <= 1) {
        if (timeBeginPeriod(1) == TIMERR_NOERROR) {

            NumCycles = 100000000;
            count = NumCycles;

            start = timeGetTime();

            do { count--; } while (count >= 0);

            finish = timeGetTime();

            duration = finish - start;

            *ns_per_cycle = ceil((double) duration / (double) NumCycles * 1.0e6);

            timeEndPeriod(1);
        } else {
            sprintf(ErrMSG, "*ERROR* (get_ns_per_cycle): failed to set timer resolution to 1 ms");
            Pixie_Print_MSG(ErrMSG);
            return (-2);
        }
    } else {
        sprintf(ErrMSG, "*ERROR* (get_ns_per_cycle): minimum timer resolution is greater than 1 ms");
        Pixie_Print_MSG(ErrMSG);
        return (-3);
    }
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI

    // There are two options in Linux for returning the ns_per_cycle:
    //   (1) Use gettimeofday function to directly compute it and then wait
    //       for the number of cycles directly;
    //   (2) Directly return it as NSMULTIPLIER and then use USleep function
    //       for the wait.

#ifndef USE_USLEEP  // USleep function will NOT be used

    // initialize the counter
    NumCycles = 100000000;
    count = NumCycles;

    // record the start time
    gettimeofday(&start_time, NULL);

    do { count--; } while (count >= 0);

    // record the end time
    gettimeofday(&end_time, NULL);

    start_count = (double) start_time.tv_sec + 1.0e-6 * (double) start_time.tv_usec;
    end_count = (double) end_time.tv_sec + 1.0e-6 * (double) end_time.tv_usec;
    duration = end_count - start_count;

    // return ns_per_cycle with a multiplication factor (NSMULTIPLIER), which is defined
    // in pixie16sys_defs.h and can be modified to accommodate different booting timing
    // requirements in different Linux systems.

    *ns_per_cycle = NSMULTIPLIER * ceil((double) duration / (double) NumCycles * 1.0e9);

#else  // USleep function will be used

    *ns_per_cycle = NSMULTIPLIER;

#endif  // End of USE_USLEEP check

#endif  // End of PIXIE16_LINUX_SYSAPI

    return (0);
}


/****************************************************************
*	wait_for_a_short_time:
*		A shot wait (the time for each cycle = ns_per_cycle).
*
****************************************************************/

PIXIE16SYS_EXPORT void PIXIE16SYS_API wait_for_a_short_time(int cycles) {

#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI

    do { cycles--; } while (cycles >= 0);

#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI


    // There are two options for this wait_for_a_short_time in Linux:
    //   (1) wait for the number of cycles directly;
    //   (2) use USleep function for the wait.

#ifndef USE_USLEEP  // USleep function will NOT be used

    do { cycles--; } while (cycles >= 0);

#else  // USleep function will be used

    USleep((double) cycles);

#endif  // End of USE_USLEEP check

#endif  // End of PIXIE16_LINUX_SYSAPI
}


/****************************************************************
*	SYS16_SetBit:
*		Set a bit in a 16-bit unsigned integer.
*
****************************************************************/

PIXIE16SYS_EXPORT unsigned short PIXIE16SYS_API SYS16_SetBit(unsigned short bit, unsigned short value) {
    if(bit > 15)
        return value;
    return (value | (unsigned short) (pow(2.0, (double) bit)));
}


/****************************************************************
*	SYS16_ClrBit:
*		Clear a bit in a 16-bit unsigned integer.
*
****************************************************************/

PIXIE16SYS_EXPORT unsigned short PIXIE16SYS_API SYS16_ClrBit(unsigned short bit, unsigned short value) {
    if(bit > 15)
        return value;
    value = SYS16_SetBit(bit, value);
    return (value ^ (unsigned short) (pow(2.0, (double) bit)));
}


/****************************************************************
*	SYS16_TstBit:
*		Test a bit in a 16-bit unsigned integer.
*
****************************************************************/

PIXIE16SYS_EXPORT unsigned short PIXIE16SYS_API SYS16_TstBit(unsigned short bit, unsigned short value) {
    if(bit > 15)
        return FALSE_;
    return (((value & (unsigned short) (pow(2.0, (double) bit))) >> bit));
}


/****************************************************************
*	SYS32_SetBit:
*		Set a bit in a 32-bit unsigned integer.
*
****************************************************************/

PIXIE16SYS_EXPORT unsigned int PIXIE16SYS_API SYS32_SetBit(unsigned short bit, unsigned int value) {
    if(bit > 31)
        return value;
    return (value | (unsigned int) (pow(2.0, (double) bit)));
}


/****************************************************************
*	SYS32_ClrBit:
*		Clear a bit in a 32-bit unsigned integer.
*
****************************************************************/

PIXIE16SYS_EXPORT unsigned int PIXIE16SYS_API SYS32_ClrBit(unsigned short bit, unsigned int value) {
    if(bit > 31)
        return value;
    value = SYS32_SetBit(bit, value);
    return (value ^ (unsigned int) (pow(2.0, (double) bit)));
}


/****************************************************************
*	SYS32_TstBit:
*		Test a bit in a 32-bit unsigned integer.
*
****************************************************************/

PIXIE16SYS_EXPORT unsigned int PIXIE16SYS_API SYS32_TstBit(unsigned short bit, unsigned int value) {
    if(bit > 31)
        return FALSE_;
    return (((value & (unsigned int) (pow(2.0, (double) bit))) >> bit));
}

/****************************************************************
*	Pixie_Print_MSG:
*		Print messages to a text file called "Pixie16msg.txt".
*
*		Return Value:
*			 0 - Successful
*			-1 - Can't open the message file
*
****************************************************************/

static int Pixie_Print_Buffer(char* message) {
    time_t rawtime;
    struct tm* timeinfo;
    FILE* Pixie16msg = NULL;

    // Get current date and time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Append current date and time
    strcat(message, "\t\t[");
    strncat(message, asctime(timeinfo), strlen(asctime(timeinfo)) - 1);
    strcat(message, "]\n");

    // Write to file
    Pixie16msg = fopen("Pixie16msg.txt", "a");
    if (Pixie16msg == NULL) {
        return (-1);
    }

    fprintf(Pixie16msg, "%s", message);
    fclose(Pixie16msg);

    return (0);
}

static void Pixie_Print_VMSG(const char* format, va_list args) {
    char formatBuffer[MAX_ERRMSG_LENGTH];
    vsnprintf(formatBuffer, sizeof(formatBuffer), format, args);
    formatBuffer[sizeof(formatBuffer) - 1] = '\0';
    (void) Pixie_Print_Buffer(formatBuffer);
}

void Pixie_Print_MSG(const char* format, ...) {
    va_list args;
    va_start(args, format);
    Pixie_Print_VMSG(format, args);
    va_end(args);
}

void Pixie_Print_Type_MGS(const char* type, const char* func, char* format, va_list args) {
    char formatBuffer[MAX_ERRMSG_LENGTH];
    size_t len;
    len = snprintf(formatBuffer, sizeof(formatBuffer), "%s (%s): ", type, func);
    vsnprintf(formatBuffer + len, sizeof(formatBuffer) - len, format, args);
    formatBuffer[sizeof(formatBuffer) - 1] = '\0';
    (void) Pixie_Print_Buffer(formatBuffer);
}

void Pixie_Print_Error(const char* func, char* format, ...) {

    va_list args;
    va_start(args, format);
    Pixie_Print_Type_MGS("*ERROR*", func, format, args);
    va_end(args);
}

void Pixie_Print_Warning(const char* func, char* format, ...) {

    va_list args;
    va_start(args, format);
    Pixie_Print_Type_MGS("WARNING", func, format, args);
    va_end(args);
}

void Pixie_Print_Info(const char* func, char* format, ...) {
    va_list args;
    va_start(args, format);
    Pixie_Print_Type_MGS("info", func, format, args);
    va_end(args);
}

void Pixie_Print_Debug(const char* func, char* format, ...) {
#if (PRINT_DEBUG_MSG == 1)
    va_list args;
    va_start(args, format);
    Pixie_Print_Type_MGS("debug", func, format, args);
    va_end(args);
#endif
}
