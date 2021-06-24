add_executable(legacy_unit_test_runner $<TARGET_OBJECTS:BaseTestRunnerObjLib>
        test_pixie16app.cpp
        test_utilities.cpp
        test_tools.cpp
        $<TARGET_OBJECTS:LegacyObjLib>)
target_include_directories(legacy_unit_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/legacy/include
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/doctest
        ${PROJECT_SOURCE_DIR}/externals/nolhmann
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET legacy_unit_test_runner USE_PLX FORCE_DEBUG LINUX_LIBS -Wl,--no-as-needed)