add_subdirectory(data)
add_subdirectory(pixie16)
add_subdirectory(reports)

# These source files have some specificity to the Pixie-16 hardware. They'll eventually be
# agnostic, but aren't yet. Ex. `eeprom.cpp` has a dependence on the "module" class.
list(APPEND SDK_PIXIE16_SOURCES config.cpp eeprom.cpp fw.cpp param.cpp stats.cpp)

# These sources are fully generalized and could be used in a separate XIA library.
set(SDK_COMMON_SOURCES
        buffer.cpp
        error.cpp
        log.cpp
        mib.cpp
        utils/crc.cpp
        utils/io.cpp
        utils/numerics.cpp
        utils/path.cpp
        utils/string.cpp
        utils/thread.cpp
        utils/time.cpp)

add_library(PixieSdkCommonObjLib OBJECT ${SDK_COMMON_SOURCES})
set_property(TARGET PixieSdkCommonObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(PixieSdkCommonObjLib PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/ ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET PixieSdkCommonObjLib CONFIG_OBJ)

add_library(PixieSdkObjLib OBJECT ${SDK_PIXIE16_SOURCES} ${SDK_REPORT_SOURCES})
set_property(TARGET PixieSdkObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(PixieSdkObjLib PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/ ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET PixieSdkObjLib USE_PLX CONFIG_OBJ)

add_library(PixieSDK STATIC $<TARGET_OBJECTS:PixieSdkObjLib> $<TARGET_OBJECTS:PixieSdkCommonObjLib>)
target_include_directories(PixieSDK PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/ ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET PixieSDK USE_PLX)

install(TARGETS PixieSDK ARCHIVE DESTINATION lib)
