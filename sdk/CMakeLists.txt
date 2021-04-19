set(PIXIE_SDK_LEGACY_SOURCES
        src/communication.c
        src/i2cm24c64.c
        src/pcf8574.c
        src/pixie16app.c
        src/pixie16sys.c
        src/tools.c
        src/utilities.c
        )

#---- Build the Legacy object library so that we don't compile the sources more than once
add_library(LegacyPixieSdkObjLib OBJECT ${PIXIE_SDK_LEGACY_SOURCES})
set_property(TARGET LegacyPixieSdkObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(LegacyPixieSdkObjLib PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices)
xia_configure_target(TARGET LegacyPixieSdkObjLib USE_PLX CONFIG_OBJ)

#---- Build the Legacy shared object for install and general use.
add_library(LegacyPixieSDK SHARED $<TARGET_OBJECTS:LegacyPixieSdkObjLib>)
target_include_directories(LegacyPixieSDK PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices)
xia_configure_target(TARGET LegacyPixieSDK USE_PLX)

#---- Install LegacyPixieSDK
file(GLOB LEGACYPIXIESDK_PUBLIC_HEADERS include/*.h
        ${PROJECT_SOURCE_DIR}/externals/analog_devices/def21160.h)
set_target_properties(LegacyPixieSDK PROPERTIES PUBLIC_HEADER "${LEGACYPIXIESDK_PUBLIC_HEADERS}")
install(TARGETS LegacyPixieSDK LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include)

set(PIXIE_SDK_SOURCES
        src/configuration.cpp
        src/pixie16.cpp
        src/pixie_crate.cpp
        src/pixie_module.cpp
        src/pixie_fw.cpp
        src/pixie_param.cpp
        src/pixie_hw.cpp
        src/pixie_util.cpp
        src/pixie_log.cpp
        src/pixie_error.cpp
        src/hw/csr.cpp
        src/hw/dsp.cpp
        src/hw/fpga_comms.cpp
        src/hw/fpga_fippi.cpp
        src/hw/fpga.cpp
        src/hw/i2c_bitbash.cpp
        src/hw/i2cm24c64.cpp
        src/hw/memory.cpp
        src/hw/pcf8574.cpp
        src/hw/run.cpp
        )

#---- Build the object library so that we don't compile the sources more than once
add_library(PixieSdkObjLib OBJECT ${PIXIE_SDK_SOURCES})
set_property(TARGET PixieSdkObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(PixieSdkObjLib PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices)
xia_configure_target(TARGET PixieSdkObjLib USE_PLX CONFIG_OBJ)

#---- Build the shared object for install and general use.
add_library(PixieSDK SHARED $<TARGET_OBJECTS:PixieSdkObjLib> $<TARGET_OBJECTS:LegacyPixieSdkObjLib>)
target_include_directories(PixieSDK PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices)
xia_configure_target(TARGET PixieSDK USE_PLX)

#---- Install PixieSDK
file(GLOB PIXIESDK_PUBLIC_HEADERS include/*.hpp include/hw/*.hpp)
set_target_properties(PixieSDK PROPERTIES PUBLIC_HEADER "${PIXIESDK_PUBLIC_HEADERS}")
install(TARGETS PixieSDK LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include)
