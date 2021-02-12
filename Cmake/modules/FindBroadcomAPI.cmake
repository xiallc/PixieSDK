# Find the Broadcom PCI/PCIe library
#
# PLX_LIBRARY_DIR - Location of the PlxApi* files.
# PLX_INCLUDE_DIR - Location of the Plx header files.
# PLX_STATIC_LIB - List of the static library.
# PLX_SHARED_LIB - List of the shared library.
# PLX_FOUND - true if at least one of the libraries and the includes was found.
#

find_path(PLX_LIBRARY_DIR
        NAMES PlxApi.a PlxApi.so PlxApi.lib
        HINTS $ENV{PLX_SDK_DIR}
        PATHS /usr/local/broadcom/current /usr/src/PlxSdk C:/PlxApi
        PATH_SUFFIXES PlxApi/Library Linux/PlxApi/Library PlxApi/Release)

find_path(PLX_INCLUDE_DIR
        NAMES Plx.h PlxTypes.h PlxApi.h
        HINTS $ENV{PLX_SDK_DIR}
        PATHS /usr/local/broadcom/current /usr/src/PlxSdk
        PATH_SUFFIXES Include)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BroadcomAPI DEFAULT_MSG PLX_LIBRARY_DIR PLX_INCLUDE_DIR)

if (BroadcomAPI_FOUND)
    set(PLX_SHARED_LIB -l:PlxApi.so)
    set(PLX_STATIC_LIB -l:PlxApi.a)
endif (BroadcomAPI_FOUND)

mark_as_advanced(PLX_SHARED_LIB PLX_STATIC_LIB PLX_LIBRARY_DIR PLX_INCLUDE_DIR)