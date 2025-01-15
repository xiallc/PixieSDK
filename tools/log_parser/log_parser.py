#!/usr/bin/env python

import argparse
import csv
from pathlib import Path

STATS_HEADER = [
    "time", "mod-num", "mod-slot", "period", "bw_MBps", "bw-max_MBps", "bw-min_MBps",
    "in_B", "out_B", "dma_B", "overflows", "dropped", "hw-overflows"
]

READ_HEADER = [
    "time", "mod-num", "mod-slot", "level", "read-words", "fifo-words"
]

DMA_HEADER = [
    "time", "mod-num", "mod-slot", "addr", "length", "end-time", "period"
]

API_HEADER = [
    "time", "call", "mod-num", "words", "reads"
]

dma_reads = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]


def process_stats(line):
    """
    Processes the FIFO stats debug message from a Pixie-16 log file into a CSV line
    containing the following fields: time, mod-num, mod-slot, period, bw_MBps,
    bw-max_MBps, bw-min_MBps, in_B, out_B, dma_B, overflows, dropped, hw-overflows

    :param line: the log line containing the FIFO Stats to extract
    :returns: A string formatted as a CSV containing the fields listed above.
    """
    line_parts = line.split()
    csv_parts = [line_parts[1]]
    csv_parts += (line_parts[3].replace('num=', '').replace('slot=', "").
                  rstrip(":").split(","))
    csv_parts.append(line_parts[7].split("=")[1])
    csv_parts.append(line_parts[8].split("=")[1].rstrip("MB/s"))
    csv_parts.append(line_parts[9].split("=")[1].rstrip("MB/s"))
    csv_parts.append(line_parts[10].split("=")[1].rstrip("MB/s"))
    csv_parts.append(line_parts[11].split("=")[1])
    csv_parts.append(line_parts[12].split("=")[1])
    csv_parts.append(line_parts[13].split("=")[1])
    csv_parts.append(line_parts[14].split("=")[1])
    csv_parts.append(line_parts[15].split("=")[1])
    csv_parts.append(line_parts[16].split("=")[1])
    return csv_parts

def process_read(line):
    """
    Processes the FIFO read debug message from a Pixie-16 log file into a CSV line
    containing the following fields: time, mod-num, mod-slot, level, read-words,
    data-fifo-words

    :param line: the log line containing the FIFO Stats to extract
    :returns: A string formatted as a CSV containing the fields listed above.
    """
    line_parts = line.split()
    csv_parts = [line_parts[1]]
    csv_parts += (line_parts[3].replace('num=', '').replace('slot=', "").
                  rstrip(":").split(","))
    csv_parts.append(line_parts[6].split("=")[1])
    csv_parts.append(line_parts[7].split("=")[1])
    csv_parts.append(line_parts[8].split("=")[1])
    return csv_parts

def process_dma_end(line):
    line_parts = line.split()
    csv_parts = [line_parts[1]]
    csv_parts += (line_parts[3].replace('num=', '').replace('slot=', "").
                  rstrip(":").split(","))
    csv_parts.append(line_parts[7].split("=")[1])
    dma_reads[int(csv_parts[1])] += 1
    return csv_parts

def process_pixie(line):
    line_parts = line.split()
    csv_parts = [line_parts[1]]
    csv_parts.append(line_parts[2].rstrip(":"))
    csv_parts.append(line_parts[3].split("=")[1])
    csv_parts.append(line_parts[4].split("=")[1])
    csv_parts.append(dma_reads[int(csv_parts[2])])
    dma_reads[int(csv_parts[2])] = 0
    return csv_parts

def process_level(line):
    line_parts = line.split()
    csv_parts = [line_parts[1]]
    csv_parts.append(line_parts[4].rstrip(":"))
    csv_parts.append(line_parts[3].replace('num=', '').replace('slot=', "").
                  rstrip(":").split(",")[0])
    csv_parts.append(line_parts[7])
    csv_parts.append(0)
    return csv_parts

def main(args):
    source_path = Path(args.filename)

    err_path = source_path.with_name(f'{source_path.stem}_error').with_suffix(".log")
    error_fh = open(err_path, "w", buffering=1)
    error_flags = ("Pixie16InitSystem", "[ERROR]", "eeprom: format", "eeprom: channels")

    stat_path = source_path.with_name(f'{source_path.stem}_stats').with_suffix(".csv")
    stats_fh = open(stat_path, "w", buffering=1, newline="")
    stat_writer = csv.writer(stats_fh)
    stat_writer.writerow(STATS_HEADER)

    read_path = source_path.with_name(f'{source_path.stem}_words').with_suffix(".csv")
    read_fh = open(read_path, "w", buffering=1, newline="")
    read_writer = csv.writer(read_fh)
    read_writer.writerow(READ_HEADER)

    dma_path = source_path.with_name(f'{source_path.stem}_DMA').with_suffix(".csv")
    dma_fh = open(dma_path, "w", buffering=1, newline="")
    dma_writer = csv.writer(dma_fh)
    dma_writer.writerow(DMA_HEADER)
    
    pixie_path = source_path.with_name(f'{source_path.stem}_api').with_suffix(".csv")
    pixie_fh = open(pixie_path, "w", buffering=1, newline="")
    pixie_writer = csv.writer(pixie_fh)
    pixie_writer.writerow(API_HEADER)

    with open(source_path, 'r') as f:
        for line in f:
            try:
                if any(flag in line for flag in error_flags):
                    error_fh.write(line)
                if " FIFO stats:  run:" in line:
                    stat_writer.writerow(process_stats(line))
                if " dma read: " in line:
                    # turn these lines into a single csv line with the following header
                    # so that we can plot the time and size of the reads.
                    if "addr=0x" in line:
                        #dma_writer.writerow(process_dma_start(line))
                        pass
                    if "done, period=" in line:
                        dma_writer.writerow(process_dma_end(line))
                if "FIFO read" in line:
                    read_writer.writerow(process_read(line))
                if "FromExternalFIFO" in line:
                    pixie_writer.writerow(process_pixie(line))
                if "read-list-mode-level:" in line:
                    pixie_writer.writerow(process_level(line))
            except KeyboardInterrupt:
                break

    error_fh.close()
    stats_fh.close()
    read_fh.close()
    dma_fh.close()
    pixie_fh.close()


if __name__ == '__main__':
    try:
        parser = argparse.ArgumentParser(
                    prog='Pixie-16 Log Parser',
                    description='Parses Pixie-16 logs for useful information.')
        parser.add_argument('filename')
        args = parser.parse_args()
        main(args)
    except KeyboardInterrupt:
        print("Received keyboard interrupt. Stopping execution.")
    finally:
        print("Finished execution.")
