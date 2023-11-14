/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2023 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @brief Offset Sweep Test
 */

#include <fstream>

#include <pixie/utils/io.hpp>
#include <pixie/utils/numerics.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-threads.hpp>

#include <omnitool-qa.hpp>

namespace xia {
namespace omnitool {
namespace qa {
struct offset_sweep_worker : public thread::module_worker {
    /*
     * Test defaults.
     */
    static constexpr size_t default_dac_settle_msec = 15;
    static constexpr int default_offset_start = 1000;
    static constexpr int default_offset_end = 65000;
    static constexpr int default_offset_step = 1000;
    static constexpr int default_threshold_multipler = 100;
    static constexpr size_t default_window = 5;

    /*
     * A result per offset for each channel a module has
     */
    struct offset_result {
        int  offset;
        double mean;
        double stddev;
        double delta;
        offset_result(int offset_, double mean_, double stddev_)
            : offset(offset_), mean(mean_), stddev(stddev_), delta(0) {}
    };
    struct channel_results {
        size_t channel;
        std::string error;
        std::vector<offset_result> results;
        size_t slope_start;
        size_t slope_end;
        double slope_k;
        double slope_c;
        channel_results() : channel(0), slope_start(0), slope_end(0), slope_k(0), slope_c(0) {}
    };
    using module_results = std::vector<channel_results>;

    pixie::channel::range channels;

    size_t dac_settle_usec;

    int offset_start;
    int offset_end;
    int offset_step;

    int threshold_multipler;
    size_t window;

    std::string filename;

    bool plot;

    offset_sweep_worker();

    void csv_generate(const module_results& mod_results);

    void plot_generate(
        pixie::module::module& module,
        const module_results& mod_results);
    void plot_header(std::ofstream& out);
    void plot_trailer(std::ofstream& out, const std::string title);

    void set_module_mean_deltas(module_results& mod_results);

    void find_slope(module_results& mod_results);
    void find_channel_mean_min_max(
        const channel_results& channel_results, int& max, size_t& max_index,
        int& min, size_t& min_index);
    double find_max_stddev(const channel_results& channel_results);

    void worker(command::context& context, pixie::module::module& module);
};

static double adc_mean(xia::pixie::hw::adc_trace& adc_trace) {
    util::numerics::average<double> mean;
    for (auto adc : adc_trace) {
        mean.update(adc);
    }
    mean.calc();
    return mean.avg;
}

static double adc_stddev(xia::pixie::hw::adc_trace& adc_trace, double mean) {
    util::numerics::variance<double> var(mean);
    for (auto adc : adc_trace) {
        var.update(adc);
    }
    var.calc();
    return var.stddev;
}

offset_sweep_worker::offset_sweep_worker()
    : dac_settle_usec(default_dac_settle_msec), offset_start(default_offset_start),
      offset_end(default_offset_end), offset_step(default_offset_step),
      threshold_multipler(default_threshold_multipler), window(default_window),
      plot(false) {}

void offset_sweep_worker::csv_generate(
    const offset_sweep_worker::module_results& mod_results) {
    std::string fout = filename + ".csv";
    std::ofstream out(fout, std::ios::out | std::ios::binary);
    out << "chan,stddev,gain,intercepts" << std::endl;
    for (size_t channel = 0; channel < mod_results.size(); ++channel) {
        auto& chan_results = mod_results[channel];
        auto msdi = std::max_element(
            chan_results.results.begin(), chan_results.results.end(),
            [](auto& a, auto& b) {
                return a.stddev < b.stddev;
            });
        auto& max_stddev = *msdi;
        out << channel << ','
            << max_stddev.stddev << ','
            << chan_results.slope_k << ','
            << chan_results.slope_c
            << std::endl;
    }
    out.close();
}

void offset_sweep_worker::plot_generate(
    pixie::module::module& module, const offset_sweep_worker::module_results& mod_results) {
    std::string fout = filename + ".py";
    std::ofstream out(fout, std::ios::out | std::ios::binary);
    plot_header(out);
    for (const auto& chan_results : mod_results) {
        if (chan_results.results.size() > 0) {
            for (const auto& result : mod_results[0].results) {
                out << "offsets += [" << result.offset << ']' << std::endl;
            }
            break;
        }
    }
    for (size_t channel = 0; channel < mod_results.size(); ++channel) {
        const auto& chan_results = mod_results[channel];
        if (!chan_results.error.empty()) {
            out << "errors[" << channel << "] = '" << chan_results.error << "'" << std::endl;
        }
        util::numerics::average<double> avg_stddev;
        for (const auto& result : chan_results.results) {
            avg_stddev.update(result.stddev);
        }
        avg_stddev.calc();
        out << "slopes[" << channel
            << "] = [" << chan_results.results[chan_results.slope_start].offset
            << ", " << chan_results.results[chan_results.slope_end].offset
            << ", " << chan_results.results[chan_results.slope_start].mean
            << ", " << chan_results.results[chan_results.slope_end].mean
            << ", " << chan_results.slope_k
            << ", " << chan_results.slope_c
            << ", " << avg_stddev.avg
            << "]" << std::endl;
        for (const auto& result : chan_results.results) {
            out << "means[" << channel << "] += [" << result.mean << ']' << std::endl
                << "stddevs[" << channel << "] += [" << result.stddev << ']' << std::endl;
        }
    }
    const std::string title =
        "'Pixie16 - Serial Num:" + std::to_string(serial_num) +
        " Rev:" + module.version_label() + " : Channel '  + str(channel)";
    plot_trailer(out, title);
    out.close();
}

void offset_sweep_worker::plot_header(std::ofstream& out) {
    out << "from matplotlib.backends.backend_pdf import PdfPages" << std::endl
        << "import matplotlib.pyplot as plt" << std::endl
        << std::endl
        << "filename = '" << filename << ".pdf'"  << std::endl
        << std::endl
        << "channels = " << channels.size() << std::endl
        << "offsets = []" << std::endl
        << "means = [[] for i in range(0, " << channels.size() << ")]" << std::endl
        << "stddevs = [[] for i in range(0, " << channels.size() << ")]" << std::endl
        << "slopes = [[] for i in range(0, " << channels.size() << ")]" << std::endl
        << "errors = ['' for i in range(0, " << channels.size() << ")]" << std::endl
        << std::endl;
}

void offset_sweep_worker::plot_trailer(std::ofstream& out, const std::string title) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    out << std::endl
        << "with PdfPages(filename) as pdf:" << std::endl
        << "    for channel in range(0, channels):" << std::endl
        << "        plt.figure()" << std::endl
        << "        fig, axs = plt.subplots(2, 1)" << std::endl
        << "        fig.suptitle(" + title + ")" << std::endl
        << "        axs[0].plot(offsets, means[channel], linewidth=0.8)" << std::endl
        << "        axs[0].set_ylabel('ADC')" << std::endl
        << "        axs[0].set_xlabel('Offset (DAC)')" << std::endl
        << "        axs[0].set_title('Offset Sweep')" << std::endl
        << "        axs[0].grid(True, linewidth=0.5)" << std::endl
        << "        if len(errors[channel]) == 0:" << std::endl
        << "            if len(slopes[channel]) > 0:" << std::endl
        << "                axs[0].vlines(slopes[channel][0:2], 0, 1," << std::endl
        << "                              linestyles='dotted', colors='r', linewidth=0.7," << std::endl
        << "                              transform=axs[0].get_xaxis_transform())" << std::endl
        << "                axs[0].plot(slopes[channel][0:2]," << std::endl
        << "                            slopes[channel][2:4]," << std::endl
        << "                            linewidth=0.4, linestyle='dotted')" << std::endl
        << "                y = lambda x: x * slopes[channel][4] + slopes[channel][5]" << std::endl
        << "                x_vals = [x for x in range(slopes[channel][0], slopes[channel][1], " << offset_step << ")]" << std::endl
        << "                y_vals = list(map(y, x_vals))" << std::endl
        << "                axs[0].plot(x_vals, y_vals, linewidth=3, alpha=0.2, color='black')" << std::endl
        << "                x_pos = (slopes[channel][1] - slopes[channel][0]) / 2 + slopes[channel][0]" << std::endl
        << "                y_pos = y(x_pos) " << std::endl
        << "                axs[0].annotate('gain:' + str(slopes[channel][4]) + ', intercept:' + str(slopes[channel][5])," << std::endl
        << "                                xy=(x_pos, y_pos), xycoords='data'," << std::endl
        << "                                fontsize=8, horizontalalignment='right')" << std::endl
        << "        else:" << std::endl
        << "            axs[0].text(.5, .5, errors[channel]," << std::endl
        << "                        bbox={'facecolor': 'red', 'alpha': 0.5, 'pad': 2},"
        << std::endl
        << "                        horizontalalignment='center', verticalalignment='center',"
        << std::endl
        << "                        transform=axs[0].transAxes)" << std::endl
        << "        axs[1].plot(offsets, stddevs[channel], linewidth=0.8)" << std::endl
        << "        axs[1].set_ylabel('Standard Deviation')" << std::endl
        << "        axs[1].set_xlabel('Offset (DAC)')" << std::endl
        << "        axs[1].set_title('Standard Deviation')" << std::endl
        << "        axs[1].grid(True, linewidth=0.5)" << std::endl
        << "        axs[1].annotate('" << std::put_time(&tm, "%d %b %Y %H:%M:%S") << "',"
        << std::endl
        << "                        xy = (-0.1, -0.6)," << std::endl
        << "                        xycoords='axes fraction'," << std::endl
        << "                        ha='left'," << std::endl
        << "                        va='bottom'," << std::endl
        << "                        fontsize=8)" << std::endl
        << "        fig.tight_layout()" << std::endl
        << "        pdf.savefig()" << std::endl
        << "        plt.close('all')" << std::endl;
}

void offset_sweep_worker::find_slope(
    offset_sweep_worker::module_results& mod_results) {
    for (size_t channel = 0; channel < mod_results.size(); ++channel) {
        auto& chan_results = mod_results[channel];
        /*
         * Find the last min value and the first max value. Then move
         * right to find the start of the slop from the min and then
         * right from the max value to end of the slope.
         */
        int max;
        int min;
        size_t max_index;
        size_t min_index;
        find_channel_mean_min_max(chan_results, max, max_index, min, min_index);
        if (min_index >= max_index || (max_index - min_index) <= window) {
            xia_log(xia::log::error) << "invalid signal: serial-num=" << serial_num
                                     << " channel=" << channel;
            chan_results.error = "Invalid signal, no slope found";
            continue;
        }
        const double threshold = threshold_multipler * find_max_stddev(chan_results);
        bool searching = true;
        while (searching && min_index < max_index - window) {
            if (chan_results.results[min_index].delta >= threshold) {
                searching = false;
                for (size_t i = 0; i < window; ++i) {
                    if (chan_results.results[min_index + i].delta < threshold) {
                        min_index += i;
                        searching = true;
                        break;
                    }
                }
            } else {
                ++min_index;
            }
        }
        if (min_index >= max_index - window) {
            xia_log(xia::log::error) << "slope start not found: serial-num=" << serial_num
                                     << " channel=" << channel
                                     << " min-index=" << min_index << " (" << min << ')'
                                     << " min-detla=" << chan_results.results[min_index].delta
                                     << " max_index=" << max_index << " (" << max << ')'
                                     << " max-detla=" << chan_results.results[max_index].delta
                                     << " threshold=" << threshold;
            chan_results.error = "Invalid signal, slope start not found";
            continue;
        }
        searching = true;
        while (searching && max_index > min_index + window) {
            if (chan_results.results[max_index].delta >= threshold) {
                searching = false;
                for (size_t i = 0; i < window; ++i) {
                    if (chan_results.results[max_index - i].delta < threshold) {
                        max_index -= i;
                        searching = true;
                        break;
                    }
                }
            } else {
                --max_index;
            }
        }
        if (max_index <= min_index + window) {
            xia_log(xia::log::error) << "slope end not found: serial-num=" << serial_num
                                     << " channel=" << channel;
            chan_results.error = "Invalid signal, slope end not found";
            continue;
        }
        /*
         * Shrink the limits to avoid picking up a slope that is the
         * result of the offset hitting a rail at an offset inside the
         * step size.
         */
        chan_results.slope_start = min_index;
        chan_results.slope_end = max_index;
        util::numerics::linear_fit<double> slope;
        for (size_t index = min_index; index < max_index; ++index) {
            auto& result = chan_results.results[index];
            slope.update(result.offset, result.mean);
        }
        slope.calc();
        chan_results.slope_k = slope.k;
        chan_results.slope_c = slope.c;
    }
}

void offset_sweep_worker::find_channel_mean_min_max(
    const offset_sweep_worker::channel_results& chan_results, int& max, size_t& max_index,
    int& min, size_t& min_index) {
    max = std::numeric_limits<int>::min();
    min = std::numeric_limits<int>::max();
    max_index = chan_results.results.size();
    min_index = chan_results.results.size();
    for (size_t i = 0; i < chan_results.results.size(); ++i) {
        auto& result = chan_results.results[i];
        int mean = int(result.mean);
        if (max < mean) {
            max = mean;
            max_index = i;
        }
        if (min >= mean) {
            min = mean;
            min_index = i;
        }
    }
    if (min_index == chan_results.results.size()) {
        throw std::runtime_error("minimum value not found in ADC trace data");
    }
    if (max_index == chan_results.results.size()) {
        throw std::runtime_error("maximum value not found in ADC trace data");
    }
}

double offset_sweep_worker::find_max_stddev(const channel_results& channel_results) {
    auto max_iter = std::max_element(
        channel_results.results.begin(), channel_results.results.end(), [](auto& a, auto& b) {
            return a.stddev < b.stddev;
        });
    return (*max_iter).stddev;
}

void offset_sweep_worker::set_module_mean_deltas(
    offset_sweep_worker::module_results& mod_results) {
    for (auto& chan_results : mod_results) {
        auto& results = chan_results.results;
        for (auto cri = results.begin() + 1; cri != results.end(); ++cri) {
            auto& prev = *(cri - 1);
            auto& curr = *cri;
            curr.delta = curr.mean - prev.mean;
        }
    }
}

void offset_sweep_worker::worker(
    command::context& , pixie::module::module& module) {
    try {
        pixie::hw::adc_trace adc_trace(pixie::hw::max_adc_trace_length);
        module_results mod_results(channels.size());
        for (auto chan : channels) {
            mod_results[chan].channel = chan;
        }
        period.start();
        for (auto channel : channels) {
            /*
             * 4 is a magic number to make this work!
             */
            module.write_var("ChanCSRa", 4, channel);
        }
        for (int offset = offset_start;
             offset <= offset_end;
             offset += offset_step) {
            for (auto channel : channels) {
                module.write_var("OffsetDAC", offset, channel);
            }
            module.set_dacs();
            /*
             * This is set by looking at Rev F
             */
            pixie::hw::wait(dac_settle_usec);
            module.get_traces();
            for (auto channel : channels) {
                module.read_adc(channel, adc_trace, false);
                auto mean = adc_mean(adc_trace);
                auto stddev = adc_stddev(adc_trace, mean);
                mod_results[channel].results.emplace_back(offset, mean, stddev);
            }
        }
        set_module_mean_deltas(mod_results);
        find_slope(mod_results);
        if (plot) {
            plot_generate(module, mod_results);
        }
        csv_generate(mod_results);
        period.end();
    } catch (...) {
        period.end();
        has_error = true;
        throw;
    }
}

void offset_sweep(command::context& context) {
    auto& crate = context.crate;
    auto offset_opt = context.cmd.get_option("-o");
    auto plot_opt = context.cmd.get_option("-p");
    auto dac_settle_opt = context.cmd.get_option("-s");
    auto threshold_multipler_opt = context.cmd.get_option("-t");
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    command::modules_option(
        mod_nums, mod_nums_opt, crate.num_modules);
    if (mod_nums.size() > 6) {
        throw std::runtime_error(
            "test: offset sweep: too modules for test; max 6, select fewer");
    }
    size_t dac_settle_msec = offset_sweep_worker::default_dac_settle_msec;
    if (!dac_settle_opt.empty()) {
        dac_settle_msec = util::io::get_value<size_t>(dac_settle_opt);
    }
    int offset_start = offset_sweep_worker::default_offset_start;
    int offset_end = offset_sweep_worker::default_offset_end;
    int offset_step = offset_sweep_worker::default_offset_step;
    int threshold_multipler = offset_sweep_worker::default_threshold_multipler;
    if (!threshold_multipler_opt.empty()) {
        threshold_multipler = util::io::get_value<int>(threshold_multipler_opt);
    }
    auto offset_nums = util::io::get_values<int>(offset_opt);
    if (offset_nums.size() > 0) {
        offset_start = offset_nums[0];
        if (offset_nums.size() > 1) {
            offset_end = offset_nums[1];
            if (offset_nums.size() > 2) {
                offset_step = offset_nums[2];
            }
        }
    }
    auto offset_sweeps = std::vector<offset_sweep_worker>(mod_nums.size());
    auto datetime = xia::util::time::datetime_iso8601();
    util::time::datetime_iso8601_as_filename(datetime);
    thread::set_num_slot(crate, mod_nums, offset_sweeps);
    for (auto mod_num : mod_nums) {
        pixie::channel::range channels;
        command::channels_option(channels, chans_opt, crate[mod_num].num_channels);
        auto& worker = offset_sweeps[mod_num];
        worker.channels = channels;
        worker.dac_settle_usec = dac_settle_msec * 1000UL;
        worker.offset_start = offset_start;
        worker.offset_end = offset_end;
        worker.offset_step = offset_step;
        worker.threshold_multipler = threshold_multipler;
        worker.filename =
            datetime + '_' + std::to_string(worker.serial_num) + "_dac-sweep";
        worker.plot = plot_opt == "true";
    }
    context.opts.out << "Test: offset sweep : num-modules=" << mod_nums.size()
                     << " start=" << offset_start
                     << " end=" << offset_end
                     << " step=" << offset_step
                     << " settle-period=" << dac_settle_msec << "msec"
                     << " threshold-multipler=" << threshold_multipler
                     << std::endl;
    module_threads(
        context, mod_nums, offset_sweeps, "offset sweep test error; see log", false);
}

void offset_sweep_comp(
    command::context& context, command::completion& completions) {
    auto offset_sweep_cmd = context.cmd.def;

    auto not_completed = !command::completions::flag_completion(NULL,
        offset_sweep_cmd.name, completions);
    if (not_completed) {
        auto off = command::completions::get_pos_arg_offset(
            offset_sweep_cmd.name, completions);
        if (off != 0) {
            command::completions::modules_completions(context, offset_sweep_cmd.name,
                off, completions);

            command::completions::channels_completions(context, offset_sweep_cmd.name,
                off, off + 1, completions);
        }
    }
}
}
}
}
