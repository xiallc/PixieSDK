set(SDK_HW_SOURCES
        src/hw/csr.cpp
        src/hw/dsp.cpp
        src/hw/fpga.cpp
        src/hw/fpga_comms.cpp
        src/hw/fpga_fippi.cpp
        src/hw/i2c_bitbash.cpp
        src/hw/i2cm24c64.cpp
        src/hw/memory.cpp
        src/hw/pcf8574.cpp
        src/hw/run.cpp
        )

set(SDK_FRAMEWORK_SOURCES
        src/pixie_buffer.cpp
        src/pixie_channel.cpp
        src/pixie_config.cpp
        src/pixie_crate.cpp
        src/pixie_eeprom.cpp
        src/pixie_error.cpp
        src/pixie_fw.cpp
        src/pixie_hw.cpp
        src/pixie_log.cpp
        src/pixie_module.cpp
        src/pixie_param.cpp
        src/pixie_sim.cpp
        src/pixie_stats.cpp
        src/pixie_util.cpp
        )

add_library(PixieSdkObjLib OBJECT ${SDK_HW_SOURCES} ${SDK_FRAMEWORK_SOURCES})
set_property(TARGET PixieSdkObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(PixieSdkObjLib PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/nolhmann)
xia_configure_target(TARGET PixieSdkObjLib USE_PLX CONFIG_OBJ)

add_library(PixieSDK STATIC $<TARGET_OBJECTS:PixieSdkObjLib>)
target_include_directories(PixieSDK PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices)
xia_configure_target(TARGET PixieSDK USE_PLX)

install(TARGETS PixieSDK LIBRARY)
install(DIRECTORY include/ TYPE INCLUDE)

