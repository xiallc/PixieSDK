# Installation Instructions

These instructions primarily focus on the PixieSDK. We encourage all users to migrate to the latest
version of the library. For convenience, we provide a backward compatible Pixie16Api library. 

We will not be distributing any precompiled binaries for the time being. This is due to the extreme
variability of the compiler / architecture options for Windows.

There are a number of [CMake Options](cmake-options.md) available when generating the build 
system files. The defaults will always build the PixieSDK and the Pixie16Api compatibility library.

## Tested Systems
| Operating System | CMake Version | Compiler | Compiler Version |
|---|---|---|---|
| Windows 10 | 3.20.2 | MSVC | 14.28.29333 |
| Ubuntu 20.04 | 3.16.3 | GCC | 9.3.0 |
| CentOS 8 | 3.11.4 | GCC | 8.3.1 |

## Dependencies and Prerequisites

* Compiler with C++ STD 14+ support
* Cmake 3.10+
* [Broadcom PCI/PCIe SDK 8.00+](https://github.com/xiallc/broadcom_pci_pcie_sdk)
    * You have already installed the PLX drivers.
    * You have `PLX_SDK_DIR` exported in your environment.

> **Note**: The Legacy code can be used with much older compilers (ex. GCC 4.5+) and older versions
> of the Broadcom PCI/PCIe SDK (ex. 7.11). You'll still need CMake 3.10+ to generate the build files.

## Windows 10

At the moment we can only compile the 32-bit application on Windows. This is due to an issue
determining the appropriate system architecture, compiler architecture, and variability in
Broadcom's installation process.

### Instructions

## Linux

### Instructions

These installation instructions assume that you're using git to obtain a copy of the project. If 
you're starting with a zip folder from a release then skip to Step 2.

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
5. (**Optional**) Install the software system-wide. Defaults to `/usr/local/xia/PixieSDK`.
   ```shell script
   [user@localhost pixie_sdk]$ sudo make install
   ```
6. (**Optional**) Add the `XIA_PIXIE_SDK` environment variable (assumes bash) 
   ```shell
   [user@localhost pixie_sdk]$ echo -e "export XIA_PIXIE_SDK=/usr/local/xia/PixieSDK\n" | sudo tee -a /etc/profile.d/xia_pixie_sdk.sh
   ```