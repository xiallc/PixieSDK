## Introduction
We provide some basic programs that you can use to test the functionality of the API. These programs do not encompass
the full functionality of the API. They are **not recommended for production use**. Users may use these codes as a 
basis for developing their own applications.

## utilities/pixie.cpp
This sample C++ base code can be used to test basic functionality to Pixie modules. At the moment users can boot modules
and take list mode data. Due to the API design you will need to have a `pxisys.ini` file in the same directory that 
you execute the function. We'll walk you through the basics of using this program below. If you need help with a 
particular command, then you can use `pixie -h` or `pixie COMMAND -h` to get more information about how to use the commands.

**NOTE**: Make sure that you replace `${INSTALL_PATH}` with the actual installation path. Ex. `/tmp`  

1. Ensure that you have built and installed the program.
   ```shell script
   [user@localhost pixie_sdk]$ python3 waf configure build --utilities install --prefix=INSTALL_PATH
   ```
1. In your home directory create a test folder
   ```shell script
   [user@localhost ~]$ mkdir pixie_sdk_tests && cd pixie_sdk_tests
   ```
1. Link to the correct `pxisys.ini` file.
   ```shell script
   [user@localhost pixie_sdk_tests]$ ln -s INSTALL_PATH/xia/pixie-sdk/share/pxi/pxisys_14slot_wiener_pxie.ini pxisys.ini
   ```
1. Copy the sample configuration file from the installation location
   ```shell script
   [user@localhost pixie_sdk_tests]$ cp INSTALL_PATH/xia/pixie-sdk/share/config/cfgPixie16.txt .
   ```
1. Update the configuration file (see below). You can name it whatever you want.
1. Execute the program
   ```shell script
   [user@localhost pixie_sdk_tests]$ LD_LIBRARY_PATH=${INSTALL_PATH}/xia/pixie-sdk/lib/ ${INSTALL_PATH}/xia/pixie-sdk/bin/pixie -c <name of config file>
   ```

### Configuration file format
```
<Number of Modules you want to boot>
<The Crate slot for the first module>
<The Crate slot for the second module>
...
<The Crate slot for the last module>
/relative/path/to/ComFPGAConfigFile
/relative/path/to/SPFPGAConfigFile
/relative/path/to/TrigFPGAConfigFile
/relative/path/to/DSPCodeFile
/relative/path/to/DSPParFile
/relative/path/to/DSPVarFile
```
#### Example config
```
1
5
/home/stan/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin
/home/stan/firmware/fippixie16_revfgeneral_16b250m_r36563.bin
FPGATrig
/home/stan/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr
/home/stan/configuration/test_ena_cfd_trace_qdc_esums.set
/home/stan/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var
```

## utilities/pixie.py


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
