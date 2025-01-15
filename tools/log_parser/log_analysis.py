#!/usr/bin/env python

import argparse
import datetime

import matplotlib.pyplot as plt
import matplotlib.ticker as tck
import pandas as pd

def time_convert(df):
    format_string = "%Y-%m-%dT%H:%M:%S.%f"
    df['time'] = df['time'].apply(lambda x: datetime.datetime.strptime(x, format_string).timestamp())
    df['time'] = df['time'].apply(lambda x: x - df['time'][0])
    return df

def slot_bandwidth(args):
    df = pd.read_csv(args.filename)
    df_bw = df[['mod-slot', 'bw_MBps']].groupby(['mod-slot']).mean()
    ax = df_bw.plot.bar(rot=0, legend=False)
    ax.set_ylabel("Mean Bandwidth (MB/s)")
    ax.set_xlabel("Hardware Slot")
    plt.show()

def max_slot_bandwidth(args):
    df = pd.read_csv(args.filename)
    df_bw = df[['mod-slot', 'bw-max_MBps']].groupby(['mod-slot']).mean()
    ax = df_bw.plot.bar(rot=0, legend=False)
    ax.set_ylabel("Max Bandwidth (MB/s)")
    ax.set_xlabel("Hardware Slot")
    plt.show()

def bandwidth_time(args):
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    slots = df['mod-slot'].unique()
    for slot in slots:
        df_slot = df[df['mod-slot'] == slot]
        df_slot.plot(x='time', y='bw_MBps', marker='.', title='Slot' + str(slot), ylabel='Mean Bandwidth (MB/s)', xlabel='Time (s)')
        df_slot.plot(x='time', y='bw-max_MBps', marker='.', title='Slot' + str(slot), ylabel='Max Bandwidth (MB/s)', xlabel='Time (s)')
    plt.show()

def dma_time(args):
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    df_bw = df[['time', 'mod-slot', 'dma_B']]
    df_bw = df_bw.pivot(index='time', columns='mod-slot', values='dma_B')
    ax = df_bw.plot(marker='.')
    ax.set_ylabel("DMA")
    ax.set_xlabel("Time (s)")
    plt.show()

def in_out(args):
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    slots = df['mod-slot'].unique()
    ya = 0
    for slot in slots:
        df_slot = df[df['mod-slot'] == slot]
        df_io = df_slot[['time', 'in_B', 'out_B']]
        df_io['diff'] = df['in_B'] - df['out_B']
        df_io.plot(y=['in_B', 'out_B'], marker='.', title="Slot " + str(slot), ylabel='DMA', xlabel='Time (s)')
        df_io.plot(y='diff', marker='.', title="Slot " + str(slot), ylabel='DMA in/out difference', xlabel='Time (s)')
        ya += 1
    plt.show()

def bandwidth_io(args):
    fig, axes = plt.subplots(nrows=1, ncols=7)
    df = pd.read_csv(args.filename)
    if (args.t):
        df = df.tail(700)
    slots = df['mod-slot'].unique()
    ya = 0
    for slot in slots:
        df_slot = df[df['mod-slot'] == slot]
        df_bw = df_slot[['bw_MBps']]
        df_bw['diff'] = df_slot['in_B'] - df_slot['out_B']
        df_bw.plot.scatter(x='bw_MBps', y='diff', ax=axes[ya], title="Slot " + str(slot), ylabel='DMA in/out difference', xlabel='Mean bandwidth')
        ya+=1
    plt.show()

def dma_prog(args):
    format_string = "%Y-%m-%dT%H:%M:%S.%f"
    fig, axes = plt.subplots(nrows=1, ncols=3)
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    slots = df['mod-slot'].unique()
    ya = 0
    for slot in slots:
        df_slot = df[df['mod-slot'] == slot]
        df_slot['in_diff'] = df_slot['in_B'] - df_slot['in_B'].shift(1)
        df_slot['in_diff'] = df_slot['in_diff'].clip(lower=0)
        df_slot.plot(x='time', y='in_diff', ax=axes[ya], marker='.', title="Slot " + str(slot), ylabel='Difference in DMA in', xlabel='Time (s)')
        ya += 1
    plt.show()

def dma_out(args):
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    df_bw = df[['time', 'mod-slot', 'out_B', 'dma_B']]
    df_bw.sort_values(by=['mod-slot', 'time'])
    slots = df['mod-slot'].unique()
    df_bw['dma_out'] = df['dma_B'] / df['out_B']
    ya = 0
    for slot in slots:
        df_slot = df_bw[df_bw['mod-slot'] == slot]
        a = df_slot.plot(x='time', y='dma_out', marker='.', title='Slot ' + str(slot), ylabel='Total DMA/output ratio', xlabel='Time (s)')
        a.yaxis.set_major_formatter(tck.FormatStrFormatter('%.5f'))
        ya+=1
    plt.show()

def words_time(args):
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    slots = df['mod-slot'].unique()
    ya = 0
    for slot in slots:
        df_slot = df[df['mod-slot'] == slot]
        df_slot.plot(x='time', y='read-words', marker='.', title='Slot' + str(slot), ylabel='Words read', xlabel='Time (s)')
        df_slot.plot(x='time', y='fifo-words', marker='.', title='Slot' + str(slot), ylabel='Data queue size', xlabel='Time (s)')
        ya += 1
    plt.show()

def period_time(args):
    period_string = "%H:%M:%S.%f"
    df = pd.read_csv(args.filename)
    df['period'] = df['period'].apply(lambda x: datetime.datetime.strptime(x, period_string))
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    df_bw = df[['time', 'mod-slot', 'period']]
    df_bw = df_bw.pivot(index='time', columns='mod-slot', values='period')
    ax = df_bw.plot(marker='.')
    ax.set_ylabel("Period (s)")
    ax.set_xlabel("Time")
    plt.show()

def read_usage(args):
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    df['time_diff'] = df['time'] - df['time'].shift(1)
    slots = df['mod-slot'].unique()
    for slot in slots:
        df_slot = df[df['mod-slot'] == slot]
        df_slot.plot(x='time', y='time_diff', marker='.', title='Slot' + str(slot), ylabel='Read time', xlabel='Time (s)')
    plt.show()

def api_usage(args):
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    mods = df['mod-num'].unique()
    for mod in mods:
        df_mod = df[df['mod-num'] == mod]
        df_mod_check = df_mod[df_mod['call'].str.contains('level')]
        df_mod_read = df_mod[df_mod['call'].str.contains('Read')]
        df_mod_check['time_diff'] = df_mod_check['time'] - df_mod_check['time'].shift(1)
        df_mod_read['time_diff'] = df_mod_read['time'] - df_mod_read['time'].shift(1)
        df_mod_check.plot(x='time', y='words', marker='.', title='Check FIFO Module ' + str(mod), ylabel='FIFO level', xlabel='Time (s)')
        df_mod_read.plot(x='time', y='time_diff', marker='.', title='Read FIFO Module ' + str(mod), ylabel='Time b/w calls', xlabel='Time (s)')
    plt.show()

def read_count(args):
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    mods = df['mod-num'].unique()
    for mod in mods:
        df_mod = df[df['mod-num'] == mod]
        df_mod_read = df_mod[df_mod['call'].str.contains('Read')]
        df_mod_read['time_diff'] = df_mod_read['time'] - df_mod_read['time'].shift(1)
        df_mod_read.plot(x='time', y='reads', marker='.', title='Read FIFO Module ' + str(mod), ylabel='DMA reads b/w calls', xlabel='Time (s)')
        df_mod_read = df_mod_read[df_mod_read['time_diff'] < 1]
        df_mod_read.plot.scatter(x='time_diff', y='reads', marker='.', title='Read FIFO Module ' + str(mod), ylabel='DMA reads b/w calls', xlabel='Time (s) b/w calls')
    plt.show()

def words_reads(args):
    df = pd.read_csv(args.filename)
    df = time_convert(df)
    if (args.t):
        df = df.tail(700)
    mods = df['mod-num'].unique()
    for mod in mods:
        df_mod = df[df['mod-num'] == mod]
        df_mod_read = df_mod[df_mod['call'].str.contains('Read')]
        df_mod_read['time_diff'] = df_mod_read['time'] - df_mod_read['time'].shift(1)
        df_mod_read.plot.scatter(x='words', y='reads', marker='.', title='Read FIFO Module ' + str(mod), ylabel='DMA reads b/w calls', xlabel='Words read')
    plt.show()

def main(args):
    if "stats" in args.filename:
        match args.graph:
            case "read_usage":
                read_usage(args)
            case "period_time":
                period_time(args)
            case "dma_out":
                dma_out(args)
            case "dma_prog":
                dma_prog(args)
            case "bandwidth_io":
                bandwidth_io(args)
            case "in_out":
                in_out(args)
            case "dma_time":
                dma_time(args)
            case "bandwidth_time":
                bandwidth_time(args)
            case "slot_bandwidth":
                slot_bandwidth(args)
            case "max_slot_bandwidth":
                max_slot_bandwidth(args)
    elif "words" in args.filename:
        words_time(args)
    elif "DMA" in args.filename:
        read_usage(args)
    elif "api" in args.filename:
        match args.graph:
            case "api_usage":
                api_usage(args)
            case "read_count":
                read_count(args)
            case "words_reads":
                words_reads(args)


if __name__ == '__main__':
    try:
        parser = argparse.ArgumentParser(
            prog='Pixie-16 Log Analyzer',
            description='Analyzes information parsed from Pixie-16 logs.',
            usage='log_analysis [-h] [-t] filename graph')
        parser.add_argument('filename', help='Name of file to parse (types: stats, words, DMA, api)')
        parser.add_argument('graph', help='''Type of graph to produce:\n
                                             [stats:
                                                slot_bandwidth,
                                                max_slot_bandwidth,
                                                bandwidth_time,
                                                dma_time,
                                                in_out,
                                                bandwidth_io,
                                                dma_prog,
                                                dma_out,
                                                period_time,
                                                read_usage]
                                              [words:
                                                words_time]
                                              [DMA:
                                                read_usage]
                                              [api:
                                                api_usage,
                                                read_count,
                                                words_reads]''')
        parser.add_argument('-t', action='store_true', help='Analyze only the end of the run')
        args = parser.parse_args()
        main(args)
    except KeyboardInterrupt:
        print("Received keyboard interrupt. Stopping execution.")
    finally:
        print("Finished execution.")
