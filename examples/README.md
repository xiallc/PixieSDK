# PixieSDK - Example Codes

We provide some basic programs that you can use to test the functionality of the API. These programs
do not encompass the full functionality of the API. They are **not recommended for production use**.
Users may use these codes as a basis for developing their own applications.

## pixie_sdk_example.cpp

This sample C++ base code can be used to test basic functionality to Pixie modules. At the moment
users can boot modules and take list mode data. We'll walk you through the basics of using this
program below. If you need help with a particular command, then you can use `pixie_sdk_example -h`
or `pixie_sdk_example COMMAND -h` to get more information about how to use the commands.

These instructions assumes

1. that you've built and installed PixieSDK into the default location
2. and that you're on a linux environment.

Windows will work similarly except that you'll need to add the library path to your `PATH` variable.

1. Prepare the execution directory
    1. Create the directory
       ```shell script
       mkdir ~/pixie_sdk_example
       cd ~/pixie_sdk_example
       ```
    1. Copy the sample configuration file from the repo
        ```shell script
        cp /usr/local/xia/PixieSdk/share/config/cfgPixie16.txt .
        ```
    2. Update the configuration file (see below). You can name it whatever you want.
1. Execute the program to boot the modules
   ```shell script
   LD_LIBRARY_PATH=/usr/local/xia/PixieSDK/lib/ /usr/local/xia/PixieSDK/bin/pixie_sdk_example boot <name of config file>
   ```

### Configuration file format

```
<Number of Modules you want to boot>
<The Crate slot for the first module>
<The Crate slot for the second module>
...
<The Crate slot for the last module>
/absolute/path/to/ComFPGAConfigFile
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
/home/xiauser/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin
/home/xiauser/firmware/fippixie16_revfgeneral_16b250m_r36563.bin
FPGATrig
/home/xiauser/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr
/home/xiauser/configuration/test_ena_cfd_trace_qdc_esums.set
/home/xiauser/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var
```

## utilities/pixie_sdk_example.py

This is a quick POC for interfacing with PixieSDK via python. This is the sole reason that we'll
likely keep around a C interface to the SDK. It's easier to interface using ctypes as opposed to
something like SWIG.

## Copyright

Copyright (c) 2005 - 2021, XIA LLC All rights reserved.

## License

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and
  the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
  and the following disclaimer in the documentation and/or other materials provided with the
  distribution.
* Neither the name of XIA LLC nor the names of its contributors may be used to endorse or promote
  products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
