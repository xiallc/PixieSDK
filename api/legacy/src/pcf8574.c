/**----------------------------------------------------------------------
* Copyright (c) 2020 - 2021, XIA LLC
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
/// @file pcf8574.c
/// @brief This file contains functions to access the I2C to Parallel-Port Expander
/// 		PCF8574 on Pixie-16. PCF8574 is connected to the 4-bit Rev ID and 4-bit
///         Slot ID of each slot of the crate.
/// @author H. Tan
/// @date June 29, 2020

#include "pixie16sys_common.h"
#include "pixie16sys_defs.h"
#include "pixie16sys_export.h"
#include "pixie16sys_globals.h"

/****************************************************************
 *	PCF8574_Read_One_Byte:
 *		Bus master reads one byte from PCF8574.
 *
 *		Return Value:
 *			 0 - Successful
 *			-1 - Invalid Pixie module number
 *			-2 - Failed to get Acknowledge after sending DevSel byte
 *			-3 - Failed to get Acknowledge after sending DevSel byte
 *
 ****************************************************************/

int PCF8574_Read_One_Byte (
        unsigned short ModNum,		// Pixie module number
        char *ByteValue )			// The byte value
{
    char IOByte;
    char ackvalue;

    // Check if ModNum is valid
    if(ModNum >= SYS_MAX_NUM_MODULES)
    {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid Pixie module number %d", ModNum);
        return(-1);
    }

    // Send "START"
    PCF8574_start(ModNum);

    // Send Device Select Code
    IOByte = (char)0x43;
    PCF8574_byte_write(ModNum, IOByte);

    // Get Acknowledge
    ackvalue = PCF8574_getACK(ModNum);
    if(ackvalue != 0)
    {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending DevSel byte");
        return(-2);
    }

    // Receive one byte
    PCF8574_byte_read(ModNum, ByteValue);

    Pixie_Print_Info(PIXIE_FUNC, "ByteValue read = %x", *ByteValue);

    // Send Acknowledge
    PCF8574_sendACK(ModNum);

    // Send "STOP"
    PCF8574_stop(ModNum);

    return(0);
}

/* ----------------------------------------------------- */
// PCF8574_start:
//   Bus master sends "START" to PCF8574
/* ----------------------------------------------------- */

int PCF8574_start(unsigned short ModNum)
{
    unsigned int buffer[4];

    //***************************
    //	Set SCL and SDA to 1
    //***************************

    buffer[0] = SDA | SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

    //***************************
    //	Set SDA to 0 while keep SCL at 1
    //***************************

    buffer[0] = SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

    return(0);
}


/* ----------------------------------------------------- */
// PCF8574_stop:
//   Bus master sends "STOP" to PCF8574
/* ----------------------------------------------------- */

int PCF8574_stop(unsigned short ModNum)
{
    unsigned int buffer[4];

    //***************************
    //	Set SCL to 1 and SDA to 0
    //***************************

    buffer[0] = SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

    //***************************
    //	Set SDA to 1 while keep SCL at 1
    //***************************

    buffer[0] = SDA | SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

    return(0);
}


/* ----------------------------------------------------- */
// PCF8574_byte_write:
//   Bus master sends a byte to PCF8574
/* ----------------------------------------------------- */

int PCF8574_byte_write(unsigned short ModNum, char ByteToSend)
{
    short i;
    unsigned int buffer[4];

    /* Initialize buffer[0] */

    buffer[0] = CTRL;

    for(i = 7; i >= 0; i --)
    {
        //***************************
        //	Set SCL to 0
        //***************************

        buffer[0] = (unsigned int)SYS16_ClrBit(1, (unsigned short)buffer[0]);
        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 6000 ns */
        wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

        //***************************
        //	Send bit i
        //***************************

        if(SYS16_TstBit((unsigned short)i, (unsigned short)ByteToSend) == 1)
            buffer[0] = SDA | CTRL;
        else
            buffer[0] = CTRL;

        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 1000 ns */
        wait_for_a_short_time((int)(1000.0 / (double)Ns_Per_Cycle));

        //***************************
        //	Set SCL to 1
        //***************************
        buffer[0] = SYS16_SetBit(1, (unsigned short)buffer[0]);
        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 6000 ns */
        wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));
    }

    //************************************************************
    //	Set SCL and CTRL to 0 to release bus for acknowledge
    //***********************************************************

    buffer[0] = (buffer[0] & 0x1);
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

    return(0);
}


/* ----------------------------------------------------- */
// PCF8574_byte_read:
//   Bus master receives a byte from PCF8574
/* ----------------------------------------------------- */

int PCF8574_byte_read(unsigned short ModNum, char *ByteToReceive)
{
    short i;
    unsigned int buffer[4];
    char ByteReceived;

    buffer[0] = 0x0;

    for(i = 7; i >= 0; i --)
    {
        //***************************
        //	Set SCL to 1
        //***************************

        buffer[0] = (unsigned int)SYS16_SetBit(1, (unsigned short)buffer[0]);
        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 6000 ns */
        wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

        //***************************
        //	Receive bit i
        //***************************

        buffer[0] = 0x0;

        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_READ, buffer);
        ByteReceived = (char)buffer[0];

        if(SYS16_TstBit(0, (unsigned short)ByteReceived) == 1)
            *ByteToReceive = (char)SYS16_SetBit((unsigned short)i, (unsigned short)*ByteToReceive);
        else
            *ByteToReceive = (char)SYS16_ClrBit((unsigned short)i, (unsigned short)*ByteToReceive);

        //***************************
        //	Set SCL to 0
        //***************************
        buffer[0] = (unsigned int)SYS16_ClrBit(1, (unsigned short)buffer[0]);
        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 6000 ns */
        wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));
    }

    return(0);
}


/* ----------------------------------------------------- */
// PCF8574_getACK:
//   Bus master receives ACKNOWLEDGE from PCF8574
//   keep CTRL = 0 to leave bus to memory for reading
/* ----------------------------------------------------- */

char PCF8574_getACK(unsigned short ModNum)
{
    unsigned int buffer[4], rbuf[4];
    char retval;

    //***************************
    //	Set SCL to 1
    //***************************

    buffer[0] = SCL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

    //***************************
    //	Read SDA
    //***************************
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_READ, rbuf);

    //***************************
    //	Set SCL to 0
    //***************************

    buffer[0] = 0x0;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

    retval = (char)(rbuf[0] & 0x1);
    return(retval);
}


/* ----------------------------------------------------- */
// PCF8574_sendACK:
//   Bus master sends ACKNOWLEDGE to PCF8574
/* ----------------------------------------------------- */

char PCF8574_sendACK(unsigned short ModNum)
{
    unsigned int buffer[4];

    //***************************
    //	Pull SDA down to LOW
    //***************************
    buffer[0] = CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 1000 ns */
    wait_for_a_short_time((int)(1000.0 / (double)Ns_Per_Cycle));

    //***************************
    //	Set SCL to 1 while keep SDA LOW
    //***************************

    buffer[0] = SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

    //***************************
    //	Set SCL to 0
    //***************************

    buffer[0] = 0x0;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int)(6000.0 / (double)Ns_Per_Cycle));

    return(0);
}
