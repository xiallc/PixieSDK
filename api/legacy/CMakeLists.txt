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
target_include_directories(LegacyObjLib PUBLIC include/ ${PROJECT_SOURCE_DIR}/externals)
xia_configure_target(TARGET LegacyObjLib USE_PLX CONFIG_OBJ)

#---- Build the Legacy shared object for install and general use.
add_library(Pixie16App SHARED $<TARGET_OBJECTS:LegacyObjLib>)
target_include_directories(Pixie16App PUBLIC include/)
xia_configure_target(TARGET Pixie16App USE_PLX)

#---- Install LegacyPixieSDK
install(TARGETS Pixie16App LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)