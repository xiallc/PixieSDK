/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2022 XIA LLC, All rights reserved.
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

/** @file list_mode_file_stats.cpp
 * @brief Ingests a list-mode data file and validates its contents. Part of P16-502.
 */

#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include <args/args.hxx>

#include <pixie/format.hpp>

#include "pixie/data/list_mode.hpp"

using json = xia::pixie::format::json;

struct LOG {
    explicit LOG(const std::string& type) {
        type_ = type;

        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::chrono::milliseconds now2 =
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        char timeBuffer[80];
        std::strftime(timeBuffer, 80, "%FT%T", gmtime(&currentTime));

        std::stringstream tmp;
        tmp << timeBuffer << "." << std::setfill('0') << std::setw(3) << now2.count() % 1000 << "Z";

        datetime_ = tmp.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const LOG& log) {
        os << log.datetime_ << " - " << log.type_ << " - ";
        return os;
    }

    std::string type_;
    std::string datetime_;
};

struct config {
    ///TODO: Add a set of expected data that can be configured on a per-channel basis. E.g. esums.
    uint32_t frequency;
    uint32_t revision;
    uint32_t slot;
    uint32_t bits{};
    std::vector<std::string> files;
    config() : frequency(0), revision(0), slot(0), bits(0) {}
};

using configs = std::vector<config>;

struct channel_info {
    size_t id;
    size_t count;
    double cps;
    double energy_ave;
    double energy_min;
    double energy_max;
    double event_length_ave;
    double header_length_ave;
    double time_min;
    double time_max;
    double trace_length_ave;
};

void to_json(json& j, const channel_info& ch) {
    j = json{
        {"id", ch.id},
        {"count", ch.count},
        {"count_per_second", ch.cps},
        {"energy", {{"max", ch.energy_max}, {"min", ch.energy_min}, {"ave", ch.energy_ave}}},
        {"time", {{"max", ch.time_max}, {"min", ch.time_min}}},
        {"event_length", ch.event_length_ave},
        {"trace_length", ch.trace_length_ave},
        {"header_length", ch.header_length_ave},
    };
}

using channel_id = size_t;
using channel_stats = std::map<channel_id, channel_info>;

void verify_json_slot(const json& node) {
    if (!node.contains("slot")) {
        throw std::invalid_argument("Missing slot definition in configuration element.");
    }

    if (!node.contains("revision")) {
        throw std::invalid_argument("Missing revision object in configuration element.");
    }

    if (!node.contains("frequency")) {
        throw std::invalid_argument("Missing frequency object in configuration element.");
    }

    if (!node.contains("files")) {
        throw std::invalid_argument("Missing files list in configuration element.");
    }
}

void read_config(const std::string& file, configs& cfgs) {
    std::ifstream input(file, std::ios::in);
    if (input.fail()) {
        throw std::ios_base::failure("open: " + file + ": " + std::strerror(errno));
    }

    json jf = json::parse(input);
    input.close();

    for (const auto& element : jf) {
        verify_json_slot(element);

        config cfg;
        cfg.slot = element["slot"];
        cfg.revision = element["revision"];
        cfg.frequency = element["frequency"];
        cfg.files = element["files"].get<std::vector<std::string>>();
        cfgs.push_back(cfg);
    }
}

double calculate_duration_in_seconds(const std::chrono::system_clock::time_point& start,
                                     const std::chrono::system_clock::time_point& end) {
    return std::chrono::duration<double>(end - start).count();
}

int main(int argc, char** argv) {
    auto first_start = std::chrono::system_clock::now();
    auto start = first_start;
    args::ArgumentParser parser("Validates list-mode data files produced by the example software.");
    parser.LongSeparator("=");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::ValueFlag<std::string> conf_flag(
        arguments, "cfg", "The configuration file telling us what to expect in the list-mode data.",
        {'c', "config"}, args::Options::Required);
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});
    args::ValueFlag<std::string> output_flag(
        arguments, "output_file",
        "The name of the JSONL file that will contain all the extracted records.", {'o', "output"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << LOG("INFO") << help.what() << std::endl;
        std::cout << parser;
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << LOG("ERROR") << e.what() << std::endl;
        std::cout << parser;
        return EXIT_FAILURE;
    }

    configs cfgs;
    try {
        read_config(conf_flag.Get(), cfgs);
    } catch (std::exception& e) {
        std::cout << LOG("ERROR") << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << LOG("INFO") << "Finished reading config in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s."
              << std::endl;

    std::ofstream output;
    if (output_flag) {
        output.open(output_flag.Get());
    }

    static const auto bytes_per_word = 4;
    /*
     * This is the approximate maximum size of a Rev H record (header + trace). Results
     * in an empirically optimized processing time.
     */
    static const auto chunk_size_words = 16398;

    for (const auto& cfg : cfgs) {
        for (const auto& file : cfg.files) {
            start = std::chrono::system_clock::now();
            std::cout << LOG("INFO") << "Starting to parse " << file << std::endl;

            std::ifstream input(file, std::ios::in | std::ios::binary | std::ios::ate);
            xia::pixie::data::list_mode::buffer remainder;
            auto record_total = 0;
            channel_stats stats;
            try {
                if (input.fail()) {
                    throw std::ios_base::failure("open: " + file + ": " + std::strerror(errno));
                }

                const auto size_bytes = input.tellg();
                const auto size_words = size_bytes / bytes_per_word;
                auto remaining_words = size_words;
                auto num_chunks = size_words / chunk_size_words;
                auto data_vec_size = chunk_size_words;
                auto max_retries = 10;
                auto retries = max_retries;

                std::cout << LOG("INFO") << "File Size In Bytes: " << size_bytes
                          << " | File Size In Words: " << size_words << std::endl;

                input.seekg(0);
                std::cout << LOG("INFO") << "Starting to decode data." << std::endl;

                for (int chunk_num = 0; chunk_num <= num_chunks;
                     chunk_num++, remaining_words -= chunk_size_words) {
                    if (retries == 0) {
                        std::cout << LOG("ERROR") << "Failed to resync data stream." << std::endl;
                        break;
                    }

                    if (remaining_words < chunk_size_words) {
                        data_vec_size = int(remaining_words);
                    }

                    xia::pixie::data::list_mode::buffer data(data_vec_size, 0);
                    input.read(reinterpret_cast<char*>(&data[0]),
                               chunk_size_words * bytes_per_word);

                    std::cout << LOG("INFO") << "Read words: " << chunk_size_words * chunk_num
                              << " Remaining words: " << remaining_words << std::endl;

                    if (!remainder.empty()) {
                        data.insert(data.begin(), remainder.begin(), remainder.end());
                        remainder.clear();
                        remainder.shrink_to_fit();
                    }
                    try {
                        xia::pixie::data::list_mode::records records;
                        xia::pixie::data::list_mode::decode_data_block(
                            data, cfgs[0].revision, cfgs[0].frequency, records, remainder);
                        record_total += int(records.size());
                        for (const auto& record : records) {
                            if (output.is_open()) {
                                std::string out;
                                xia::pixie::data::list_mode::record_to_json(record, out);
                                output << out << std::endl;
                            }
                            auto stat_rec = stats.find(record.channel_number);
                            if (stat_rec == stats.end()) {
                                stats[record.channel_number] = {record.channel_number,
                                                                1,
                                                                0,
                                                                record.energy,
                                                                record.energy,
                                                                record.energy,
                                                                double(record.event_length),
                                                                double(record.header_length),
                                                                record.time.count(),
                                                                record.time.count(),
                                                                double(record.trace_length)};
                            } else {
                                auto ch = &stat_rec->second;
                                ch->count++;
                                ch->energy_ave += record.energy;
                                ch->trace_length_ave += double(record.trace_length);
                                ch->header_length_ave += record.header_length;
                                ch->event_length_ave += record.event_length;

                                if (record.energy > ch->energy_max) {
                                    ch->energy_max = record.energy;
                                }
                                if (record.energy < ch->energy_min) {
                                    ch->energy_min = record.energy;
                                }
                                if (record.time.count() > ch->time_max) {
                                    ch->time_max = record.time.count();
                                }
                                if (record.time.count() < ch->time_min) {
                                    ch->time_min = record.time.count();
                                }
                            }
                        }
                        if (retries != max_retries) {
                            std::cout << LOG("INFO") << "Successfully synchronized data stream."
                                      << std::endl;
                            retries = max_retries;
                        }
                    } catch (xia::pixie::data::list_mode::error& error) {
                        std::cout << LOG("WARN") << "Decoding failed on chunk " << chunk_num - 1
                                  << " starting at byte "
                                  << (chunk_num - 1) * chunk_size_words * bytes_per_word
                                  << " with the following error." << std::endl;
                        std::cout << LOG("ERROR") << error.what() << std::endl;
                        if (chunk_num == num_chunks) {
                            std::cout << LOG("INFO") << "Possible data loss in last chunk."
                                      << std::endl;
                        } else {
                            std::cout << LOG("INFO") << "Attempting to resync data stream."
                                      << std::endl;
                        }
                        retries--;
                    }
                }
                std::cout << LOG("INFO") << "Finished decoding data in "
                          << calculate_duration_in_seconds(start, std::chrono::system_clock::now())
                          << " s." << std::endl;
                input.close();
            } catch (std::bad_alloc& bad_alloc) {
                std::cout << LOG("ERROR") << bad_alloc.what() << std::endl;
            } catch (std::ios_base::failure& failure) {
                std::cout << LOG("ERROR") << failure.what() << std::endl;
            } catch (xia::pixie::error::error& sdkerr) {
                std::cout << LOG("ERROR") << sdkerr.what() << std::endl;
            }

            std::cout << LOG("INFO") << "Total records processed : " << record_total << std::endl;

            for (auto& stat : stats) {
                auto ch = &stat.second;
                ch->energy_ave /= double(ch->count);
                ch->cps = double(ch->count) / (ch->time_max - ch->time_min);
                ch->header_length_ave /= double(ch->count);
                ch->trace_length_ave /= double(ch->count);
                ch->event_length_ave /= double(ch->count);
                json j = *ch;
                std::cout << LOG("INFO") << j.dump() << std::endl;
            }

            if (!remainder.empty()) {
                std::cout << LOG("WARN") << "Leftover Words: " << remainder.size() << std::endl;
            }

            std::cout << LOG("INFO") << "Finished parsing " << file << std::endl;
        }
    }

    if (output.is_open()) {
        output.close();
    }

    std::cout << LOG("INFO") << "Finished execution in "
              << calculate_duration_in_seconds(first_start, std::chrono::system_clock::now())
              << " s." << std::endl;
    return EXIT_SUCCESS;
}
