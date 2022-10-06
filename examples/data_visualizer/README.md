# Data Visualizer
This script visualizes the CSV and list-mode data from our example codes.

## Prerequisites

1. python 3.7+
2. pip
3. venv

## Linux Install Instructions

1. Clone the pixie_sdk repository into your Linux environment
    ```shell
    git clone https://github.com/xiallc/pixie_sdk.git
    ```
2. Create/Navigate to dev folder
    ```shell
    mkdir ~/pixie_sdk/dev && cd ~/pixie_sdk/dev
    ```
3. Initialize python virtual environment
    ```shell
    python3 -m venv venv
    ```
4. Activate the virtual environment
    ```shell
    source venv/bin/activate
    ```
5. Ensure pip is upgraded
    ```shell
    pip install -U pip
    ```
6. Install the requirements for the data visualizer
    ```shell
    pip install -r ../examples/data_visualizer/requirements.txt
    ```
7. Run the data visualizer
    ```shell
    python3 ../examples/data_visualizer/data_visualizer.py -h
    ```

## Windows Install Instructions

1. Clone the pixie_sdk repository to your Windows environment (Downloads folder)
2. Create/Navigate to dev folder
    ```shell
    mkdir ~/pixie_sdk/dev && cd ~/pixie_sdk/dev
    ```
3. Initialize python virtual environment
    ```shell
    python3 -m venv venv
    ```
4. Install the requirements for the data visualizer
    ```shell
    pip install -r ../examples/data_visualizer/requirements.txt
    ```
5. Activate the virtual environment
    ```shell
    .\venv\Scripts\activate
    ```
6. Run the data visualizer
    ```shell
    python3 ../examples/data_visualizer/data_visualizer.py -h
    ```
7. To stop the virtual environment
    ```shell
    deactivate
    ```

## Usage
```shell
usage: data_visualizer.py [-h] [-b] [-c CHAN] -f FILE [--freq FREQ] [-l] [-m]
                          [-x XLIM] [--rev REV] [-s] [-a]

Optional app description

options:
  -h, --help            show this help message and exit
  -b, --baseline        Plots MCA spectra
  -c CHAN, --chan CHAN  The channel that you'd like to plot.
  -f FILE, --file FILE  The file containing the data to read.
  --freq FREQ           The sampling frequency used to collect list-mode data.
                        Ex. 250
  -l, --lmd             Tells the program that the file is list-mode data.
  -m, --mca             Plots MCA spectra
  -x XLIM, --xlim XLIM  Comma separated range for X-axis limits. Ex. 10,400
  --rev REV             The firmware used to collect list-mode data. Ex. 30474
  -s, --stats           Processes a module's CSV statistics file.
  -a, --adc             Plots traces
```
