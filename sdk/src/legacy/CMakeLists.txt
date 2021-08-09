add_library(LegacyObjLib OBJECT
        communication.c
        i2cm24c64.c
        pcf8574.c
        pixie16app.c
        pixie16sys.c
        tools.c
        utilities.c
        )
set_property(TARGET LegacyObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(LegacyObjLib PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/legacy/)
xia_configure_target(TARGET LegacyObjLib USE_PLX CONFIG_OBJ)

add_library(Pixie16App STATIC $<TARGET_OBJECTS:LegacyObjLib>)
target_include_directories(Pixie16App PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/legacy/)
xia_configure_target(TARGET Pixie16App USE_PLX)

install(TARGETS Pixie16App ARCHIVE DESTINATION lib)
