#---- BUILD LEGACY TESTS
add_executable(legacy_test_runner base_test_runner.cpp
        app/test_pixie16app.cpp
        app/test_utilities.cpp
        sys/test_pixie16sys.cpp
        sys/test_tools.cpp
        sys/test_i2cm24c64.cpp
        sys/test_communication.cpp
        $<TARGET_OBJECTS:LegacyPixieSdkObjLib>)
target_include_directories(legacy_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/doctest
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET legacy_test_runner USE_PLX FORCE_DEBUG LINUX_LIBS -Wl,--no-as-needed)

#---- BUILD TEST RUNNER
add_executable(pixie_sdk_test_runner base_test_runner.cpp $<TARGET_OBJECTS:PixieSdkObjLib>
        src/test_pixie_error.cpp
        src/test_pixie_util.cpp)
target_include_directories(pixie_sdk_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/doctest
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET pixie_sdk_test_runner USE_PLX FORCE_DEBUG)