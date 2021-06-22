add_subdirectory(legacy)

#---- Build SDK
set(PIXIE_SDK_SOURCES
        src/pixie_crate.cpp
        src/pixie_module.cpp
        src/pixie_channel.cpp
        src/pixie_config.cpp
        src/pixie_stats.cpp
        src/pixie_fw.cpp
        src/pixie_param.cpp
        src/pixie_eeprom.cpp
        src/pixie_hw.cpp
        src/pixie_buffer.cpp
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
        ${PROJECT_SOURCE_DIR}/externals/nolhmann)
xia_configure_target(TARGET PixieSdkObjLib USE_PLX CONFIG_OBJ)

#---- Build the static object for install and general use.
add_library(PixieSDK STATIC $<TARGET_OBJECTS:PixieSdkObjLib>)
target_include_directories(PixieSDK PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices)
xia_configure_target(TARGET PixieSDK USE_PLX)

#---- Prep the headers for install
file(GLOB PIXIESDK_PUBLIC_HEADERS include/*.hpp include/hw/*.hpp)
set_target_properties(PixieSDK PROPERTIES PUBLIC_HEADER "${PIXIESDK_PUBLIC_HEADERS}")

#---- Install PixieSDK
install(TARGETS PixieSDK LIBRARY PUBLIC_HEADER)

#--- Build Pixie16 API
add_library(Pixie16Api SHARED src/pixie16.cpp $<TARGET_OBJECTS:PixieSdkObjLib>)
target_include_directories(Pixie16Api PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/nolhmann)
xia_configure_target(TARGET Pixie16Api USE_PLX)
install(TARGETS Pixie16Api LIBRARY PUBLIC_HEADER)