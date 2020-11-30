// @file helper_functions.hpp
// @brief Namespace and functions commonly used by the utilities programs.
// @author S. V. Paulauskas <stan@xia.com>
// @date November 13, 2020

#ifndef PIXIE_SDK_FUNCTIONS_HPP
#define PIXIE_SDK_FUNCTIONS_HPP

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace xia {
    class Configuration {
    public:
        unsigned short numModules;
        unsigned short* slot_map;
        std::string ComFPGAConfigFile;
        std::string SPFPGAConfigFile;
        std::string TrigFPGAConfigFile;
        std::string DSPCodeFile;
        std::string DSPParFile;
        std::string DSPVarFile;
    };

    Configuration read_configuration_file(const std::string& config_file_name) {
        std::ifstream input(config_file_name, std::ios::in);

        if (input.fail()) {
            std::stringstream errmsg;
            errmsg << "Could not open " << config_file_name << "!";
            throw std::invalid_argument(errmsg.str());
        }

        xia::Configuration cfg;
        input >> cfg.numModules;
        cfg.slot_map = new unsigned short[cfg.numModules + 1];
        for (size_t i = 0; i < cfg.numModules; i++)
            input >> cfg.slot_map[i];

        input >> cfg.ComFPGAConfigFile >> cfg.SPFPGAConfigFile >> cfg.TrigFPGAConfigFile >> cfg.DSPCodeFile >>
                cfg.DSPParFile >> cfg.DSPVarFile;

        input.close();
        return cfg;
    }

    //From https://stackoverflow.com/a/868894
    char* getCmdOption(const int& argc, char** argv, const std::string& option) {
        auto end = argv + argc;
        char** itr = std::find(argv, end, option);
        if (itr != end && ++itr != end)
            return *itr;
        return nullptr;
    }

    bool cmdOptionExists(const int& argc, char** argv, const std::string& option) {
        auto end = argv + argc;
        return std::find(argv, end, option) != end;
    }
}  // namespace xia

#endif  //SOFTWARE_HELPER_FUNCTIONS_HPP
