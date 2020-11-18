"""
    @file wscript
    @brief WAF build script to replace the old Make implementation
    @author S. V. Paulauskas
    @date October 23, 2020
"""
from sys import platform
from os import environ
from os.path import isdir

from waflib.Configure import conf

TOP = "."
OUT = "build"


def options(opt):
    """
    Option parsing to handle including/removing some preprocessor commands.
    :param opt: The options dictionary
    """
    opt.load("compiler_c compiler_cxx")
    opt.add_option('--utilities', action='store_true', default=False, help="Builds the utility programs.")
    opt.add_option('--usleep', action='store_true', default=False, help="Adds USLEEP parameter to sys compilation.")


@conf
def find_broadcom_sdk(ctx):
    """
    Looks for the Broadcom PCI/PCIe SDK by checking for the PLX_SDK_DIR environment variable.
    :param ctx: Configuration that we'll update with the locations we find.
    :return:
    """
    ctx.start_msg('Checking for Broadcom PCI/PCIe SDK')

    sdk_dir = environ.get("PLX_SDK_DIR", None)
    if not sdk_dir:
        ctx.end_msg("Couldn't locate the PLX_SDK_DIR in the environment")
        ctx.fatal("We need the PLX_SDK_DIR to build our drivers.")

    include_dir = f"{sdk_dir}/Include"
    if not isdir(include_dir):
        ctx.end_msg(f"Couldn't locate {include_dir}")
        ctx.fatal("We need these headers to build our drivers.")

    ctx.env.INCLUDES_PLX = include_dir

    lib_dir = f"{sdk_dir}/PlxApi/Library"
    lib_name = "PlxApi.a"
    ctx.find_file(lib_name, lib_dir)
    ctx.env.LIB_PLX = f':{lib_name}'
    ctx.env.LIBPATH_PLX = lib_dir

    ctx.end_msg(sdk_dir)


def configure(ctx):
    """
    Configuration function to load the C and C++ compilers and set some of our flags
    :param ctx: The configuration dictionary.
    """
    ctx.env.PREFIX = f"{ctx.env.PREFIX}/xia/pixie-sdk"
    ctx.load("compiler_c compiler_cxx")
    ctx.env.CFLAGS = ["-g", "-Wall", "-DPLX_LITTLE_ENDIAN", "-DPCI_CODE"]

    ctx.find_broadcom_sdk()

    if platform == "linux":
        ctx.env.append_value("CFLAGS", "-DPLX_LINUX")

    ctx.env.INCLUDES_APP = ["app", "sys"]

    ctx.env.INCLUDES_SYS = ["sys", "vendor/analog_devices"]

    if ctx.options.usleep:
        ctx.env.CFLAGS_SYS = ctx.env.CFLAGS + ["-DUSE_USLEEP"]

    ctx.env.CPPFLAGS = ctx.env.CFLAGS

    ctx.env.INCLUDES_UTILITIES = ['utilities/include']


def build(bld):
    """
    The main build function. This function builds libPixie16app.a and libPixie16sys.a.
    :param bld: The build dictionary
    """
    for name in ['app', 'sys']:
        use_list = [name.upper()]
        if name == 'app':
            use_list = use_list + ['Pixie16Sys', 'SYS', 'PLX']
        if name == 'sys':
            use_list.append("PLX")

        bld.shlib(source=bld.path.find_dir(name).ant_glob("*.c"), target=f'Pixie16{name.title()}',
                  install_path="${PREFIX}/lib", use=use_list)

    for header_path in set(bld.env.INCLUDES_APP + bld.env.INCLUDES_SYS):
        path = bld.path.find_dir(header_path)
        bld.install_files('${PREFIX}/include', path.ant_glob('*.h'), cwd=path, relative_trick=True)

    if bld.options.utilities:
        for source in ['pixie.cpp']:
            bld.program(source=f"utilities/src/{source}", target=source.split(".")[0],
                        use='Pixie16App Pixie16Sys PLX APP SYS UTILITIES', lib=['m', 'dl'],
                        install_path="${PREFIX}/bin")
        bld.install_files('${PREFIX}/share', bld.path.find_dir("utilities/share").ant_glob('**/*'),
                          cwd=bld.path.find_dir("utilities/share/"), relative_trick=True)

        bld.install_files('${PREFIX}/bin', bld.path.find_dir("utilities/src").ant_glob('*.py'))
