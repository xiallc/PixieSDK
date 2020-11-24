# Pixie SDK
Software used to access Pixie hardware using the PLX 9054 driver. Provides
user functions to perform common tasks on the hardware (ex. set parameters,
start runs, load firmware).

## Dependencies
* gcc/g++ 7+
* linux-kernel headers
* [Broadcom PCI/PCIe SDK](https://www.broadcom.com/products/pcie-switches-bridges/software-dev-kits) - See next section.
* Python 3.6+

### PLX SDK
This software uses the [Broadcom PLX 9054 driver](https://www.broadcom.com/products/pcie-switches-bridges/software-dev-kits) 
to communicate with the Pixie modules. Broadcom does not provide clear installation instructions for 
Linux systems. Here we'll provide instructions for Broadcom PCI/PCIe SDK v8.00 on CentOS 8. 

#### Linux Installation
1. [Download the v8.00 SDK](https://www.broadcom.com/products/pcie-switches-bridges/software-dev-kits#downloads) to your system.
2. Extract the tarball and move it to `/usr/local/broadcom/8.00`
   ```shell script
   [user@localhost ~]$ tar xzf Broadcom_PCI_PCIe_SDK_Linux_v8_00_Final_2019-07-30.tar.gz
   [user@localhost ~]$ sudo mkdir /usr/local/broadcom
   [user@localhost ~]$ sudo mv PlxSdk /usr/local/broadcom/8.00
   [user@localhost ~]$ sudo ln -s /usr/local/broadcom/8.00 /usr/local/broadcom/current
   ```
2. Export the `PLX_SDK_DIR` environment variable in `/etc/profile.d/broadcom.sh`
   ```shell script
   [user@localhost ~]$ echo -e "export PLX_SDK_DIR=/usr/local/broadcom/current\n" | sudo tee /etc/profile.d/broadcom.sh > /dev/null
   [user@localhost ~]$ source /etc/profile.d/broadcom.sh
   ```
2. Update `Include/Plx_sysdep.h::access_ok` to allow building on the CentOS modified Linux 4.18+ kernels.
   ```c
   /***********************************************************
    * access_ok
    *
    * access_ok() removed type param in 5.0
    **********************************************************/
   #if (LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0))
       #define Plx_access_ok                     access_ok
   #else
       #define Plx_access_ok(type,addr,size)     access_ok( (addr),(size) )
   #endif
   ```
2. Build the API library
   ```shell script
   [user@localhost ~]$ sudo make -C ${PLX_SDK_DIR}
   ```
3. Build the driver
   ```shell script
   [user@localhost ~]$ sudo -E PLX_CHIP=9054 make -C ${PLX_SDK_DIR}/Driver/
   ```
4. Load the kernel driver
    ```shell script
    [user@localhost ~]$ sudo -E ${PLX_SDK_DIR}/Bin/Plx_load 9054
    ```

## Installation
Our build system is (WAF)[https://waf.io]. It's python based and lightweight. The entire script lives with this 
repository so you don't have any dependencies other than Python 3.6+.

### Dependencies
* You have Python 3.5+ installed on your system.
* You have already installed the PLX drivers.
* You have `PLX_SDK_DIR` exported in your environment.

### Instructions
These installation instructions assume that you 
1. Clone the project from [https://git.xia.com/pixie/pixie_sdk.git](https://git.xia.com/pixie/pixie_sdk.git)
   ```shell script
   [user@localhost ~]$ git clone https://git.xia.com/pixie/pixie_sdk.git 
   ```
2. Navigate to the cloned project
    ```shell script
    [user@localhost ~]$ cd pixie_sdk
    ```
2. Configure and build the project
    ```shell script
    [user@localhost pixie_sdk]$ python3 waf configure build
    ```
2. (**Optional**) Install the software system wide. Defaults to `/usr/local/`
    ```shell script
    [user@localhost pixie_sdk]$ sudo python3 waf install
    ```
2. (**Optional**) Install the software to a user specified location
    ```shell script
    [user@localhost pixie_sdk]$ python3 waf configure install --prefix=/path/to/installation/location/
    ```

## Utility programs
We provide some basic programs that you can use to test the functionality of the API. These programs do not encompass
the full functionality of the API. They are **not recommended for production use**. Users may use these codes as a 
basis for developing their own applications.

[View the README.md](https://git.xia.com/pixie/pixie_sdk/-/blob/master/utilities/README.md) 

## Testing the software
TO BE FILLED IN LATER

### Procedure for making Pixie-16 testing programs
*NOTE:* Pixie-16 testing programs should ONLY be compiled with Pixie-16
static libraries.

1. Open a command line window, go to C:\Program Files\Microsoft Visual Studio 8\VC\bin, and then type 'vcvars32.bat' to set up VC tools environment.
2. Go to C:\path\to\project\root\software, and type 'make clean'
3. Go to C:\path\to\project\root\software\testing, and type 'rm *.exe'
4. Go back to C:\path\to\project\root\software, and type 'make tests'.

## Copyright
Copyright (c) 2005 - 2020, XIA LLC
All rights reserved.

## License
Redistribution and use in source and binary forms,
with or without modification, are permitted provided
that the following conditions are met:

  * Redistributions of source code must retain the above
    copyright notice, this list of conditions and the
    following disclaimer.
  * Redistributions in binary form must reproduce the
    above copyright notice, this list of conditions and the
    following disclaimer in the documentation and/or other
    materials provided with the distribution.
  * Neither the name of XIA LLC nor the names of its
    contributors may be used to endorse or promote
    products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
