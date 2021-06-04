#---- Legacy System test for testing direct communication to modules.
add_executable(test_direct_communication
        src/test_direct_communication.cpp
        src/system_test_utilities.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_crate.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_module.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_channel.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_config.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_stats.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_fw.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_param.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_hw.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_buffer.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_util.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_log.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_error.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/csr.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/dsp.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/fpga_comms.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/fpga_fippi.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/fpga.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/i2c_bitbash.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/i2cm24c64.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/memory.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/pcf8574.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/hw/run.cpp
        $<TARGET_OBJECTS:LegacyPixieSdkObjLib>
        $<TARGET_OBJECTS:EasyLoggingObjLib>)
target_include_directories(test_direct_communication PUBLIC
        include
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/args
        ${PROJECT_SOURCE_DIR}/externals/easylogging
        ${PROJECT_SOURCE_DIR}/externals/nolhmann)
xia_configure_target(TARGET test_direct_communication USE_PLX)

#---- System test for module construction
add_executable(test_api src/test_api
        src/simulation.cpp
        $<TARGET_OBJECTS:PixieSdkObjLib>
        $<TARGET_OBJECTS:LegacyPixieSdkObjLib>)
target_include_directories(test_api PUBLIC
        include
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/args
        ${PROJECT_SOURCE_DIR}/externals/nolhmann)
xia_configure_target(TARGET test_api USE_PLX)
