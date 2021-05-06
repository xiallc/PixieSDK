"""
file: set_file_decoder.py
brief: Decodes legacy settings files and converts them into JSON format.
author: S. V. Paulauskas
date: May 04, 2021
"""
import argparse
import io
import os.path
import re
import struct
import json

ENTRIES_PER_MOD = 1280
WORD = 4

HW_VERSION_PATTERN = re.compile('rev([a,b,c,d,f])')
VERSION_PATTERN = re.compile('(r\d{5})')
ADC_PATTERN = re.compile('(\d{2})b(\d{3})m')

CHANNEL_INPUT_VARIABLES = ["ChanCSRa", "ChanCSRb", "GainDAC", "OffsetDAC", "DigGain", "SlowLength",
                           "SlowGap", "FastLength", "FastGap", "PeakSample", "PeakSep", "CFDThresh",
                           "FastThresh", "ThreshWidth", "PAFlength", "TriggerDelay", "ResetDelay",
                           "ChanTrigStretch", "TraceLength", "Xwait", "TrigOutLen", "EnergyLow",
                           "Log2Ebin", "MultiplicityMaskL", "PSAoffset", "PSAlength", "Integrator",
                           "BLcut", "BaselinePercent", "FtrigoutDelay", "Log2Bweight", "PreampTau",
                           "Xavg", "MultiplicityMaskH", "FastTrigBackLen", "CFDDelay", "CFDScale",
                           "ExtTrigStretch", "VetoStretch", "ExternDelayLen", "QDCLen0", "QDCLen1",
                           "QDCLen2", "QDCLen3", "QDCLen4", "QDCLen5", "QDCLen6", "QDCLen7"]
CHANNEL_OUTPUT_VARIABLES = ['LiveTimeA', 'LiveTimeB', 'FastPeaksA', 'FastPeaksB', 'OverflowA',
                            'OverflowB', 'InSpecA', 'InSpecB', 'UnderflowA', 'UnderflowB',
                            'ChanEventsA', 'ChanEventsB', 'AutoTau', 'U30']
MODULE_INPUT_VARIABLES = ['ModNum', 'ModCSRA', 'ModCSRB', 'ModFormat', 'RunTask', 'ControlTask',
                          'MaxEvents', 'CoincPattern', 'CoincWait', 'SynchWait', 'InSynch',
                          'Resume', 'SlowFilterRange', 'FastFilterRange', 'ChanNum', 'HostIO',
                          'UserIn', 'FastTrigBackplaneEna', 'CrateID', 'SlotID', 'ModID',
                          'TrigConfig', 'U00', 'HostRunTimePreset', 'PowerUpInitDone']
MODULE_OUTPUT_VARIABLES = ['RealTimeA', 'RealTimeB', 'RunTimeA', 'RunTimeB', 'GSLTtime',
                           'NumEventsA', 'NumEventsB', 'DSPerror', 'SynchDone', 'BufHeadLen',
                           'EventHeadLen', 'ChanHeadLen', 'UserOut', 'AOutBuffer', 'LOutBuffer',
                           'AECorr', 'LECorr', 'HardwareID', 'HardVariant', 'FIFOLength', 'FippiID',
                           'FippiVariant', 'DSPrelease', 'DSPbuild', 'DSPVariant', 'U20']


def extract_metadata(var_name):
    name = os.path.basename(var_name).rsplit(".", 1)[0]

    hw_rev = re.search(HW_VERSION_PATTERN, var_name)
    if hw_rev:
        hw_rev = hw_rev.group(1)
    else:
        hw_rev = ""

    rev = re.search(VERSION_PATTERN, var_name)
    if rev:
        rev = rev.group()
    else:
        rev = ""

    adc_match = re.search(ADC_PATTERN, var_name)
    if adc_match:
        bit, freq = adc_match.group(1, 2)
    else:
        bit = freq = 0

    return {'var': f"{name}.var", 'ldr': f"{name}.ldr", 'rev': rev, 'adc_bit_resolution': int(bit),
            'adc_sampling_frequency': int(freq), 'hardware_revision': hw_rev.upper()}


def organize_module_dict(raw):
    mod_dict = {'metadata': {}, 'channel': {'input': {}, 'output': {}},
                'module': {'input': {}, 'output': {}}}
    for key, value in raw.items():
        if key in CHANNEL_INPUT_VARIABLES:
            mod_dict['channel']['input'][key] = value
        if key in CHANNEL_OUTPUT_VARIABLES:
            mod_dict['channel']['output'][key] = value
        if key in MODULE_INPUT_VARIABLES:
            mod_dict['module']['input'][key] = value
        if key in MODULE_OUTPUT_VARIABLES:
            mod_dict['module']['output'][key] = value
    return mod_dict


def process_module_buffer(buf, variables):
    pars = dict()
    for key, value in variables.items():
        pars[key] = struct.unpack(f"{value}L", buf.read(value * WORD))
        if value == 1:
            pars[key] = pars[key][0]
    return organize_module_dict(pars)


def read_settings_file(input_file, variables, metadata):
    modules = list()
    raw_buf = open(input_file, 'rb')
    while True:
        chunk = raw_buf.read(ENTRIES_PER_MOD * WORD)
        if chunk:
            data = process_module_buffer(io.BytesIO(chunk), variables)
            data['metadata'] = metadata
            modules.append(data)
        else:
            break
    raw_buf.close()
    return modules


def read_variable_file(var):
    addresses = list()
    labels = list()
    for line in open(var).readlines():
        address, name = line.strip("\n").split("  ")
        addresses.append(int(address, 16))
        labels.append(name)
    lengths = [addresses[i + 1] - addresses[i] for i in range(0, len(addresses) - 1)]
    lengths.append(ENTRIES_PER_MOD - sum(lengths))
    address_map = {x[0]: x[1] for x in zip(labels, lengths)}
    return address_map


def main():
    parser = argparse.ArgumentParser(description='Converts legacy settings files to JSON.')
    parser.add_argument("-v", "--var", dest="var_file", type=str, help="Path to the DSP var file.",
                        required=True)
    parser.add_argument("-i", "--input", dest="input_file", type=str,
                        help="Path to the settings file.", required=True)
    args = parser.parse_args()

    if not os.path.isfile(args.input_file):
        print(f"Could not access {args.input_file}")
        return
    if not os.path.isfile(args.var_file):
        print(f"Could not access {args.var_file}")
        return

    data = read_settings_file(args.input_file,
                              read_variable_file(args.var_file),
                              extract_metadata(args.var_file))
    output_file = f'{args.input_file.rsplit(".", 1)[0]}.json'
    try:
        out = open(output_file, 'w')
        out.write(json.dumps(data, sort_keys=True, indent=2, ensure_ascii=True))
    except IOError:
        print(f"Could not open {output_file} for processing!")
    finally:
        out.close()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt as ki:
        print("Received keyboard interrupt! Stopping execution")
    finally:
        print("Execution complete. See you, space cowboy!")
