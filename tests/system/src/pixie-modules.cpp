/*----------------------------------------------------------------------
* Copyright (c) 2021, XIA LLC
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

#include <iostream>

#include "args.hxx"

#include <pixie_crate.hpp>

int
main(int argc, char* argv[])
{
    args::ArgumentParser parser("Pixie Modules Report");

    parser.LongSeparator("=");

    args::Group arguments(parser,
                          "arguments",
                          args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::HelpFlag help_flag(arguments,
                             "help", "Displays this message",
                             {'h', "help"});

    args::ValueFlag<std::size_t> num_modules(arguments, "num-modules",
                                           "The number of modules in the crate",
                                             {'n', "num-modules"},
                                             0);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << parser;
        return EXIT_SUCCESS;
    } catch (args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (args::get(num_modules) == 0) {
        std::cerr << "error: no number of modules" << std::endl;
        return EXIT_FAILURE;
    }

    xia::pixie::crate::crate crate(args::get(num_modules));

    std::cout << "Modules found: " << crate.modules.size() << std::endl;

    return EXIT_SUCCESS;
}
