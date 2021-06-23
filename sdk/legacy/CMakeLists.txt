set(LEGACY_SOURCES
        src/communication.c
        src/i2cm24c64.c
        src/pcf8574.c
        src/pixie16app.c
        src/pixie16sys.c
        src/tools.c
        src/utilities.c
        )

add_library(LegacyObjLib OBJECT ${LEGACY_SOURCES})
set_property(TARGET LegacyObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(LegacyObjLib PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices)
xia_configure_target(TARGET LegacyObjLib USE_PLX CONFIG_OBJ)

#---- Build the Legacy shared object for install and general use.
add_library(Pixie16App SHARED $<TARGET_OBJECTS:LegacyObjLib>)
target_include_directories(Pixie16App PUBLIC include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices)
xia_configure_target(TARGET Pixie16App USE_PLX)

#---- Install LegacyPixieSDK
file(GLOB LEGACYPIXIESDK_PUBLIC_HEADERS include/*.h
        ${PROJECT_SOURCE_DIR}/externals/analog_devices/def21160.h)
set_target_properties(Pixie16App PROPERTIES PUBLIC_HEADER "${LEGACYPIXIESDK_PUBLIC_HEADERS}")
install(TARGETS Pixie16App LIBRARY PUBLIC_HEADER)