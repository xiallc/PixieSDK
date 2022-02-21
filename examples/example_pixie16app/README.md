# example_pixie16app
`example_pixie16app` compiles and links against the Legacy C implementation. Most users will be
familiar with this implementation. 

## Deprecation notice
The Legacy C API is now deprecated and will no longer receive support outside critical bug fixes.
**We will remove the legacy C API on July 31, 2023**.

## Configuration file format

The configuration file format is a JSON file. The file contains a single array element, where each
element represents a module in the configuration. The software will automatically determine the
number of modules in the system according to the number of objects in the file. The order of the
objects defines the slot to module number mapping. For example, Slot 2 will map to Module 0.
On Windows environments the `\` in file paths should be escaped, see the example below.

```
[
  {
    "slot": <slot number>,
    "dsp": {
      "ldr": "<absolute or relative path to DSP LDR file>",
      "par": "<absolute or relative path to binary settings file>",
      "var": "<absolute or relative path to DSP VAR file>"
    },
    "fpga": {
      "fippi": "<absolute or relative path to FPGA FIPPI settings file>",
      "sys": "<absolute or relative path to FPGA SYS settings file>",
    }
  },
  <repeat above object with next module configruation>
]
```

### Example config

#### Linux

```json
[
  {
    "slot": 2,
    "dsp": {
      "ldr": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr",
      "par": "pixie.set",
      "var": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var"
    },
    "fpga": {
      "fippi": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/fippixie16_revfgeneral_16b250m_r36563.bin",
      "sys": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"
    }
  }
]
```

#### Windows

```json
[
  {
    "slot": 2,
    "dsp": {
      "ldr": "C:\\Program Files (x86)\\XIA\\Pixie16_VB 2.3.1\\DSP\\Pixie16DSP_revfgeneral_16b250m_r35921.ldr",
      "par": "pixie.set",
      "var": "C:\\Program Files (x86)\\XIA\\Pixie16_VB 2.3.1\\DSP\\Pixie16DSP_revfgeneral_16b250m_r35921.var"
    },
    "fpga": {
      "fippi": "C:\\Program Files (x86)\\XIA\\Pixie16_VB 2.3.1\\Firmware\\fippixie16_revfgeneral_16b250m_r36563.bin",
      "sys": "C:\\Program Files (x86)\\XIA\\Pixie16_VB 2.3.1\\Firmware\\syspixie16_revfgeneral_adc250mhz_r33339.bin",
      "trig": "FPGATrig"
    }
  }
]
```

## Usage Instructions

These instructions assume that you've built and installed PixieSDK into the default location.

### Linux

1. Create the directory
   ```shell script
   mkdir ~/pixie_sdk_example
   cd ~/pixie_sdk_example
   ```
2. Copy the sample configuration file from the repo
   ```shell script
   cp /usr/local/xia/PixieSdk/share/config/example_config.json .
   ```
3. Update the configuration file. You can name it whatever you want.
4. Execute the program to boot the modules
   ```shell script
   LD_LIBRARY_PATH=/usr/local/xia/PixieSDK/lib/ /usr/local/xia/PixieSDK/bin/example_pixie16app boot <name of config file>
   ```

### Windows

Windows will work similarly to Linux. You'll need to add the PixieSDK library and bin paths to
your `Path` variable.