# Pixie SDK

[![Build Status](https://travis-ci.com/xiallc/pixie_sdk.svg?branch=master)](https://travis-ci.com/xiallc/pixie_sdk)

Software used to access Pixie hardware using the PLX 9054 driver. Provides user functions to perform
common tasks on the hardware (ex. set parameters, start runs, load firmware).

## Dependencies

* gcc/g++ 7+
* linux-kernel headers
* [Broadcom PCI/PCIe SDK](https://github.com/xiallc/broadcom_pci_pcie_sdk)
    * You have already installed the PLX drivers.
    * You have `PLX_SDK_DIR` exported in your environment.
* Cmake 3.11+

### Instructions

These installation instructions assume that you

1. Clone the project from [https://github.com/xiallc/pixie_sdk](https://github.com/xiallc/pixie_sdk)
   ```shell script
   [user@localhost ~]$ git clone https://github.com/xiallc/pixie_sdk.git
   ```
2. Navigate to the cloned project
   ```shell script
   [user@localhost ~]$ cd pixie_sdk
   ```
3. Configure and build the project
   ```shell script
   [user@localhost pixie_sdk]$ mkdir build; cd build; cmake ../
   ```
4. Compile the project
   ```shell
   [user@localhost pixie_sdk]$ make
   ```
5. (**Optional**) Install the software system wide. Defaults to `/usr/local/xia/PixieSDK`
   ```shell script
   [user@localhost pixie_sdk]$ sudo make install
   ```
6. (**Optional**) Install the software to a user specified location
   ```shell script
   [user@localhost pixie_sdk]$ cmake ../ -DCMAKE_INSTALL_PREFIX=/path/to/installation/location/; sudo make install
   ```

## Utility programs

We provide some basic programs that you can use to test the functionality of the API. These programs
do not encompass the full functionality of the API. They are **not recommended for production use**.
Users may use these codes as a basis for developing their own applications.

[View the README.md](https://github.com/xiallc/pixie_sdk/blob/master/utilities/README.md)

## Testing the software

We are in the process of building a robust test suite for the software. These tests are not
installed with the software as they're typically just to validate that everything installed
properly. There are some basic tests of the legacy APIs that run with the `legacy_unit_test_runner`.

## Copyright

Copyright (c) 2005 - 2021, XIA LLC All rights reserved.
