# Pixie SDK
Software used to access Pixie hardware using the PLX 9054 driver. Provides
user functions to perform common tasks on the hardware (ex. set parameters,
start runs, load firmware).

## Prerequisites
### General
* gcc/g++
* linux-kernel headers


### PLX SDK
This software uses the [Broadcom PLX 9054 driver](https://www.broadcom.com/products/pcie-switches-bridges/software-dev-kits) 
to communicate with the Pixie modules. Broadcom does not provide clear installation instructions for 
Linux systems. Here we'll provide instructions for Broadcom PCI/PCIe SDK v8.00 on CentOS 8. 

#### Linux Installation
1. [Download the v8.00 SDK](https://www.broadcom.com/products/pcie-switches-bridges/software-dev-kits#downloads) to your system.
2. Extract the tarball and move it to `/usr/local/broadcom/8.00`
   ```
   tar xzf Broadcom_PCI_PCIe_SDK_Linux_v8_00_Final_2019-07-30.tar.gz
   sudo mkdir /usr/local/broadcom
   sudo mv PlxSdk /usr/local/broadcom/8.00
   sudo ln -s /usr/local/broadcom/8.00 /usr/local/broadcom/current
   ```
2. Export the `PLX_SDK_DIR` environment variable in `/etc/profile.d/broadcom.sh`
   ```
   echo -e "export PLX_SDK_DIR=/usr/local/broadcom/current\n" | sudo tee /etc/profile.d/broadcom.sh > /dev/null
   source /etc/profile.d/broadcom.sh
   ```
2. Update `Include/Plx_sysdep.h::access_ok` to allow building on the CentOS modified Linux 4.18+ kernels.
   ```
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
   ```
    sudo make -C ${PLX_SDK_DIR}
   ```
3. Build the driver
   ```
   sudo -E PLX_CHIP=9054 make -C ${PLX_SDK_DIR}/Driver/
   ```
4. Load the kernel driver
    ```
    sudo -E ${PLX_SDK_DIR}/Bin/Plx_load 9054
    ```

## Procedure for making Pixie-16 testing programs
*NOTE:* Pixie-16 testing programs should ONLY be compiled with Pixie-16
static libraries.

1. Open a command line window, go to C:\Program Files\Microsoft Visual Studio 8\VC\bin, and then type 'vcvars32.bat' to set up VC tools environment.
2. Go to C:\path\to\project\root\software, and type 'make clean'
3. Go to C:\path\to\project\root\software\testing, and type 'rm *.exe'
4. Go back to C:\path\to\project\root\software, and type 'make tests'.
