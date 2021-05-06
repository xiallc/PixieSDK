#---- BUILD LEGACY TESTS
add_executable(legacy_unit_test_runner $<TARGET_OBJECTS:BaseTestRunnerObjLib>
        legacy/test_pixie16app.cpp
        legacy/test_utilities.cpp
        legacy/test_tools.cpp
        $<TARGET_OBJECTS:LegacyPixieSdkObjLib>)
target_include_directories(legacy_unit_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/doctest
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET legacy_unit_test_runner USE_PLX FORCE_DEBUG LINUX_LIBS -Wl,--no-as-needed)

#---- BUILD TEST RUNNER
add_executable(pixie_sdk_unit_test_runner $<TARGET_OBJECTS:BaseTestRunnerObjLib>
        $<TARGET_OBJECTS:PixieSdkObjLib>
        src/test_pixie_buffer.cpp
        src/test_pixie_error.cpp
        src/test_pixie_log.cpp
        src/test_pixie_util.cpp
        )
target_include_directories(pixie_sdk_unit_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/doctest
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET pixie_sdk_unit_test_runner USE_PLX FORCE_DEBUG)
