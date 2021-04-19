"""
Copyright (c) 2005 - 2021, XIA LLC
All rights reserved.

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

file: pixie.py
brief: 
author: S. V. Paulauskas
date: November 16, 2020
"""
from argparse import ArgumentParser
import ctypes

import yaml


def calculate_num_modules(slot_map):
    """
    Reads the slot map and counts the number of modules we have in total
    :param slot_map: The Slot map containing the number of modules.
    :return: The number of modules counted in the config.
    """
    return sum([len(v) for v in slot_map.values()])


def initialize_modules(cfg, sdk):
    """
    Initializes the modules according to the information provided in the configuration file.
    :param cfg: The configuration containing the information we need.
    :param sdk: The SDK object that contains our functions.
    :return: The API result code.
    """
    num_modules = calculate_num_modules(cfg['slot_map'])

    slot_map = (ctypes.c_short * num_modules)()
    for crate, module_map in cfg.get('slot_map').items():
        for slot, module in module_map.items():
            slot_map[module] = ctypes.c_short(slot)

    return sdk.Pixie16InitSystem(ctypes.c_short(num_modules),
                                 ctypes.cast(slot_map, ctypes.POINTER(ctypes.c_short * 1)),
                                 ctypes.c_short(cfg['offline_mode']))


def boot(cfg, sdk):
    """
    Boots the modules by loading the firmware and a settings file.
    :param cfg: The configuration containing all the information we need to boot the module
    :param sdk: The SDK object with the boot function
    :return: The status code returned from the procedure.
    """
    return sdk.Pixie16BootModule(ctypes.c_char_p(cfg['firmware']['sys'].encode()),
                                 ctypes.c_char_p(cfg['firmware']['fippi'].encode()),
                                 ctypes.c_char_p(cfg['firmware']['trig'].encode()),
                                 ctypes.c_char_p(cfg['dsp']['ldr'].encode()),
                                 ctypes.c_char_p(cfg['dsp']['par'].encode()),
                                 ctypes.c_char_p(cfg['dsp']['var'].encode()),
                                 ctypes.c_short(calculate_num_modules(cfg['slot_map'])),
                                 127)


if __name__ == '__main__':
    try:
        PARSER = ArgumentParser(description='Converts PLD files into Apache Parquet format.')
        PARSER.add_argument('cfg', type=str, default='config.yaml',
                            help='The YAML configuration file')
        ARGS = PARSER.parse_args()
        with open(ARGS.cfg) as f:
            CFG = yaml.safe_load(f)

        if not CFG.get('sdk', None):
            raise LookupError("Configuration file is missing the path to the SDK")

        PIXIE_SDK = ctypes.cdll.LoadLibrary(CFG['sdk'])

        INIT_CODE = initialize_modules(CFG, PIXIE_SDK)
        if INIT_CODE != 0:
            raise RuntimeError(f"Initialization failed with Error Code {INIT_CODE}")

        BOOT_CODE = boot(CFG, PIXIE_SDK)
        if BOOT_CODE != 0:
            raise RuntimeError(f"Boot failed with Error Code {BOOT_CODE}")

    except (LookupError, RuntimeError) as err:
        print(err)
    except KeyboardInterrupt:
        print("Received interrupt!")
    finally:
        print("Finalizing...")
