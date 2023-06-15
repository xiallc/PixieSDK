Pixie-16 C API Example
######################

``example_pixie16api`` compiles and links against ``Pixie16Api.so``.
This library provides users a C wrapper to the ``PixieSDK``. We also
provide a python example that uses the same configuration file and
library.

Configuration file
******************

File Format
===========

The configuration file uses
`JSON <https://www.json.org/json-en.html>`__. This file format is ideal
as it provides us a semi-structured format that allows expansion without
impacting older versions.

Slot Definition Order
=====================

The software will automatically determine the number of requested
modules in the system according to the number of objects in the array.
Note that this is separate from the process that the SDK uses to
identify the PLX 9054 devices present. The order of the objects defines
the slot to module number mapping. For example, Slot 2 will map to
Module 0.

Module definitions
==================

Each requested module must have a JSON array element. These elements
contain the information necessary for the SDK to fully initialize and
boot the hardware.

+-----------------+-----------------+-----------------+-----------------+
| Name            | Description     | Elements        | Required?       |
+=================+=================+=================+=================+
| slot            | Defines the     | N/A             | Yes             |
|                 | physical slot   |                 |                 |
|                 | this module     |                 |                 |
|                 | occupies        |                 |                 |
+-----------------+-----------------+-----------------+-----------------+
| dsp             | Defines the DSP | ldr, var, par   | Yes             |
|                 | firmware and    |                 |                 |
|                 | settings file   |                 |                 |
|                 | to load to the  |                 |                 |
|                 | hardware.       |                 |                 |
+-----------------+-----------------+-----------------+-----------------+
| fpga            | Defines the     | fippi, sys      | Yes             |
|                 | FPGA firmware   |                 |                 |
|                 | to load to the  |                 |                 |
|                 | hardware.       |                 |                 |
+-----------------+-----------------+-----------------+-----------------+
| fw              | Used during     | version,        | Yes, if using   |
|                 | parallel        | revision,       | parallel boot   |
|                 | booting to      | adc_msps,       | functionality   |
|                 | associated a    | adc_bits        | or python       |
|                 | registered      |                 | example.        |
|                 | firmware with   |                 |                 |
|                 | the module      |                 |                 |
+-----------------+-----------------+-----------------+-----------------+
| worker          | Used to setup   | bandwi          | No              |
|                 | the module’s    | dth_mb_per_sec, |                 |
|                 | list-mode       | buffers,        |                 |
|                 | worker FIFO     | dma_trigg       |                 |
|                 | configuration   | er_level_bytes, |                 |
|                 |                 | hold_usecs,     |                 |
|                 |                 | i               |                 |
|                 |                 | del_wait_usecs, |                 |
|                 |                 | run_wait_usecs  |                 |
+-----------------+-----------------+-----------------+-----------------+

Example config
==============

The same configuration file is used on both Linux and Windows systems.
Please note that on Windows that ``\`` characters in paths need to be
escaped. For example

::

   "C:\\Program Files (x86)\\XIA\\Pixie16_VB 2.3.1\\DSP\\Pixie16DSP_revfgeneral_16b250m_r35921.ldr"

Base format
-----------

.. code:: json

   [
     {
       "slot": "<slot number>",
       "dsp": {
         "ldr": "<absolute or relative path to DSP LDR file>",
         "par": "<absolute or relative path to binary settings file>",
         "var": "<absolute or relative path to DSP VAR file>"
       },
       "fpga": {
         "fippi": "<absolute or relative path to FPGA FIPPI settings file>",
         "sys": "<absolute or relative path to FPGA SYS settings file>"
       }
     }
   ]

Single module configuration
---------------------------

.. code:: json

   [
     {
       "slot": 2,
       "dsp": {
         "ldr": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr",
         "par": "pixie.json",
         "var": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var"
       },
       "fpga": {
         "fippi": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/fippixie16_revfgeneral_16b250m_r36563.bin",
         "sys": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"
       }
     }
   ]

Multi-module configuration w/ parallel boot
-------------------------------------------

.. code:: json

   [
     {
       "slot": 2,
       "dsp": {
         "ldr": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr",
         "par": "pixie.json",
         "var": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var"
       },
       "fpga": {
         "fippi": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/fippixie16_revfgeneral_16b250m_r36563.bin",
         "sys": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"
       },
       "fw": {
         "version": 35921,
         "revision": 15,
         "adc_msps": 250,
         "adc_bits": 16
       }
     },
     {
       "slot": 4,
       "dsp": {
         "ldr": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr",
         "par": "pixie.json",
         "var": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var"
       },
       "fpga": {
         "fippi": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/fippixie16_revfgeneral_16b250m_r36563.bin",
         "sys": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"
       },
       "fw": {
         "version": 35921,
         "revision": 15,
         "adc_msps": 250,
         "adc_bits": 16
       }
     }
   ]

Usage Instructions
******************

These instructions assume that you’ve built and installed PixieSDK into the
default location.

Linux
=====

1. Create the directory

.. code:: shell

   mkdir ~/pixie_sdk_example && cd ~/pixie_sdk_example

2. Copy the sample configuration file from the repo

.. code:: shell

   cp /usr/local/xia/PixieSDK/share/config/example_config.json .

3. Update the configuration file. You can name it whatever you want.
4. Execute the program to boot the modules

.. code:: shell

   LD_LIBRARY_PATH=/usr/local/xia/PixieSDK/lib/ /usr/local/xia/PixieSDK/bin/example_pixie16api boot <name of config file>

Windows
=======

Windows will work similarly to Linux. You’ll need to add the PixieSDK library
and bin paths to your ``Path`` variable.
