# Find the Pixie SDK
#
# XIA_LIBRARY_DIR
# XIA_INCLUDE_DIR
# XIA_LIBRARIES

find_path(XIA_LIBRARY_DIR
        NAMES libPixieSDK.so
        HINTS $ENV{XIA_PIXIE_SDK}
        PATHS /usr/local/xia/pixie_sdk
        PATH_SUFFIXES lib
        DOC "Path to pixie library.")

find_path(XIA_INCLUDE_DIR
        NAMES pixie16app_defs.h pixie16sys_defs.h
        HINTS $ENV{XIA_PIXIE_SDK}
        PATHS /usr/local/xia/pixie_sdk
        PATH_SUFFIXES include
        DOC "Path to XIA includes.")

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PIXIE_SDK DEFAULT_MSG XIA_LIBRARY_DIR)

if (PIXIE_SDK_FOUND)
    set(XIA_LIBRARIES -lPixieSDK)
endif ()