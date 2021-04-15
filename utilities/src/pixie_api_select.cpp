/*----------------------------------------------------------------------
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
*----------------------------------------------------------------------*/

#include <stdexcept>

#include <pixie16.h>

#include "pixie16app_export.h"
#include "pixie16sys_export.h"

#include "pixie_api_select.hpp"

namespace xia
{
namespace pixie
{
namespace api
{
[[ noreturn ]] static void
not_implemented(const calls& calls, const char* call)
{
    std::string what("API call not implemented: ");
    what += calls.name + ": " + call;
    throw std::runtime_error(what);
}

calls::calls(std::string name_)
    : name(name_)
{
}

const std::string
calls::label(const std::string call) const
{
    auto& call_labels = get_labels();
    auto search = call_labels.find(call);
    if (search == call_labels.end()) {
        return call + " not implemented";
    }
    return std::get<1>(*search);
}

int
calls::acquire_adc_trace(unsigned short )
{
    not_implemented(*this, "acquire_adc_trace");
}

int
calls::adjust_offsets(unsigned short )
{
    not_implemented(*this, "adjust_offsets");
}

int
calls::boot_module(const char* ,
                   const char* ,
                   const char* ,
                   const char* ,
                   const char* ,
                   const char* ,
                   unsigned short ,
                   unsigned short )
{
    not_implemented(*this, "boot_module");
}

int
calls::check_run_status(unsigned short )
{
    not_implemented(*this, "check_run_status");
}

int
calls::end_run(unsigned short )
{
    not_implemented(*this, "end_run");
}

int
calls::exit_system(unsigned short )
{
    not_implemented(*this, "exit_system");
}

int
calls::init_system(unsigned short ,
                   unsigned short* ,
                   unsigned short )
{
    not_implemented(*this, "init_system");
}

int
calls::read_ext_fifo_status(unsigned int* ,
                            unsigned short )
{
    not_implemented(*this, "read_ext_fifo_status");
}

int
calls::read_sgl_chan_adc_trace(unsigned short* ,
                               unsigned int ,
                               unsigned short ,
                               unsigned short )
{
    not_implemented(*this, "read_sgl_chan_adc_trace");
}

int
calls::read_sgl_chan_par(const char* ,
                         double* ,
                         unsigned short ,
                         unsigned short )
{
    not_implemented(*this, "read_sgl_chan_par");
}

int
calls::read_sgl_mod_par(const char* ,
                        unsigned int* ,
                        unsigned short )
{
    not_implemented(*this, "read_sgl_mod_par");
}

int
calls::save_dsp_parameters_to_file(const char* )
{
    not_implemented(*this, "save_dsp_parameters_to_file");
}

int
calls::save_external_fifo_data_to_file(const char* ,
                                       unsigned int* ,
                                       unsigned short ,
                                       unsigned short )
{
    not_implemented(*this, "save_external_fifo_data_to_file");
}

int
calls::save_histogram_to_file(const char* ,
                              unsigned short )
{
    not_implemented(*this, "save_histogram_to_file");
}

int
calls::start_list_mode_run(unsigned short ,
                           unsigned short ,
                           unsigned short )
{
    not_implemented(*this, "start_list_mode_run");
}

int
calls::write_sgl_chan_par(const char* ,
                          double ,
                          unsigned short ,
                          unsigned short )
{
    not_implemented(*this, "write_sgl_chan_par");
}

int
calls::write_sgl_mod_par(const char* ,
                         unsigned int ,
                         unsigned short )
{
    not_implemented(*this, "write_sgl_mod_par");
}

/*
 * Pixie16 API calls.
 */
struct pixie16_calls
    : public calls
{
    pixie16_calls();

    virtual int acquire_adc_trace(unsigned short module) override;
    virtual int adjust_offsets(unsigned short module) override;
    virtual int boot_module(const char* com_fpga_config,
                            const char* sp_fpga_config,
                            const char* trig_fpga_config,
                            const char* dsp_code,
                            const char* dsp_par,
                            const char* dsp_var,
                            unsigned short module,
                            unsigned short boot_pattern) override;
    virtual int check_run_status(unsigned short module) override;
    virtual int end_run(unsigned short module) override;
    virtual int exit_system(unsigned short module) override;
    virtual int init_system(unsigned short num_modules,
                            unsigned short* pxi_slot_map,
                            unsigned short offline) override;
    virtual int read_ext_fifo_status(unsigned int* words,
                                     unsigned short module) override;
    virtual int read_sgl_chan_adc_trace(unsigned short* buffer,
                                        unsigned int length,
                                        unsigned short module,
                                        unsigned short channel) override;
    virtual int read_sgl_chan_par(const char* name,
                                  double* data,
                                  unsigned short module,
                                  unsigned short channel) override;
    virtual int read_sgl_mod_par(const char* name,
                                 unsigned int* data,
                                 unsigned short module) override;
    virtual int save_dsp_parameters_to_file(const char* filename) override;
    virtual int save_external_fifo_data_to_file(const char* file_name,
                                                unsigned int* words,
                                                unsigned short module,
                                                unsigned short end_of_run_read)
        override;
    virtual int save_histogram_to_file(const char* file_name,
                                       unsigned short module) override;
    virtual int start_list_mode_run(unsigned short module,
                                    unsigned short run_type,
                                    unsigned short mode) override;
    virtual int write_sgl_chan_par(const char* name,
                                   double data,
                                   unsigned short module,
                                   unsigned short channel) override;
    virtual int write_sgl_mod_par(const char* name,
                                  unsigned int data,
                                  unsigned short module) override;

    virtual const labels& get_labels() const override;

private:
    static const labels labels_;
};

pixie16_calls::pixie16_calls()
    : calls("pixie16")
{
}

int
pixie16_calls::acquire_adc_trace(unsigned short module)
{
    return Pixie16AcquireADCTrace(module);
}

int
pixie16_calls::adjust_offsets(unsigned short module)
{
    return Pixie16AdjustOffsets(module);
}

int
pixie16_calls::boot_module(const char* com_fpga_config,
                        const char* sp_fpga_config,
                        const char* trig_fpga_config,
                        const char* dsp_code,
                        const char* dsp_par,
                        const char* dsp_var,
                        unsigned short module,
                        unsigned short boot_pattern)
{
    return Pixie16BootModule(com_fpga_config,
                             sp_fpga_config,
                             trig_fpga_config,
                             dsp_code,
                             dsp_par,
                             dsp_var,
                             module,
                             boot_pattern);
}

int
pixie16_calls::check_run_status(unsigned short module)
{
    return Pixie16CheckRunStatus(module);
}

int
pixie16_calls::end_run(unsigned short module)
{
    return Pixie16EndRun(module);
}

int
pixie16_calls::exit_system(unsigned short module)
{
    return Pixie16ExitSystem(module);
}

int
pixie16_calls::init_system(unsigned short num_modules,
                        unsigned short* pxi_slot_map,
                        unsigned short offline)
{
    return Pixie16InitSystem(num_modules, pxi_slot_map, offline);
}

int
pixie16_calls::read_ext_fifo_status(unsigned int* words,
                                 unsigned short module)
{
    return Pixie_Read_ExtFIFOStatus(words, module);
}

int
pixie16_calls::read_sgl_chan_adc_trace(unsigned short* buffer,
                                    unsigned int length,
                                    unsigned short module,
                                    unsigned short channel)
{
    return Pixie16ReadSglChanADCTrace(buffer, length, module, channel);
}

int
pixie16_calls::read_sgl_chan_par(const char* name,
                                 double* data,
                                 unsigned short module,
                                 unsigned short channel)
{
    return Pixie16ReadSglChanPar(name, data, module, channel);
}

int
pixie16_calls::read_sgl_mod_par(const char* name,
                                unsigned int* data,
                                unsigned short module)
{
    return Pixie16ReadSglModPar(name, data, module);
}

int
pixie16_calls::save_dsp_parameters_to_file(const char* file_name)
{
    return Pixie16SaveDSPParametersToFile(file_name);
}

int
pixie16_calls::save_external_fifo_data_to_file(const char* file_name,
                                            unsigned int* words,
                                            unsigned short module,
                                            unsigned short end_of_run_read)
{
    return Pixie16SaveExternalFIFODataToFile(file_name,
                                             words,
                                             module,
                                             end_of_run_read);
}

int
pixie16_calls::save_histogram_to_file(const char* file_name,
                                   unsigned short module)
{
    return Pixie16SaveHistogramToFile(file_name, module);
}

int
pixie16_calls::start_list_mode_run(unsigned short module,
                                unsigned short run_type,
                                unsigned short mode)
{
    return Pixie16StartListModeRun(module, run_type, mode);
}

int
pixie16_calls::write_sgl_chan_par(const char* name,
                               double data,
                               unsigned short module,
                               unsigned short channel)
{
    return Pixie16WriteSglChanPar(name, data, module, channel);
}

int
pixie16_calls::write_sgl_mod_par(const char* name,
                              unsigned int data,
                              unsigned short module)
{
    return Pixie16WriteSglModPar(name, data, module);
}

const pixie16_calls::labels&
pixie16_calls::get_labels() const
{
    return labels_;
}

const pixie16_calls::labels
pixie16_calls::labels_ =
{
    { "acquire_adc_trace",               "Pixie16AcquireADCTrace" },
    { "adjust_offsets",                  "Pixie16AdjustOffsets" },
    { "boot_module",                     "Pixie16BootModule" },
    { "check_run_status",                "Pixie16CheckRunStatus" },
    { "end_run",                         "Pixie16EndRun" },
    { "exit_system",                     "Pixie16ExitSystem" },
    { "init_system",                     "Pixie16InitSystem" },
    { "read_ext_fifo_status",            "Pixie_Read_ExtFIFOStatus" },
    { "read_sgl_chan_adc_trace",         "Pixie16ReadSglChanADCTrace" },
    { "read_sgl_chan_par",               "Pixie16WriteSglChanPar" },
    { "read_sgl_mod_par",                "Pixie16WriteSglModPar" },
    { "save_dsp_parameters_to_file",     "Pixie16SaveDSPParametersToFile" },
    { "save_external_fifo_data_to_file", "Pixie16SaveExternalFIFODataToFile" },
    { "save_histogram_to_file",          "Pixie16SaveHistogramToFile" },
    { "start_list_mode_run",             "Pixie16StartListModeRun" },
    { "write_sgl_chan_par",              "Pixie16WriteSglChanPar" },
    { "write_sgl_mod_par",               "Pixie16WriteSglModPar" }
};

/*
 * Pixie API calls.
 */
struct pixie_calls
    : public calls
{
    pixie_calls();

    virtual int boot_module(const char* com_fpga_config,
                            const char* sp_fpga_config,
                            const char* trig_fpga_config,
                            const char* dsp_code,
                            const char* dsp_par,
                            const char* dsp_var,
                            unsigned short module,
                            unsigned short boot_pattern) override;
    virtual int exit_system(unsigned short module);
    virtual int init_system(unsigned short num_modules,
                            unsigned short* pxi_slot_map,
                            unsigned short offline) override;

    virtual const labels& get_labels() const override;

private:
    static const labels labels_;
};

pixie_calls::pixie_calls()
    : calls("pixie")
{
}

int
pixie_calls::boot_module(const char* com_fpga_config,
                       const char* sp_fpga_config,
                       const char* trig_fpga_config,
                       const char* dsp_code,
                       const char* dsp_par,
                       const char* dsp_var,
                       unsigned short module,
                       unsigned short boot_pattern)
{
    return PixieBootModule(com_fpga_config,
                           sp_fpga_config,
                           trig_fpga_config,
                           dsp_code,
                           dsp_par,
                           dsp_var,
                           module,
                           boot_pattern);
}

int
pixie_calls::init_system(unsigned short num_modules,
                       unsigned short* pxi_slot_map,
                       unsigned short offline)
{
    return PixieInitSystem(num_modules, pxi_slot_map, offline);
}

int
pixie_calls::exit_system(unsigned short module)
{
    return PixieExitSystem(module);
}

const pixie_calls::labels&
pixie_calls::get_labels() const
{
    return labels_;
}

const pixie_calls::labels
pixie_calls::labels_ =
{
    { "boot_module",                     "PixieBootModule" },
    { "init_system",                     "PixieInitSystem" },
};

calls_handle
make_pixie16_api()
{
    return calls_handle(new pixie16_calls());
}

calls_handle
make_pixie_api()
{
    return calls_handle(new pixie_calls());
}
}
}
}
