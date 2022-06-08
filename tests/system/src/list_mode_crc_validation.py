""" SPDX-License-Identifier: Apache-2.0 """
import argparse
import csv
import logging
import os
import sys
import zlib

"""
Copyright 2022 XIA LLC, All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

""" list-mode-crc-validation.py validates the data contained within a 
list-mode binary data file produced with the PixieSDK examples 
against a CSV containing DMA reads and CRC-32 values.

At the moment the CSV file must be manually produced as this process is still
in its early stages. Assuming that you have a log file containing just the 
debug log for the run of interest, you can generate the CSV using the following
bash command: 

```bash
CSV_FILE="list-mode-crc.csv"; echo "len,crc" > ${CSV_FILE}; \
grep "crc=0x" Pixie16Msg.log | awk '{print $8 " " $10}' \
| awk '{gsub("=", " ", $0); print $2,",",$4} ' >> ${CSV_FILE}
```
"""

logging.basicConfig(stream=sys.stdout, level=logging.INFO, datefmt="%Y-%m-%dT%H:%M:%S",
                    format='%(asctime)s.%(msecs)03d - %(levelname)s - %(message)s')


def main(cfg):
    logging.info(f"Starting to process {cfg.file}")

    log_data = []
    with open(cfg.input, mode='r', newline='') as csv_log:
        entries = csv.DictReader(csv_log)
        for entry in entries:
            entry['len'] = int(entry['len'], 10)
            entry['crc'] = int(entry['crc'], 16)
            log_data.append(entry)

    file_size_words = os.path.getsize(cfg.file) / cfg.bpw
    total_dma_words = sum([x['len'] for x in log_data])
    logging.info(f"DMA Word Total: {total_dma_words}")
    logging.info(f"File Word Total: {file_size_words}")

    if file_size_words != total_dma_words:
        raise ValueError("The DMA word total does not match the File word total!")

    with open(cfg.file, "rb") as file:
        for idx, x in enumerate(log_data):
            log_crc = x['crc']
            file_crc = zlib.crc32(file.read(x['len'] * cfg.bpw))
            if log_crc != file_crc:
                logging.info(
                    f"Total DMA Words Processed: {sum([x['len'] for x in log_data[:idx]])}")
                raise ValueError(
                    f"CRC check failed! Segment={idx} Len={x['len']} DMA={log_crc} FILE={file_crc}")
    logging.info("All CRC checks passed!")


if __name__ == '__main__':
    try:
        parser = argparse.ArgumentParser(description='Compares DMA CRCs to Files written to disk.')
        parser.add_argument('-b', '--bytes-per-word', type=int, dest='bpw', default=4,
                            help="The number of bytes per data word for the list-mode file.")
        parser.add_argument('-i', '--input', type=str, dest="input",
                            required=True, help="The CSV file containing the read lengths and CRC.")
        parser.add_argument('-f', '--file', dest='file',
                            help="The binary data file containing the list-mode data.")
        cfg = parser.parse_args()

        main(cfg)
        logging.info(f"Finished processing {cfg.file}.")
    except ValueError as ve:
        logging.error(ve)
    except KeyboardInterrupt:
        logging.info("Received keyboard interrupt. Stopping execution.")
    except Exception as e:
        logging.error(e)
    finally:
        logging.info("Execution finished.")
