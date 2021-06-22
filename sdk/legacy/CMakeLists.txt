set(PIXIE_SDK_LEGACY_SOURCES
        src/communication.c
        src/i2cm24c64.c
        src/pcf8574.c
        src/pixie16app.c
        src/pixie16sys.c
        src/tools.c
        src/utilities.c
        )

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
install(TARGETS LegacyPixieSDK LIBRARY PUBLIC_HEADER)