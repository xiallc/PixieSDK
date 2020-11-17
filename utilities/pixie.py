"""
file: pixie.py
brief: 
author: S. V. Paulauskas
date: November 16, 2020
"""
from sys import exit
from argparse import ArgumentParser
import ctypes

import yaml


def calculate_num_modules(slot_map):
    return sum([len(v) for v in slot_map.values()])


def initialize_modules(cfg, sdk):
    num_modules = calculate_num_modules(cfg['slot_map'])

    slot_map = (ctypes.c_short * num_modules)()
    for crate, module_map in cfg.get('slot_map').items():
        for slot, module in module_map.items():
            slot_map[module] = ctypes.c_short(slot)

    return sdk.Pixie16InitSystem(ctypes.c_short(num_modules),
                                 ctypes.cast(slot_map, ctypes.POINTER(ctypes.c_short * 1)),
                                 ctypes.c_short(cfg['offline_mode']))


def boot(cfg, sdk):
    return sdk.Pixie16BootModule(ctypes.c_char_p(cfg['firmware']['sys'].encode()),
                                 ctypes.c_char_p(cfg['firmware']['fippi'].encode()),
                                 ctypes.c_char_p(cfg['firmware']['trig'].encode()),
                                 ctypes.c_char_p(cfg['dsp']['ldr'].encode()),
                                 ctypes.c_char_p(cfg['dsp']['par'].encode()),
                                 ctypes.c_char_p(cfg['dsp']['var'].encode()),
                                 ctypes.c_short(calculate_num_modules(cfg['slot_map'])),
                                 127)


def main():
    # PixieSysDLL = ctypes.cdll.LoadLibrary("/home/stan/clion/software/build/libPixie16Sys.so")
    # print(PixieSysDLL.Pixie_InitSystem(ctypes.c_short(1),
    #                                    ctypes.cast(slot_map, ctypes.POINTER(ctypes.c_short * 1)),
    #                                    ctypes.c_short(0)))
    pass


if __name__ == '__main__':
    try:
        PIXIE_SDK = ctypes.cdll.LoadLibrary("/home/stan/clion/software/build/libPixie16App.so")

        PARSER = ArgumentParser(description='Converts PLD files into Apache Parquet format.')
        PARSER.add_argument('cfg', type=str, default='config.yaml',
                            help='The YAML configuration file')
        ARGS = PARSER.parse_args()
        with open(ARGS.cfg) as f:
            CFG = yaml.safe_load(f)

        INIT_CODE = initialize_modules(CFG, PIXIE_SDK)
        if INIT_CODE != 0:
            print(f"Initialization failed with Error Code {INIT_CODE}")
            raise KeyboardInterrupt

        BOOT_CODE = boot(CFG, PIXIE_SDK)
        if BOOT_CODE != 0:
            print(f"Boot failed with Error Code {INIT_CODE}")
            raise KeyboardInterrupt
    except KeyboardInterrupt:
        print("Received interrupt!")
    finally:
        print("Finalizing...")
