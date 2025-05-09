/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file example_pixie16api.cpp
 * @brief Demonstrates how to use the Pixie16Api functions to communicate with Pixie-16 modules.
 *
 * We demonstrate both the PixieSDK implementation using `Pixie16Api.so`.
 *
 * The `Pixie16Api.so` includes all the goodness that comes with using the C++ implementation
 * just with a convenient C wrapper. Users don't have as much control over the nitty-gritty details
 * of the PixieSDK. We've intentionally limited this to prevent this API from growing out of hand.
 */

#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include <args/args.hxx>

#include <pixie16/pixie16.h>
#include <pixie/format.hpp>

#include <sys/stat.h>

#if defined(_WIN64) || defined(_WIN32)
#include <isakbosman/dirent.h>
#else
#include <dirent.h>
#endif

#define MAX_SLOTS 15
#define MSG_SIZE 1024
#define TIME_SIZE 80

using json = xia::pixie::format::json;

/*
 * List-mode/MCA run data types
 */
using run_data = uint32_t;
using fifo_data = std::vector<run_data>;
using histogram = std::vector<run_data>;
using histograms = std::vector<histogram>;

/*
 * Baseline/tau capture data types
 */
using data_value = double;
using baseline_value = std::vector<data_value>;
using baseline_values = std::vector<baseline_value>;
using timestamp_value = std::vector<data_value>;
using timestamp_values = std::vector<timestamp_value>;
using tau_values = std::vector<data_value>;

/*
 * Stats/trace capture data types
 */
using stats_values = std::vector<unsigned int>;
using trace_value = std::vector<unsigned short>;
using trace_values = std::vector<trace_value>;

/*
 * Module/channel masks
 */
using masks = std::vector<unsigned short>;

/*
 * Input/output data types
 */
using option_value = std::string;
using option_values = std::vector<option_value>;
using file_stream = std::ofstream*;
using file_streams = std::vector<file_stream>;

std::string walltime_iso_string() {
    auto now = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::to_time_t(now);
    auto now2 =
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    char timeBuffer[TIME_SIZE];
    std::strftime(timeBuffer, TIME_SIZE, "%FT%T", gmtime(&currentTime));

    std::stringstream tmp;
    tmp << timeBuffer << "." << std::setfill('0') << std::setw(3) << now2.count() % 1000 << "Z";
    return tmp.str();
}

struct logging {
    logging(const std::string& type_) {
        type = type_;
        datetime = walltime_iso_string();
    }

    std::string type;
    std::string datetime;
};

std::ostream& operator<<(std::ostream& os, const logging& log) {
    os << log.datetime << " - " << log.type << " - ";
    return os;
}

struct mod_cfg {
    ::module_config cfg;
    std::string dsp_par;
    module_fifo_config fifo_config;
    bool has_firmware_spec;
    bool has_firmware_files;
    bool has_fifo_cfg;

    mod_cfg() : has_firmware_spec(false), has_firmware_files(false), has_fifo_cfg(false) {
        std::memset(&cfg, 0, sizeof(cfg));
        std::memset(&fifo_config, 0, sizeof(fifo_config));
    }
};

using module_configs = std::vector<mod_cfg>;

struct configuration {
    module_configs modules;
    std::vector<unsigned short> slot_def;
    unsigned short num_modules() const {
        return static_cast<unsigned short>(modules.size());
    }
};

std::string generate_filename(const std::string& type, const std::string& ext,
                              const std::string& dir) {
    return dir + "pixie16api-" + type + "." + ext;
}

void verify_json_module(const json& mod) {
    if (!mod.contains("slot")) {
        throw std::invalid_argument("Missing slot definition in configuration element.");
    }

    if (!mod.contains("dsp")) {
        throw std::invalid_argument("Missing dsp object in configuration element.");
    }

    auto& dsp = mod["dsp"];
    if (!dsp.contains("par")) {
        throw std::invalid_argument(
            "Missing dsp object in configuration element: par.");
    }

    if (mod.contains("fw")) {
        auto& fw = mod["fw"];
        if (!fw.contains("adc_bits") || !fw.contains("adc_msps") || !fw.contains("fw_set") ||
            !fw.contains("revision")) {
            throw std::invalid_argument(
                "Missing firmware configuration (fw) definition (adc_bits, adc_msps, fw_set,"
                " revision).");
        }
    }

    if (mod.contains("fifo_config")) {
        auto& fifo_config = mod["fifo_config"];
        if (!fifo_config.contains("bandwidth_mb_per_sec") || !fifo_config.contains("buffers") ||
            !fifo_config.contains("dma_trigger_level_bytes") ||
            !fifo_config.contains("hold_usecs") || !fifo_config.contains("idle_wait_usecs") ||
            !fifo_config.contains("run_wait_usecs")) {
            throw std::invalid_argument(
                "Missing FIFO configuration (fifo_config) definition (bandwidth_mb_per_sec,"
                "buffers, dma_trigger_level_bytes, hold_usecs, idle_wait_usecs, run_wait_usecs).");
        }
    }
}

std::string fifo_stats_to_json(module_fifo_stats& stats) {
    json data = {
        {"timestamp", std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::system_clock::now().time_since_epoch())
                          .count()},
        {"in", stats.in},
        {"out", stats.out},
        {"dma_in", stats.dma_in},
        {"overflows", stats.overflows},
        {"dropped", stats.dropped},
        {"hw_overflows", stats.hw_overflows},
    };
    return data.dump();
}

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const bool& print_success = true) {
    if (val < 0) {
        std::string msg;
        msg.resize(MSG_SIZE);
        ::PixieGetReturnCodeText(val, &msg[0], MSG_SIZE);
        std::cout << logging("ERROR") << func_name << " failed with code " << val
                  << " and message: " << msg << std::endl;
        return false;
    }
    if (print_success) {
        std::cout << logging("INFO") << func_name << " finished successfully." << std::endl;
    }
    return true;
}

void read_config(const std::string& config_file_name, configuration& cfg) {
    std::ifstream input(config_file_name, std::ios::in);
    if (input.fail()) {
        throw std::ios_base::failure("open: " + config_file_name + ": " + std::strerror(errno));
    }

    json jf = json::parse(input);
    input.close();

    if (jf.empty() || jf.size() > MAX_SLOTS) {
        throw std::invalid_argument("invalid number of modules");
    }

    cfg.modules.clear();
    cfg.slot_def.clear();

    for (const auto& module : jf) {
        verify_json_module(module);

        cfg.slot_def.push_back(module["slot"]);

        mod_cfg mcfg;
        mcfg.cfg.slot = module["slot"];
        mcfg.cfg.number = static_cast<unsigned short>(cfg.slot_def.size() - 1);
        if (module.contains("dsp")) {
            auto& jdsp = module["dsp"];
            mcfg.dsp_par = jdsp["par"];
        }
        if (module.contains("fw")) {
            auto& jfw = module["fw"];
            if (jfw.contains("adc_msps")) {
                mcfg.cfg.adc_sampling_frequency = jfw["adc_msps"];
            }
            if (jfw.contains("adc_bits")) {
                mcfg.cfg.adc_bit_resolution = jfw["adc_bits"];
            }
            if (jfw.contains("revision")) {
                mcfg.cfg.revision = jfw["revision"];
            }
            if (jfw.contains("fw_set")) {
                size_t dev = 0;
                for (const auto& fw : jfw["fw_set"].items()) {
                    std::strncpy(
                        mcfg.cfg.fw_device[dev],
                        fw.key().c_str(),
                        sizeof(mcfg.cfg.fw_device[dev]) - 1);
                    std::strncpy(
                        mcfg.cfg.fw_device_file[dev],
                        fw.value().template get<std::string>().c_str(),
                        sizeof(mcfg.cfg.fw_device_file[dev]) - 1);
                    ++dev;
                    if (dev == PIXIE16_API_MOD_CONFIG_MAX_DEVICES) {
                        break;
                    }
                }
                mcfg.has_firmware_files = true;
            }
            mcfg.has_firmware_spec = true;
        }
        if (module.contains("fifo_config")) {
            auto& fifo_config = module["fifo_config"];
            mcfg.fifo_config.bandwidth_mb_per_sec = fifo_config["bandwidth_mb_per_sec"];
            mcfg.fifo_config.buffers = fifo_config["buffers"];
            mcfg.fifo_config.dma_trigger_level_bytes =
                fifo_config["dma_trigger_level_bytes"];
            mcfg.fifo_config.hold_usecs = fifo_config["hold_usecs"];
            mcfg.fifo_config.idle_wait_usecs = fifo_config["idle_wait_usecs"];
            mcfg.fifo_config.run_wait_usecs = fifo_config["run_wait_usecs"];
            mcfg.has_fifo_cfg = true;
        }
        cfg.modules.push_back(mcfg);
    }
}

std::string generate_hardware_statistics_header() {
    return "time,run,module,channel,real_time,live_time,input_counts,input_count_rate,output_counts,output_count_rate";
}

bool output_statistics_data(std::ofstream& stream, const mod_cfg& mod, const unsigned int run_num,
                            const bool& final_stats) {
    if (final_stats) {
        std::cout << logging("INFO") << "Requesting run statistics from module." << std::endl;
    }
    stats_values stats(::Pixie16GetStatisticsSize(), 0);
    if (!verify_api_return_value(::Pixie16ReadStatisticsFromModule(stats.data(), mod.cfg.number),
                                 "Pixie16ReadStatisticsFromModule", false)) {
        return false;
    }

    auto real_time = ::Pixie16ComputeRealTime(stats.data(), mod.cfg.number);
    json json_stats;
    if (final_stats) {
        std::cout << logging("INFO") << "Begin Statistics for Module " << mod.cfg.number << std::endl;
    }
    for (unsigned short chan = 0; chan < mod.cfg.number_of_channels; chan++) {
        auto live_time = ::Pixie16ComputeLiveTime(stats.data(), mod.cfg.number, chan);
        auto icr = ::Pixie16ComputeInputCountRate(stats.data(), mod.cfg.number, chan);
        auto ocr = ::Pixie16ComputeOutputCountRate(stats.data(), mod.cfg.number, chan);
        auto ic = ::Pixie16ComputeRawInputCount(stats.data(), mod.cfg.number, chan);
        auto oc = ::Pixie16ComputeRawOutputCount(stats.data(), mod.cfg.number, chan);

        if (final_stats) {
            json_stats = {{"run_number", run_num},
                          {"module", mod.cfg.number},
                          {"channel", chan},
                          {"real_time", real_time},
                          {"live_time", live_time},
                          {"icr", icr},
                          {"ocr", ocr},
                          {"input_counts", ic},
                          {"output_counts", oc}};
        }

        stream << walltime_iso_string() << "," << run_num << "," << mod.cfg.number << "," << chan << ","
               << std::fixed << std::setprecision(12) << real_time << "," << live_time << "," << ic
               << "," << icr << "," << oc << "," << ocr << std::endl;

        if (final_stats) {
            std::cout << logging("INFO") << json_stats << std::endl;
        }
    }
    if (final_stats) {
        std::cout << logging("INFO") << "End Statistics for Module " << mod.cfg.number << std::endl;
    }
    return true;
}


bool save_dsp_pars(const std::string& filename) {
    std::cout << logging("INFO") << "Saving DSP Parameters to " << filename << "." << std::endl;
    if (!verify_api_return_value(::Pixie16SaveDSPParametersToFile(filename.c_str()),
                                 "Pixie16SaveDSPParametersToFile")) {
        return false;
    }
    return true;
}

void export_mca_memory(const mod_cfg& mod, const std::string& filename) {
    std::cout << logging("INFO") << "Reading out on-board MCA memory." << std::endl;
    std::ofstream out(filename);
    out << "bin,";
    histograms hists;
    size_t max_histogram_length = 0;
    for (unsigned short i = 0; i < mod.cfg.number_of_channels; i++) {
        unsigned int hist_length = 0;
        ::PixieGetHistogramLength(mod.cfg.number, i, &hist_length);
        histogram hist(hist_length, 0);
        if (hist.size() > max_histogram_length) {
            max_histogram_length = hist.size();
        }

        auto hist_size = static_cast<unsigned int>(hist.size());
        ::Pixie16ReadHistogramFromModule(hist.data(), hist_size, mod.cfg.number, i);
        hists.push_back(hist);
        if (i < mod.cfg.number_of_channels - 1) {
            out << "Chan" << i << ",";
        } else {
            out << "Chan" << i;
        }
    }
    out << std::endl;

    for (size_t bin = 0; bin < max_histogram_length; bin++) {
        out << bin << ",";
        for (auto& hist : hists) {
            std::string val = " ";
            if (bin < hist.size()) {
                val = std::to_string(hist[bin]);
            }
            if (&hist != &hists.back()) {
                out << val << ",";
            } else {
                out << val;
            }
        }
        out << std::endl;
    }
}

bool execute_adjust_offsets(const mod_cfg& module) {
    std::cout << logging("INFO") << "Adjusting baseline offset for Module " << module.cfg.number << "."
              << std::endl;
    if (!verify_api_return_value(::Pixie16AdjustOffsets(module.cfg.number),
                                 "Pixie16AdjustOffsets for Module " +
                                 std::to_string(module.cfg.number))) {
        return false;
    }
    if (!save_dsp_pars(module.dsp_par)) {
        return false;
    }
    return true;
}

bool execute_baseline_capture(const mod_cfg& mod, const std::string& dir) {
    std::cout << logging("INFO") << "Starting baseline capture for Module " << mod.cfg.number << std::endl;
    if (!verify_api_return_value(::Pixie16AcquireBaselines(mod.cfg.number), "Pixie16AcquireBaselines")) {
        return false;
    }

    baseline_values baselines;
    timestamp_values timestamps;
    unsigned int max_num_baselines = 0;
    for (unsigned short i = 0; i < mod.cfg.number_of_channels; i++) {
        ::PixieGetMaxNumBaselines(mod.cfg.number, i, &max_num_baselines);
        baseline_value baseline(max_num_baselines);
        timestamp_value timestamp(max_num_baselines);

        std::cout << logging("INFO") << "Acquiring " << max_num_baselines << " baselines for Channel "
                  << i << std::endl;
        if (!verify_api_return_value(::Pixie16ReadSglChanBaselines(baseline.data(), timestamp.data(),
                                                                 max_num_baselines, mod.cfg.number, i),
                                     "Pixie16ReadSglChanBaselines")) {
            return false;
        }
        baselines.push_back(baseline);
        timestamps.push_back(timestamp);
    }

    std::ofstream ofstream1(
        generate_filename("module" + std::to_string(mod.cfg.number) + "-baselines", "csv", dir));
    ofstream1 << "bin,timestamp,";
    for (unsigned short i = 0; i < mod.cfg.number_of_channels; i++) {
        if (i != mod.cfg.number_of_channels - 1) {
            ofstream1 << "Chan" << i << ",";
        } else {
            ofstream1 << "Chan" << i;
        }
    }
    ofstream1 << std::endl;

    for (unsigned int i = 0; i < max_num_baselines; i++) {
        ofstream1 << i << "," << timestamps[0][i] << ",";
        for (unsigned short k = 0; k < mod.cfg.number_of_channels; k++) {
            if (k != mod.cfg.number_of_channels - 1) {
                ofstream1 << baselines[k][i] << ",";
            } else {
                ofstream1 << baselines[k][i];
            }
        }
        ofstream1 << std::endl;
    }

    ofstream1.close();
    return true;
}

bool write_synch_parameters(unsigned int synch_wait, unsigned int in_synch) {
    std::cout << logging("INFO") << "Calling Pixie16WriteSglModPar to write SYNCH_WAIT = " << synch_wait
              << " in Module 0." << std::endl;
    if (!verify_api_return_value(::Pixie16WriteSglModPar("SYNCH_WAIT", synch_wait, 0),
                                 "Pixie16WriteSglModPar - SYNC_WAIT")) {
        return false;
    }

    std::cout << logging("INFO") << "Calling Pixie16WriteSglModPar to write IN_SYNCH  = " << in_synch
              << " in Module 0." << std::endl;
    if (!verify_api_return_value(::Pixie16WriteSglModPar("IN_SYNCH", in_synch, 0),
                                 "Pixie16WriteSglModPar - IN_SYNC")) {
        return false;
    }
    return true;
}

bool check_run_completion(const configuration& cfg) {
    std::cout << logging("INFO") << "Checking that the run is finalized in all the modules."
              << std::endl;
    bool all_modules_finished;
    const unsigned int max_finalize_attempts = 50;
    for (unsigned int counter = 0; counter < max_finalize_attempts; counter++) {
        all_modules_finished = true;
        for (unsigned short k = 0; k < cfg.num_modules(); k++) {
            if (::Pixie16CheckRunStatus(k) == 1) {
                all_modules_finished = false;
            }
        }
        if (all_modules_finished) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!all_modules_finished) {
        std::cout << logging("ERROR") << "All modules did not stop their runs properly!" << std::endl;
        return false;
    } else {
        return true;
    }
}

bool execute_list_mode_run(unsigned int run_num, const configuration& cfg,
                           const double& runtime_in_seconds, unsigned int synch_wait,
                           unsigned int in_synch, const std::string& dir) {
    std::cout << logging("INFO") << "Starting list mode data run for " << runtime_in_seconds << " s."
              << std::endl;

    if (!write_synch_parameters(synch_wait, in_synch)) {
        return false;
    }

    std::cout << logging("INFO") << "Starting list-mode run." << std::endl;
    if (!verify_api_return_value(::Pixie16StartListModeRun(cfg.num_modules(), 0x100, 1),
                                 "Pixie16StartListModeRun")) {
        return false;
    }

    file_streams record_streams(cfg.num_modules());
    file_streams fifo_stat_streams(cfg.num_modules());

    std::ofstream hw_stats_output(generate_filename("list-mode-hw-stats", "csv", dir),
                                  std::ios::out);
    hw_stats_output << generate_hardware_statistics_header() << std::endl;

    for (unsigned short i = 0; i < cfg.num_modules(); i++) {
        record_streams[i] =
            new std::ofstream(generate_filename("list-mode-module" + std::to_string(i) + "-run" +
                                                    std::to_string(run_num) + "-recs",
                                                "bin", dir),
                              std::ios::out | std::ios::binary);
        fifo_stat_streams[i] =
            new std::ofstream(generate_filename("list-mode-module" + std::to_string(i) + "-run" +
                                                    std::to_string(run_num) + "-fifo-stats",
                                                "jsonl", dir),
                              std::ios::out);
    }

    unsigned int num_fifo_words = 0;
    ::module_fifo_stats fifo_stats{};

    std::cout << logging("INFO") << "Collecting data for " << runtime_in_seconds << " s." << std::endl;
    auto run_start_time = std::chrono::steady_clock::now();
    double current_run_time = 0;
    double check_time = 0;
    bool run_status = ::Pixie16CheckRunStatus(cfg.modules[0].cfg.number);

    while (run_status != 0) {
        current_run_time = std::chrono::duration_cast<std::chrono::duration<double>>(
                               std::chrono::steady_clock::now() - run_start_time)
                               .count();

        if (current_run_time >= runtime_in_seconds) {
            if (synch_wait == 0) {
                std::cout << logging("INFO") << "Stopping list-mode run individually." << std::endl;
                if (!verify_api_return_value(::Pixie16EndRun(cfg.num_modules()), "Pixie16EndRun")) {
                    return false;
                }
            } else {
                /*
                 * Stop run in the Director module (module #0) - a SYNC interrupt should be generated
                 *  to stop run in all modules simultaneously
                 */
                std::cout << logging("INFO") << "Stopping list-mode run in director module."
                          << std::endl;
                if (!verify_api_return_value(::Pixie16EndRun(cfg.modules[0].cfg.number), "Pixie16EndRun")) {
                    return false;
                }
            }

            break;
        }

        if (current_run_time - check_time > 1) {
            if (current_run_time < runtime_in_seconds) {
                std::cout << logging("INFO") << "Remaining run time: "
                          << std::round(runtime_in_seconds - current_run_time) << " s" << std::endl;
            }
            check_time = current_run_time;
        }

        for (unsigned short mod_num = 0; mod_num < cfg.num_modules(); mod_num++) {
            if (::Pixie16CheckRunStatus(mod_num) == 1) {
                if (!verify_api_return_value(
                        ::Pixie16CheckExternalFIFOStatus(&num_fifo_words, mod_num),
                        "Pixie16CheckExternalFIFOStatus", false)) {
                    return false;
                }


                std::cout << logging("INFO") << "Module " << mod_num << " FIFO has " << num_fifo_words
                          << " words." << std::endl;
                /*
                 * NOTE: The PixieSDK now uses threaded list-mode FIFO workers that live on the host machine. These
                 * workers execute in parallel. They'll read the data from each module as needed to
                 * ensure that the EXTERNAL_FIFO_LENGTH isn't exceeded. When calling
                 * `Pixie16CheckExternalFIFOStatus`, you're actually checking the status of the worker's data buffer.
                 */
                if (num_fifo_words > 0) {
                    fifo_data data(num_fifo_words, 0xDEADBEEF);
                    if (!verify_api_return_value(
                            ::Pixie16ReadDataFromExternalFIFO(data.data(), num_fifo_words, mod_num),
                            "Pixie16ReadDataFromExternalFIFO", false)) {
                        return false;
                    }
                    record_streams[mod_num]->write(reinterpret_cast<char*>(data.data()),
                                                   num_fifo_words * sizeof(data_value ));
                }

                /*
                 * The PixieSDK tracks list-mode data statistics. It keeps track of
                 * how much data goes into the FIFO and how much data gets read out.
                 */
                if (!verify_api_return_value(::PixieReadRunFifoStats(mod_num, &fifo_stats),
                                             "PixieReadRunFifoStats", false)) {
                    return false;
                }
                *fifo_stat_streams[mod_num] << fifo_stats_to_json(fifo_stats) << std::endl;

                if (!output_statistics_data(hw_stats_output, cfg.modules[mod_num], run_num, false)) {
                    return false;
                }
            } else {
                std::cout << logging("INFO") << "Module " << mod_num << " has no active run!"
                          << std::endl;
            }
        }

        /*
         * Check the run status of the Director module (module #0) to see if the run has been stopped.
         * This is possible in a multi-chassis system where modules in one chassis can stop the run
         * in all chassis.
         */
        run_status = ::Pixie16CheckRunStatus(cfg.modules[0].cfg.number);
    }

    if (!check_run_completion(cfg)) {
        return false;
    }

    std::cout << logging("INFO") << "List-mode run finished in "
              << std::chrono::duration_cast<std::chrono::duration<double>>(
                     std::chrono::steady_clock::now() - run_start_time)
                     .count()
              << " s" << std::endl
              << "Reading the final words from the External FIFO and the run statistics."
              << std::endl;
    for (unsigned short mod_num = 0; mod_num < cfg.num_modules(); mod_num++) {
        while (true) {
            if (!verify_api_return_value(::Pixie16CheckExternalFIFOStatus(&num_fifo_words, mod_num),
                                         "Pixie16CheckExternalFIFOStatus", false)) {
                return false;
            }

            std::cout << logging("INFO") << "External FIFO has " << num_fifo_words << " words."
                      << std::endl;

            if (num_fifo_words == 0) {
                break;
            }

            fifo_data data(num_fifo_words, 0xDEADBEEF);
            if (!verify_api_return_value(
                    ::Pixie16ReadDataFromExternalFIFO(data.data(), num_fifo_words, mod_num),
                    "Pixie16ReadDataFromExternalFIFO", false)) {
                return false;
            }
            record_streams[mod_num]->write(reinterpret_cast<char*>(data.data()),
                                           num_fifo_words * sizeof(data_value));

            if (!verify_api_return_value(::PixieReadRunFifoStats(mod_num, &fifo_stats),
                                         "PixieReadRunFifoStats", false)) {
                return false;
            }
            *fifo_stat_streams[mod_num] << fifo_stats_to_json(fifo_stats) << std::endl;
        }

        if (!output_statistics_data(hw_stats_output, cfg.modules[mod_num], run_num, true)) {
            return false;
        }

        std::string name = generate_filename("list-mode-module" + std::to_string(mod_num) + "-run" +
                                                 std::to_string(run_num) + "-mca",
                                             "csv", dir);
        export_mca_memory(cfg.modules[mod_num], name);
    }

    for (auto& stream : record_streams) {
        stream->close();
    }

    for (auto& stream : fifo_stat_streams) {
        stream->close();
    }

    hw_stats_output.close();
    return true;
}

bool execute_list_mode_runs(const unsigned int num_runs, const configuration& cfg,
                            const double& runtime_in_seconds, unsigned int synch_wait,
                            unsigned int in_synch, const std::string& dir) {
    for (unsigned int i = 0; i < num_runs; i++) {
        std::cout << logging("INFO") << "Starting list-mode run number " << i << std::endl;
        if (!execute_list_mode_run(i, cfg, runtime_in_seconds, synch_wait, in_synch, dir)) {
            std::cout << logging("INFO") << "List-mode data run " << i
                      << " failed! See log for more details." << std::endl;
            return false;
        }
        std::cout << logging("INFO") << "Finished list-mode run number " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return true;
}

bool execute_mca_run(unsigned int run_num, const configuration& cfg,
                     const double& runtime_in_seconds, unsigned int synch_wait,
                     unsigned int in_synch, const std::string& dir) {

    if (!write_synch_parameters(synch_wait, in_synch)) {
        return false;
    }

    std::cout << logging("INFO") << "Calling Pixie16WriteSglModPar to write HOST_RT_PRESET to "
              << runtime_in_seconds << std::endl;
    for (unsigned short i = 0; i < cfg.num_modules(); i++) {
        if (!verify_api_return_value(::Pixie16WriteSglModPar("HOST_RT_PRESET",
                                                           Decimal2IEEEFloating(runtime_in_seconds),
                                                           i),
                                     "Pixie16WriteSglModPar - HOST_RT_PRESET")) {
            return false;
        }
    }

    std::cout << logging("INFO") << "Starting MCA data run for " << runtime_in_seconds << " s."
              << std::endl;
    if (!verify_api_return_value(::Pixie16StartHistogramRun(cfg.num_modules(), 1),
                                 "Pixie16StartHistogramRun")) {
        return false;
    }

    auto run_start_time = std::chrono::steady_clock::now();
    double current_run_time = 0;
    double check_time = 0;
    bool run_status = ::Pixie16CheckRunStatus(0);
    bool forced_end = false;
    while (run_status != 0 && !forced_end) {
        current_run_time = std::chrono::duration_cast<std::chrono::duration<double>>(
                               std::chrono::steady_clock::now() - run_start_time)
                               .count();

        if (current_run_time - check_time > 1) {
            run_status = ::Pixie16CheckRunStatus(0);
            if (current_run_time < runtime_in_seconds) {
                std::cout << logging("INFO")
                          << "Remaining run time: " << runtime_in_seconds - current_run_time << " s"
                          << std::endl;
            }
            check_time = current_run_time;
        }
        if (current_run_time > runtime_in_seconds + 5) {
            std::cout << logging("INFO") << "Forcing end of run in Module 0" << std::endl;
            if (!verify_api_return_value(::Pixie16EndRun(cfg.num_modules()), "Pixie16EndRun")) {
                return false;
            }
            forced_end = true;
        }
    }

    if (current_run_time < runtime_in_seconds) {
        std::cout << logging("ERROR") << "MCA Run exited prematurely! Check log for more details."
                  << std::endl;
    }

    if (forced_end) {
        std::cout << logging("ERROR") << "MCA Run was forced to end!" << std::endl;
    }

    if (!check_run_completion(cfg)) {
        return false;
    } else {
        std::cout << logging("INFO") << "MCA Run finished!" << std::endl;
    }

    std::ofstream hw_stats_output(generate_filename("mca-run-hw-stats", "csv", dir), std::ios::out);
    hw_stats_output << generate_hardware_statistics_header() << std::endl;

    for (unsigned short i = 0; i < cfg.num_modules(); i++) {
        std::string name = generate_filename(
            "module" + std::to_string(i) + "-mca-run" + std::to_string(run_num), "csv", dir);
        export_mca_memory(cfg.modules[i], name);

        if (!output_statistics_data(hw_stats_output, cfg.modules[i], run_num, true)) {
            return false;
        }
    }
    hw_stats_output.close();
    return true;
}

bool execute_mca_runs(const unsigned int num_runs, const configuration& cfg,
                      const double& runtime_in_seconds, unsigned int synch_wait,
                      unsigned int in_synch, const std::string& dir) {
    for (unsigned int i = 0; i < num_runs; i++) {
        std::cout << logging("INFO") << "Starting MCA run number " << i << std::endl;
        if (!execute_mca_run(i, cfg, runtime_in_seconds, synch_wait, in_synch, dir)) {
            std::cout << logging("INFO") << "MCA data run " << i
                      << " failed! See log for more details.";
            return false;
        }
        std::cout << logging("INFO") << "Finished MCA run number " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return true;
}

bool execute_parameter_read(args::ValueFlag<std::string>& parameter,
                            args::ValueFlag<unsigned int>& crate, const unsigned int module,
                            args::ValueFlag<unsigned int>& channel) {
    if (channel) {
        double result;
        std::cout << logging("INFO") << "Pixie16ReadSglChanPar"
                  << " reading " << parameter.Get() << " from Crate " << crate.Get() << " Module "
                  << module << " Channel " << channel.Get() << "." << std::endl;
        if (!verify_api_return_value(
                ::Pixie16ReadSglChanPar(parameter.Get().c_str(), &result, module, channel.Get()),
                "Pixie16ReadSglChanPar", false)) {
            return false;
        }
        std::cout << logging("INFO") << parameter.Get() << "=" << result << std::endl;
    } else {
        unsigned int result;
        std::cout << logging("INFO") << "Pixie16ReadSglModPar reading " << parameter.Get()
                  << " from Crate " << crate.Get() << " Module " << module << "." << std::endl;
        if (!verify_api_return_value(::Pixie16ReadSglModPar(parameter.Get().c_str(), &result, module),
                                     "Pixie16ReadSglModPar", false)) {
            return false;
        }
        std::cout << logging("INFO") << parameter.Get() << "=" << result << std::endl;
    }
    return true;
}

bool execute_parameter_write(args::ValueFlag<std::string>& parameter,
                             args::ValueFlag<double>& value, args::ValueFlag<unsigned int>& crate,
                             const mod_cfg& module, args::ValueFlag<unsigned int>& channel) {
    std::cout << logging("INFO") << "Checking current value for " << parameter.Get() << std::endl;
    execute_parameter_read(parameter, crate, module.cfg.number, channel);
    if (channel) {
        std::cout << logging("INFO") << "Pixie16WriteSglChanPar setting " << parameter.Get() << " to "
                  << value.Get() << " for Crate " << crate.Get() << " Module " << module.cfg.number
                  << " Channel " << channel.Get() << "." << std::endl;
        if (!verify_api_return_value(::Pixie16WriteSglChanPar(parameter.Get().c_str(), value.Get(),
                                                            module.cfg.number, channel.Get()),
                                     "Pixie16WriteSglChanPar")) {
            return false;
        }
    } else {
        std::cout << logging("INFO") << "Pixie16WriteSglModPar"
                  << " setting " << parameter.Get() << " to " << value.Get() << " for  Crate "
                  << crate.Get() << " Module " << module.cfg.number << "." << std::endl;
        if (!verify_api_return_value(::Pixie16WriteSglModPar(parameter.Get().c_str(),
                                                                 static_cast<unsigned int>(value.Get()),
                                                                 module.cfg.number),
                                     "Pixie16WriteSglModPar")) {
            return false;
        }
    }

    std::cout << logging("INFO") << "Verifying written value for " << parameter.Get()
              << std::endl;
    execute_parameter_read(parameter, crate, module.cfg.number, channel);

    if (!save_dsp_pars(module.dsp_par)) {
        return false;
    }
    return true;
}

bool execute_trace_capture(const mod_cfg& mod, const std::string& dir) {
    std::cout << logging("INFO") << "Pixie16AcquireADCTrace acquiring traces for Module " << mod.cfg.number
              << "." << std::endl;
    if (!verify_api_return_value(::Pixie16AcquireADCTrace(mod.cfg.number), "Pixie16AcquireADCTrace")) {
        return false;
    }

    std::ofstream ofstream1(
        generate_filename("module" + std::to_string(mod.cfg.number) + "-adc", "csv", dir));
    ofstream1 << "bin,";

    size_t max_trace_length = 0;
    trace_values traces;
    for (unsigned short i = 0; i < mod.cfg.number_of_channels; i++) {
        unsigned int trace_length = 0;
        if (!verify_api_return_value(::PixieGetTraceLength(mod.cfg.number, i, &trace_length),
                                     "Pixie16GetTraceLength", false)) {
            return false;
        }
        trace_value trace(trace_length, 0);
        if (trace.size() > max_trace_length) {
            max_trace_length = trace.size();
        }

        auto trace_size = static_cast<unsigned int>(trace.size());
        if (!verify_api_return_value(::Pixie16ReadSglChanADCTrace(trace.data(),
                                                                      trace_size,
                                                                      mod.cfg.number, i),
                                     "Pixie16AcquireADCTrace", false)) {
            return false;
        }
        traces.push_back(trace);

        if (i != mod.cfg.number_of_channels - 1) {
            ofstream1 << "Chan" << i << ',';
        } else {
            ofstream1 << "Chan" << i;
        }
    }
    ofstream1 << std::endl;

    for (size_t idx = 0; idx < max_trace_length; idx++) {
        ofstream1 << idx << ",";
        for (auto& trace : traces) {
            std::string val = " ";
            if (idx < trace.size()) {
                val = std::to_string(trace[idx]);
            }
            if (&trace != &traces.back()) {
                ofstream1 << val << ",";
            } else {
                ofstream1 << val;
            }
        }
        ofstream1 << std::endl;
    }
    ofstream1.close();
    return true;
}

bool execute_blcut(mod_cfg& module) {
    std::cout << logging("INFO") << "Executing Pixie16BLcutFinder for Module" << module.cfg.number << "."
              << std::endl;
    unsigned int blcut = 0;
    for (int ch = 0; ch < module.cfg.number_of_channels; ch++) {
        if (!verify_api_return_value(::Pixie16BLcutFinder(module.cfg.number, ch, &blcut),
                                     "Pixie16BLcutFinder", false)) {
            return false;
        }
        std::cout << logging("INFO") << "BLCut for Module " << module.cfg.number << " Channel " << ch
                  << " is " << blcut << std::endl;
    }
    return true;
}

bool execute_find_tau(const mod_cfg& module) {
    std::cout << logging("INFO") << "Executing Pixie16TauFinder for Module" << module.cfg.number << "."
              << std::endl;

    tau_values taus(module.cfg.number_of_channels);
    if (!verify_api_return_value(::Pixie16TauFinder(module.cfg.number, taus.data()), "Pixie16TauFinder",
                                 true)) {
        return false;
    }
    for (size_t i = 0; i < taus.size(); i++) {
        std::cout << "Channel " << i << ": " << taus.at(i) << std::endl;
    }
    return true;
}

bool execute_set_dacs(const mod_cfg& module) {
    std::cout << logging("INFO") << "Executing Pixie16SetDACs for Module" << module.cfg.number << "."
              << std::endl;
    if (!verify_api_return_value(::Pixie16SetDACs(module.cfg.number), "Pixie16SetDACs", false)) {
        return false;
    }
    return true;
}

bool execute_close_module_connection(const int& numModules) {
    std::cout << logging("INFO") << "Closing out connection to Modules." << std::endl;
    verify_api_return_value(::Pixie16ExitSystem(numModules), "Pixie16ExitSystem");
    return true;
}

double calculate_duration_in_seconds(const std::chrono::system_clock::time_point& start,
                                     const std::chrono::system_clock::time_point& end) {
    return std::chrono::duration<double>(end - start).count();
}

void output_module_fifo_info(const unsigned short& mod_num) {
    module_fifo_config fifo_config;
    if (!verify_api_return_value(::PixieGetFifoConfiguration(mod_num, &fifo_config),
                                 "PixieGetFifoConfiguration", false))
        throw std::runtime_error("Could not get FIFO information for Module " +
                                 std::to_string(mod_num));
    std::cout << logging("INFO") << "Begin List-Mode FIFO worker information for Module " << mod_num
              << std::endl;
    std::cout << logging("INFO") << "Bandwidth (MB/sec): " << fifo_config.bandwidth_mb_per_sec
              << std::endl;
    std::cout << logging("INFO") << "Buffers : " << fifo_config.buffers << std::endl;
    std::cout << logging("INFO") << "DMA Trigger Level (B): " << fifo_config.dma_trigger_level_bytes
              << std::endl;
    std::cout << logging("INFO") << "Hold (usec): " << fifo_config.hold_usecs << std::endl;
    std::cout << logging("INFO") << "Idle wait (usec): " << fifo_config.idle_wait_usecs << std::endl;
    std::cout << logging("INFO") << "Run wait (usec): " << fifo_config.run_wait_usecs << std::endl;
    std::cout << logging("INFO") << "End List-Mode FIFO worker information for Module " << mod_num
              << std::endl;
}

void output_module_info(mod_cfg& mod) {
    ::module_config mcfg{};
    if (!verify_api_return_value(::PixieGetModuleInfo(mod.cfg.number, &mcfg), "PixieGetModuleInfo",
                                 false)) {
        throw std::runtime_error("Could not get module information for Module " +
                                 std::to_string(mod.cfg.number));
    }
    if (!mod.cfg.adc_bit_resolution) {
        mod.cfg.adc_bit_resolution = mcfg.adc_bit_resolution;
    }
    if (!mod.cfg.adc_sampling_frequency) {
        mod.cfg.adc_sampling_frequency = mcfg.adc_sampling_frequency;
    }
    mod.cfg.number_of_channels = mcfg.number_of_channels;
    if (!mod.cfg.revision) {
        mod.cfg.revision = mcfg.revision;
    }
    mod.cfg.serial_number = mcfg.serial_number;
    std::cout << logging("INFO") << "Begin module information for Module " << mcfg.number << std::endl
              << logging("INFO") << "Serial Number: " << mcfg.serial_number << std::endl
              << logging("INFO") << "Revision: " << mcfg.revision << std::endl
              << logging("INFO") << "ADC Bits: " << mcfg.adc_bit_resolution << std::endl
              << logging("INFO") << "ADC MSPS: " << mcfg.adc_sampling_frequency << std::endl
              << logging("INFO") << "Num Channels: " << mcfg.number_of_channels << std::endl
              << logging("INFO") << "End module information for Module " << mcfg.number << std::endl;
}

bool boot_crate(const std::string& par_file, unsigned int boot_pattern) {
    auto start = std::chrono::system_clock::now();
    std::cout << logging("INFO") << "Calling PixieBootCrate with settings: " << par_file << std::endl;
    if (par_file.empty()) {
        std::cout << logging("WARN") << "No settings file provided" << std::endl;
    }

    PIXIE_BOOT_MODE boot_mode;
    switch (boot_pattern) {
        case 0x00:
            boot_mode = PIXIE_BOOT_PROBE;
            break;
        case 0x70:
            boot_mode = PIXIE_BOOT_SETTINGS_LOAD;
            break;
        case 0x0F:
        case 0x7F:
            boot_mode = PIXIE_BOOT_RESET_LOAD;
            break;
        default:
            std::cout << logging("ERROR") << "boot mode " << std::hex << boot_pattern
                      << " not supported" << std::endl;
            return false;
    }

    if (boot_mode == PIXIE_BOOT_SETTINGS_LOAD && par_file.empty()) {
        std::cout << logging("ERROR") << "settings file required for boot pattern: 0x"
                  << std::hex << boot_pattern << std::endl;
        return false;
    }

    int rc;
    if (!par_file.empty()) {
        rc = ::PixieBootCrate(par_file.c_str(), boot_mode);
    } else {
        rc = ::PixieBootCrate(nullptr, boot_mode);
    }

    if (!verify_api_return_value(rc, "PixieBootCrate", false)) {
        return false;
    }
    std::cout << logging("INFO") << "Finished PixieBootCrate in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s."
              << std::endl;
    return true;
}

bool init_system(unsigned int num_mods, int offline_mode) {
    auto start = std::chrono::system_clock::now();
    std::cout << logging("INFO") << "Calling Pixie16InitSystem." << std::endl;
    if (!verify_api_return_value(
            Pixie16InitSystem(num_mods, nullptr, offline_mode),
            "Pixie16InitSystem", false))
        return false;

    std::cout << logging("INFO") << "Finished Pixie16InitSystem in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s."
              << std::endl;
    return true;
}

bool init_system(configuration& cfg, int offline_mode) {
    auto start = std::chrono::system_clock::now();
    std::cout << logging("INFO") << "Calling Pixie16InitSystem." << std::endl;
    if (!verify_api_return_value(
            ::Pixie16InitSystem(cfg.num_modules(), cfg.slot_def.data(), offline_mode),
            "Pixie16InitSystem", false)) {
        return false;
    }

    std::cout << logging("INFO") << "Finished Pixie16InitSystem in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s."
              << std::endl;
    return true;
}

bool set_fifo_configs(configuration& cfg) {
    try {
        for (auto& mod : cfg.modules) {
            if (mod.has_fifo_cfg) {
                if (!verify_api_return_value(
                        ::PixieSetFifoConfiguration(mod.cfg.number, &mod.fifo_config),
                        "PixieSetFifoConfiguration", false)) {
                    return false;
                }
            }
            output_module_info(mod);
            output_module_fifo_info(mod.cfg.number);
        }
    } catch (std::runtime_error& error) {
        std::cout << logging("ERROR") << error.what() << std::endl;
        return false;
    }
    return true;
}

bool boot_module(const mod_cfg& mod, unsigned int boot_pattern) {
    auto start = std::chrono::system_clock::now();
    std::cout << logging("INFO") << "Calling Pixie16BootModule for Module " << mod.cfg.number
              << " with boot pattern: " << std::showbase << std::hex << boot_pattern << std::dec
              << std::endl;

    if (!verify_api_return_value(
            ::Pixie16BootModuleFirmware(
                mod.dsp_par.c_str(), mod.cfg.number,
                boot_pattern),
            "Pixie16BootModuleFirmware", "Finished booting!")) {
        return false;
    }
    std::cout << logging("INFO") << "Finished Pixie16BootModule for Module " << mod.cfg.number << " in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s."
              << std::endl;
    return true;
}

bool register_firmware(const mod_cfg& mod, const char* filepath, const char* device) {
    if (strlen(filepath) > 0) {
        std::cout << logging("INFO")
                  << "Calling Pixie16RegisterFirmware for Module "
                  << mod.cfg.number << ": " << device << std::endl;
        int rc = ::PixieRegisterFirmware(
            0, mod.cfg.revision, mod.cfg.adc_sampling_frequency,
            mod.cfg.adc_bit_resolution, device, filepath, mod.cfg.number);
        if (!verify_api_return_value(rc, "Pixie16RegisterFirmware", false)) {
            return false;
        }
    }

    return true;
}

bool set_firmware(const mod_cfg& mod, const char* filepath, const char* device) {
    if (strlen(filepath) > 0) {
        std::cout << logging("INFO")
                  << "Calling PixieSetModuleFirmware for Module "
                  << mod.cfg.number << ": " << device << std::endl;
        if (!verify_api_return_value(
                Pixie16SetModuleFirmware(filepath, mod.cfg.slot, device),
                "Pixie16SetModuleFirmware", false)) {
            return false;
        }
    }

    return true;
}

bool is_directory(std::string& direc) {
    struct stat info{};
    const char* dir = direc.c_str();
    if (stat(dir, &info) != 0) {
        std::cout << logging("ERROR") << "cannot access " << dir << std::endl;
        return false;
    }
    if ((info.st_mode & S_IFDIR) == 0) {
        std::cout << logging("ERROR") << dir << " is not a valid directory." << std::endl;
        return false;
    }
    std::cout << logging("INFO") << dir << " is a valid directory." << std::endl;
    return true;
}

bool execute_boot(configuration& cfg, args::ValueFlag<std::string>& boot_flag,
                  args::ValueFlag<std::string>& add_cfg, args::ValueFlag<std::string>& basepath) {
    unsigned int boot_pattern = stoul(args::get(boot_flag), nullptr, 0);

    if (add_cfg) {
        boot_pattern = 0x70;
    }

    if (basepath) {
        int rc = ::Pixie16LoadModuleFirmware(basepath.Get().c_str());
        if (!verify_api_return_value(rc, "PixieLoadModuleFirmware", false)) {
            return false;
        }
    }
    std::cout << logging("INFO") << "Getting firmware from configuration file:" << std::endl;

    for (const auto& mod : cfg.modules) {
        if (mod.has_firmware_files) {
            if (mod.has_firmware_spec) {
                for (size_t dev = 0;
                     dev < PIXIE16_API_MOD_CONFIG_MAX_DEVICES && strcmp(mod.cfg.fw_device[dev], "") != 0;
                     ++dev) {
                    if (!register_firmware(mod, mod.cfg.fw_device_file[dev], mod.cfg.fw_device[dev])) {
                        return false;
                    }
                }
            } else {
                for (size_t dev = 0;
                     dev < PIXIE16_API_MOD_CONFIG_MAX_DEVICES && strcmp(mod.cfg.fw_device[dev], "") != 0;
                     ++dev) {
                    if (!set_firmware(mod, mod.cfg.fw_device_file[dev], mod.cfg.fw_device[dev])) {
                        return false;
                    }
                }
            }
        }
    }

    if (!boot_crate(cfg.modules.front().dsp_par, boot_pattern)) {
        return false;
    }

    return true;
}

bool execute_sysctl(std::string query, std::string format) {
    const char* queryp = nullptr;
    if (!query.empty()) {
        queryp = query.c_str();
    }
    auto mib_format = PIXIE_SYSCTL_FORMAT_TEXT;
    if (format == "text") {
        /* alreadtt set, do nothing */
    } else if (format == "json") {
        mib_format = PIXIE_SYSCTL_FORMAT_JSON;
    }
    int rc = PixieSysControlOpen(queryp, mib_format);
    if (!verify_api_return_value(rc, "PixieSysControlOpen", false)) {
        return false;
    }
    char buf[1024 + 1];
    size_t size = 0;
    rc = PixieSysControlSize(&size);
    std::cout << "MIB size: " << size << std::endl;
    while (size != 0) {
        size = sizeof(buf) - 1;
        rc = PixieSysControlRead(&buf[0], &size);
        if (!verify_api_return_value(rc, "PixieSysControlRead", false)) {
            return false;
        }
        if (size != 0) {
            buf[size] = '\0';
            std::cout << buf;
        }
    }
    rc = PixieSysControlClose();
    if (!verify_api_return_value(rc, "PixieSysControlClose", false)) {
        return false;
    }
    if (mib_format == PIXIE_SYSCTL_FORMAT_JSON) {
        std::cout << std::endl;
    }
    return true;
}

bool execute_sysctl_config(std::string file, std::string format) {
    auto mib_format = PIXIE_SYSCTL_FORMAT_TEXT;
    if (format == "text") {
        /* alreadtt set, do nothing */
    } else if (format == "json") {
        mib_format = PIXIE_SYSCTL_FORMAT_JSON;
    }
    int rc = PixieSysControlSetFileValues(file.c_str(), mib_format);
    if (!verify_api_return_value(rc, "PixieSysControlSetFileValues", false)) {
        return false;
    }
    return true;
}

bool execute_sysctl_get(std::string val_name, std::string val_type) {
    if (val_type == "string") {
        char val[256];
        int rc = PixieSysControlGet(val_name.c_str(), val, sizeof(val));
        if (!verify_api_return_value(rc, "PixieSysControlGet", false)) {
            return false;
        }
        std::cout << val_name << " = " << val << std::endl;
    } else if (val_type == "integer") {
        int val;
        int rc = PixieSysControlGetInt(val_name.c_str(), &val);
        if (!verify_api_return_value(rc, "PixieSysControlGetInt", false)) {
            return false;
        }
        std::cout << val_name << " = " << val << std::endl;
    } else if (val_type == "real") {
        double val;
        int rc = PixieSysControlGetDouble(val_name.c_str(), &val);
        if (!verify_api_return_value(rc, "PixieSysControlGetDouble", false)) {
            return false;
        }
        std::cout << val_name << " = " << val << std::endl;
    }
    return true;
}

bool execute_sysctl_set(std::string val_name, std::string val_type, std::string val) {
    if (val_type == "string") {
        int rc = PixieSysControlSet(val_name.c_str(), val.c_str());
        if (!verify_api_return_value(rc, "PixieSysControlSet", false)) {
            return false;
        }
    } else if (val_type == "integer") {
        int ival = std::atoi(val.c_str());
        int rc = PixieSysControlSetInt(val_name.c_str(), ival);
        if (!verify_api_return_value(rc, "PixieSysControlSetInt", false)) {
            return false;
        }
    } else if (val_type == "real") {
        double rval = std::strtod(val.c_str(), nullptr);
        int rc = PixieSysControlSetDouble(val_name.c_str(), rval);
        if (!verify_api_return_value(rc, "PixieSysControlSetDouble", false)) {
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    auto start = std::chrono::system_clock::now();
    args::ArgumentParser parser(
        "Sample code that interfaces with a Pixie system through the User API.");
    parser.LongSeparator("=");

    args::Group commands(parser, "commands");
    args::Command boot(commands, "boot", "Boots the crate of modules.");
    args::Command copy(commands, "copy", "Copies DSP parameters from source to destination.");
    args::Command export_settings(
        commands, "export-settings",
        "Boots the system and dumps the settings to the file defined in the config.");
    args::Command mca_export(commands, "mca-export",
                             "Exports histograms from the module without executing a run.");
    args::Command init(commands, "init", "Initializes the SDK and prints module information.");
    args::Command list_mode(commands, "list-mode", "Starts a list mode data run");
    args::Command read(commands, "read", "Read a parameter from the module.");
    args::Command write(commands, "write", "Write a parameter to the module.");
    args::Command trace(commands, "trace", "Captures ADC traces from the modules.");
    args::Command adjust_offsets(commands, "adjust_offsets",
                                 "Adjusts the DC offsets for all modules in the config file.");
    args::Command baseline(commands, "baseline", "Acquire and print baselines from the module");
    args::Command mca(commands, "mca", "Starts an MCA data run.");
    args::Command blcut(commands, "blcut",
                        "Starts a control task to find the BLCut for a channel.");
    args::Command dacs(commands, "dacs", "Starts a control task to set the module's DACs");
    args::Command tau_finder(commands, "tau_finder",
                             "Executes the Tau Finder control task and returns the values.");
    args::Command sysctl(commands, "sysctl", "System Control query using MIB path.");
    args::Command sysctl_config(commands, "sysctl-config", "System Control configuration from a file of values.");
    args::Command sysctl_get(commands, "sysctl-get", "System Control command to get a MIB.");
    args::Command sysctl_set(commands, "sysctl-set", "System Control command to set a MIB.");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::ValueFlag<std::string> conf_flag(arguments, "cfg", "The configuration file to read or write.",
                                           {'c', "config"});
    args::ValueFlag<std::string> additional_cfg_flag(
        arguments, "cfg", "The configuration file to load.", {"additional-config"});
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});
    args::Flag is_offline(arguments, "Offline Mode",
                          "Tells the API to use Offline mode when running.",
                          {'o', "offline"});
    args::ValueFlag<std::string> boot_pattern_flag(arguments, "boot_pattern",
                                                   "The boot pattern used for booting.",
                                                   {'b', "boot_pattern"}, "0x7F");
    args::ValueFlag<double> run_time(list_mode, "time",
                                     "The amount of time that a data run will take in seconds.",
                                     {'t', "run-time"}, 10.);
    args::ValueFlag<std::string> parameter(
        arguments, "parameter", "The parameter we want to read from the system.", {'n', "name"});
    args::ValueFlag<unsigned int> channel(
        arguments, "channel",
        "The channel to operate on. If set to the maximum number of channels in the module, then reads/writes execute for all channels.",
        {"chan"});
    args::ValueFlag<unsigned int> crate(arguments, "crate", "The crate", {"crate"}, 0);
    args::ValueFlag<unsigned int> copy_mask(
        copy, "copy_mask", "An integer representing the set of parameters to copy", {"copy-mask"});
    args::ValueFlag<unsigned int> dest_mask(
        copy, "dest_mask", "An integer representing the destination channels", {"dest-mask"});
    args::ValueFlag<unsigned int> dest_channel(copy, "dest_channel",
                                               "The channel that we'll copy to", {"dest-chan"});
    args::ValueFlag<unsigned int> dest_module(copy, "dest_module", "The module that we'll copy to.",
                                              {"dest-mod"});
    args::ValueFlag<std::string> directory(arguments, "directory",
                                           "The directory to write files to", {"output-dir"}, "");
    args::ValueFlag<std::string> firmware_path(arguments, "firmware_path",
                                               "The path to retrieve firmware from", {'f', "firmware"},
                                               ::PixieGetInstallationPath(PIXIE_PATH_FIRMWARE_DEFAULT));
    args::ValueFlag<unsigned int> module(arguments, "module", "The module to operate on.", {"mod"});
    args::ValueFlag<unsigned int> num_runs(
        arguments, "num_runs", "The number of runs to execute when taking list-mode or MCA data.",
        {"num-runs"}, static_cast<unsigned int>(1));
    args::ValueFlag<double> parameter_value(
        write, "parameter_value", "The value of the parameter we want to write.", {'v', "value"});
    args::ValueFlag<unsigned int> synch_wait(
        list_mode, "synch_wait",
        "SynchWait = 0 to start/stop runs independently. (default)\nSynchWait = 1 to start/stop runs synchronously.",
        {"synch-wait"}, static_cast<unsigned int>(0));
    args::ValueFlag<unsigned int> in_synch(
        list_mode, "in_synch",
        "InSynch = 0 to reset clocks prior to starting a run. (default)\nInSynch = 1 to take no clock action.",
        {"in-synch"}, static_cast<unsigned int>(0));
    args::ValueFlag<std::string> par_file(
        arguments, "par", "The parameter file to write.",
        {'p', "par"});
    args::ValueFlag<std::string> query(
        arguments, "query", "The regx query.",
        {'q', "query"});
    args::ValueFlag<std::string> format(
        arguments, "format", "Format of output.",
        {'F', "format"});
    args::ValueFlag<std::string> val_file(
        arguments, "vals", "File name for the command.",
        {"vals"});
    args::ValueFlag<std::string> val_name(
        arguments, "val-name", "The MIB variable name.",
        {"val-name"});
    args::ValueFlag<std::string> val_type(
        arguments, "val-type", "Type of variable.",
        {"val-type"});
    args::ValueFlag<std::string> value(
        arguments, "val", "The value of a varable.",
        {"val"});

    adjust_offsets.Add(conf_flag);
    adjust_offsets.Add(boot_pattern_flag);
    baseline.Add(conf_flag);
    baseline.Add(boot_pattern_flag);
    blcut.Add(conf_flag);
    blcut.Add(boot_pattern_flag);
    blcut.Add(module);
    baseline.Add(directory);
    boot.Add(conf_flag);
    boot.Add(boot_pattern_flag);
    copy.Add(boot_pattern_flag);
    copy.Add(module);
    copy.Add(channel);
    dacs.Add(conf_flag);
    dacs.Add(boot_pattern_flag);
    dacs.Add(module);
    init.Add(conf_flag);
    list_mode.Add(directory);
    list_mode.Add(num_runs);
    mca.Add(directory);
    mca.Add(module);
    mca.Add(boot_pattern_flag);
    mca.Add(synch_wait);
    mca.Add(in_synch);
    mca.Add(num_runs);
    mca.Add(run_time);
    mca_export.Add(conf_flag);
    mca_export.Add(directory);
    mca_export.Add(module);
    read.Add(conf_flag);
    read.Add(crate);
    read.Add(module);
    read.Add(channel);
    read.Add(parameter);
    sysctl.Add(conf_flag);
    sysctl.Add(query);
    sysctl.Add(format);
    sysctl_config.Add(conf_flag);
    sysctl_config.Add(val_file);
    sysctl_config.Add(format);
    sysctl_get.Add(conf_flag);
    sysctl_get.Add(val_name);
    sysctl_get.Add(val_type);
    sysctl_set.Add(conf_flag);
    sysctl_set.Add(val_name);
    sysctl_set.Add(val_type);
    sysctl_set.Add(value);
    tau_finder.Add(conf_flag);
    trace.Add(conf_flag);
    trace.Add(boot_pattern_flag);
    trace.Add(directory);
    write.Add(conf_flag);
    write.Add(parameter);
    write.Add(crate);
    write.Add(module);
    write.Add(channel);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << logging("INFO") << help.what() << std::endl
                  << parser;
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << logging("ERROR") << e.what() << std::endl
                  << parser;
        return EXIT_FAILURE;
    }

    configuration cfg;
    int offline_mode = 0;

    if (!is_offline) {
        if (conf_flag) {
            try {
                read_config(conf_flag.Get(), cfg);
            } catch (std::exception& e) {
                std::cout << logging("ERROR") << e.what() << std::endl;
                return EXIT_FAILURE;
            }
        } else {
            std::cout << logging("ERROR") << "Configuration file required." << std::endl;
            return EXIT_FAILURE;
        }
    } else {
        offline_mode = 1;
        for (int m = 0; m < 4; m++) {
            mod_cfg mcfg;
            mcfg.cfg.number = m;
            mcfg.dsp_par = is_offline.Get();
            mcfg.has_firmware_spec = false;
            mcfg.has_fifo_cfg = false;
            cfg.modules.push_back(mcfg);
        }
        cfg.slot_def = {2, 3, 4, 5};
        boot_pattern_flag.ParseValue({"0x70"});
    }

    std::cout << logging("INFO") << "Finished reading config in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s."
              << std::endl;

    if (directory && !is_directory(directory.Get())) {
        return EXIT_FAILURE;
    }

    std::string dir = directory.Get();
    if (!dir.empty() && dir.back() != '/') {
        dir = dir + "/";
    }

    if (!init_system(cfg, offline_mode) || !set_fifo_configs(cfg)) {
        return EXIT_FAILURE;
    }

    if (sysctl) {
        if (!execute_sysctl(query.Get(), format.Get())) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    if (sysctl_config) {
        if (!execute_sysctl_config(val_file.Get(), format.Get())) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    if (sysctl_get) {
        if (!execute_sysctl_get(val_name.Get(), val_type.Get())) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    if (sysctl_set) {
        if (!execute_sysctl_set(val_name.Get(), val_type.Get(), value.Get())) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    if (init) {
        execute_close_module_connection(cfg.num_modules());
        return EXIT_SUCCESS;
    }

    if (!execute_boot(cfg, boot_pattern_flag, additional_cfg_flag, firmware_path)) {
        return EXIT_FAILURE;
    }

    if (boot) {
        execute_close_module_connection(cfg.num_modules());
        return EXIT_SUCCESS;
    }

    if (additional_cfg_flag) {
        if (!verify_api_return_value(
                ::Pixie16LoadDSPParametersFromFile(additional_cfg_flag.Get().c_str()),
                "Pixie16LoadDSPParametersFromFile", true)) {
            return EXIT_FAILURE;
        }
    }

    if (trace) {
        for (auto& mod : cfg.modules) {
            if (!execute_trace_capture(mod, dir)) {
                return EXIT_FAILURE;
            }
        }
    }

    if (adjust_offsets) {
        for (auto& mod : cfg.modules) {
            if (!execute_adjust_offsets(mod)) {
                return EXIT_FAILURE;
            }
        }
    }

    if (baseline) {
        for (auto& mod : cfg.modules) {
            if (!execute_baseline_capture(mod, dir)) {
                return EXIT_FAILURE;
            }
        }
    }

    if (blcut) {
        for (auto& mod : cfg.modules) {
            if (!execute_blcut(mod)) {
                return EXIT_FAILURE;
            }
        }
    }

    if (dacs) {
        for (auto& mod : cfg.modules) {
            if (!execute_set_dacs(mod)) {
                return EXIT_FAILURE;
            }
        }
    }

    if (tau_finder) {
        for (auto& mod : cfg.modules) {
            if (!execute_find_tau(mod)) {
                return EXIT_FAILURE;
            }
        }
    }

    if (list_mode) {
        if (!execute_list_mode_runs(num_runs.Get(), cfg, run_time.Get(), synch_wait.Get(),
                                    in_synch.Get(), dir)) {
            return EXIT_FAILURE;
        }
    }

    if (mca) {
        if (!execute_mca_runs(num_runs.Get(), cfg, run_time.Get(), synch_wait.Get(), in_synch.Get(),
                              dir)) {
            return EXIT_FAILURE;
        }
    }

    if (mca_export) {
        for (const auto& mod : cfg.modules) {
            std::string name = generate_filename(
                "module" + std::to_string(mod.cfg.number) + "-mca-export", "csv", dir);
            export_mca_memory(mod, name);
        }
    }

    if (read) {
        auto mod = cfg.modules[module.Get()];
        if (channel.Get() >= mod.cfg.number_of_channels) {
            for (unsigned short ch = 0; ch < mod.cfg.number_of_channels; ch++) {
                channel.ParseValue(option_values(1, std::to_string(ch)));
                if (!execute_parameter_read(parameter, crate, module.Get(), channel)) {
                    return EXIT_FAILURE;
                }
            }
        } else {
            if (!execute_parameter_read(parameter, crate, module.Get(), channel)) {
                return EXIT_FAILURE;
            }
        }
    }

    if (write) {
        auto mod = cfg.modules[module.Get()];
        if (channel.Get() >= mod.cfg.number_of_channels) {
            for (unsigned short ch = 0; ch < mod.cfg.number_of_channels; ch++) {
                channel.ParseValue(option_values(1, std::to_string(ch)));
                if (!execute_parameter_write(parameter, parameter_value, crate, mod, channel)) {
                    return EXIT_FAILURE;
                }
            }
        } else {
            if (!execute_parameter_write(parameter, parameter_value, crate, mod, channel)) {
                return EXIT_FAILURE;
            }
        }
    }

    if (copy) {
        if (!module || !copy_mask || !dest_module || (!channel != !dest_channel)) {
            std::cout
                << logging("ERROR")
                << "Pixie16CopyDSPParameters requires the source/destination module and channel "
                   "and the destination mask to execute!"
                << std::endl;
        }
        masks dest_masks;
        for (unsigned short mod = 0; mod < cfg.num_modules(); mod++) {
            if (!dest_channel || !channel) {
                if (mod == dest_module.Get()) {
                    dest_masks.push_back(1);
                } else {
                    dest_masks.push_back(0);
                }
            } else {
                for (unsigned short chan = 0; chan < cfg.modules[mod].cfg.number_of_channels; chan++) {
                    if (mod == dest_module.Get() && chan == dest_channel.Get()) {
                        dest_masks.push_back(1);
                    } else {
                        dest_masks.push_back(0);
                    }
                }
            }
        }
        if (!dest_channel || !channel) {
            if (!verify_api_return_value(::Pixie16CopyDSPParameters(copy_mask.Get(), module.Get(),
                                                                 cfg.modules[module.Get()].cfg.number_of_channels,
                                                                 dest_masks.data()),
                                         "Pixie16CopyDSPParameters", true)) {
                return EXIT_FAILURE;
            }
        } else {
            if (!verify_api_return_value(::Pixie16CopyDSPParameters(copy_mask.Get(), module.Get(),
                                                                channel.Get(), dest_masks.data()),
                                        "Pixie16CopyDSPParameters", true)) {
                return EXIT_FAILURE;
            }
        }
    }

    if (export_settings) {
        if (!save_dsp_pars(cfg.modules.front().dsp_par)) {
            return EXIT_FAILURE;
        }
    }

    execute_close_module_connection(cfg.num_modules());
    return EXIT_SUCCESS;
}
