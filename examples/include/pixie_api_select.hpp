#ifndef PIXIE_API_SELECT_HPP
#define PIXIE_API_SELECT_HPP
/**----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------**/
/// @file pixie_api_select.hpp
/// @brief Provides a consistent interface between the Legacy and updated SDK.
/// @author C. Johns
/// @date April 3, 2021

#include <map>
#include <memory>
#include <string>

#include <stdint.h>

namespace xia
{
namespace pixie
{
namespace api
{
struct calls {
    typedef std::map<const std::string, const std::string> labels;

    std::string name;

    calls(std::string name);
    virtual ~calls() = default;

    const std::string label(const std::string call) const;

    virtual int acquire_adc_trace(unsigned short module);
    virtual int acquire_baselines(unsigned short module);
    virtual int adjust_offsets(unsigned short module);
    virtual int boot_module(const char* com_fpga_config,
                            const char* sp_fpga_config,
                            const char* trig_fpga_config,
                            const char* dsp_code,
                            const char* dsp_par,
                            const char* dsp_var,
                            unsigned short module,
                            unsigned short boot_pattern);
    virtual int check_run_status(unsigned short module);
    virtual int end_run(unsigned short module);
    virtual int exit_system(unsigned short module);
    virtual int init_system(unsigned short num_modules,
                            unsigned short* pxi_slot_map,
                            unsigned short offline);
    virtual int read_ext_fifo_status(unsigned int* words,
                                     unsigned short module);
    virtual int read_sgl_chan_adc_trace(unsigned short* buffer,
                                        unsigned int length,
                                        unsigned short module,
                                        unsigned short channel);
    virtual int read_sgl_chan_baselines(double* baselines,
                                        double* timestamps,
                                        unsigned short num_bases,
                                        unsigned short module,
                                        unsigned short channel);
    virtual int read_sgl_chan_par(const char* name,
                                  double* data,
                                  unsigned short module,
                                  unsigned short channel);
    virtual int read_sgl_mod_par(const char* name,
                                 unsigned int* data,
                                 unsigned short module);
    virtual int save_dsp_parameters_to_file(const char* file_name);
    virtual int save_external_fifo_data_to_file(const char* file_name,
                                                unsigned int* words,
                                                unsigned short module,
                                                unsigned short end_of_run_read);
    virtual int save_histogram_to_file(const char* file_name,
                                       unsigned short module);
    virtual int start_list_mode_run(unsigned short module,
                                    unsigned short run_type,
                                    unsigned short mode);
    virtual int write_sgl_chan_par(const char* name,
                                   double data,
                                   unsigned short module,
                                   unsigned short channel);
    virtual int write_sgl_mod_par(const char* name,
                                  unsigned int data,
                                  unsigned short module);

    virtual const labels& get_labels() const = 0;
};

/*
 * A calls handle.
 */
typedef std::unique_ptr<calls> calls_handle;

/*
 * Make a call handle for the selected API.
 *
 *      call_handle handle = make_pixi16();
 */
calls_handle make_pixie16_api();
calls_handle make_pixie_api();
}
}
}

#endif // PIXIE_API_SELECT_HPP
