# Data Visualizer
This script visualizes the CSV and list-mode data from our example codes.

## Install instructions
```shell
1. Clone the pixie_sdk repository into your Linux environment

    git clone https://github.com/xiallc/pixie_sdk.git

2. Compile the project (see installation.md in pixie_sdk/docs/pages)

3. Create/Navigate to dev folder

    mkdir ~/pixie_sdk/dev

    cd ~/pixie_sdk/dev

4. Install python virtual environment (or make sure it is installed)

    apt-get install python-venv

5. Initialize python virtual environment

    python3 -m venv venv

6. Activate the virtual environment

    source venv/bin/activate

7. Ensure pip is installed and upgraded

    python3 -m pip install --upgrade pip

    if already installed: pip install -U pip

8. Install the requirements for the data visualizer

    pip install -r ../examples/data_visualizer/requirements.txt

9. Run the data visualizer

    python3 ../examples/data_visualizer/data_visualizer.py -h
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
