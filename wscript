"""
    @file wscript
    @brief WAF build script to replace the old Make implementation
    @author S. V. Paulauskas
    @date October 23, 2020
"""
from sys import platform

top = "."
out = "build"


def options(opt):
    """
    Option parsing to handle including/removing some preprocessor commands.
    :param opt: The options dictionary
    """
    opt.load("compiler_c compiler_cxx")
    opt.add_option('--test', action='store_true', default=False, help="Instructs the program to build tests.")
    opt.add_option('--usleep', action='store_true', default=False, help="Adds USLEEP parameter to sys compilation.")


def configure(conf):
    """
    Configuration function to load the C and C++ compilers and set some of our flags
    :param conf: The configuration dictionary.
    """
    conf.load("compiler_c compiler_cxx")
    compiler_flags = ["-g", "-Wall", "-DPLX_LITTLE_ENDIAN", "-DPCI_CODE"]
    include_directories = ["sys", "sys/inc"]

    if platform == "linux":
        compiler_flags.append("-DPLX_LINUX")

    conf.env.INCLUDES_APP = include_directories + ["app"]
    conf.env.CFLAGS_APP = compiler_flags

    conf.env.INCLUDES_SYS = include_directories
    conf.env.CFLAGS_SYS = compiler_flags

    if conf.options.usleep:
        conf.env.CFLAGS_SYS = compiler_flags + ["-DUSE_USLEEP"]


def build(bld):
    """
    The main build function. This function builds libPixie16app.a and libPixie16sys.a.
    :param bld: The build dictionary
    """

    app_prefix = "app"
    bld.stlib(source=[f"{app_prefix}/pixie16app.c",
                      f"{app_prefix}/utilities.c"],
              target='Pixie16App',
              use="APP")

    sys_prefix = "sys"
    bld.stlib(source=[f"{sys_prefix}/pixie16sys.c",
                      f"{sys_prefix}/i2cm24c64.c",
                      f"{sys_prefix}/tools.c",
                      f"{sys_prefix}/communication.c"],
              target='Pixie16Sys',
              use="SYS")
