# Pixie SDK
[![Build Status](https://travis-ci.com/xiallc/pixie_sdk.svg?branch=master)](https://travis-ci.com/xiallc/pixie_sdk)

Software used to access Pixie hardware using the PLX 9054 driver. Provides
user functions to perform common tasks on the hardware (ex. set parameters,
start runs, load firmware).

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
2. Configure and build the project
    ```shell script
    [user@localhost pixie_sdk]$ mkdir build; cd build; cmake ../
    ```
2. Compile the project
   ```shell
   [user@localhost pixie_sdk]$ make
   ```
2. (**Optional**) Install the software system wide. Defaults to `/usr/local/xia/pixie_sdk`
    ```shell script
    [user@localhost pixie_sdk]$ sudo make install
    ```
2. (**Optional**) Install the software to a user specified location
    ```shell script
    [user@localhost pixie_sdk]$ cmake ../ -DCMAKE_INSTALL_PREFIX=/path/to/installation/location/; sudo make install
    ```

## Utility programs
We provide some basic programs that you can use to test the functionality of the API. These programs do not encompass
the full functionality of the API. They are **not recommended for production use**. Users may use these codes as a
basis for developing their own applications.

[View the README.md](https://git.xia.com/pixie/pixie_sdk/-/blob/master/utilities/README.md)

## Testing the software
We are in the process of building a robust test suite for the software. These tests are not installed
with the software as they're typically just to validate that everything installed properly. 
There are some basic tests of the legacy APIs that run with the `legacy_unit_test_runner`. 

## Copyright
Copyright (c) 2005 - 2021, XIA LLC
All rights reserved.

## License
Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this 
    list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice, 
    this list of conditions and the following disclaimer in the documentation 
    and/or other materials provided with the distribution.
  * Neither the name of XIA LLC nor the names of its contributors may be used 
    to endorse or promote products derived from this software without specific 
    prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
