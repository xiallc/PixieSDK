configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/test_parameter_read_write.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/test_parameter_read_write.cpp @ONLY)

add_executable(pixie_sdk_integration_test_runner $<TARGET_OBJECTS:BaseTestRunnerObjLib>
        $<TARGET_OBJECTS:PixieSdkObjLib>
        ../system/src/simulation.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/test_parameter_read_write.cpp
        )
target_include_directories(pixie_sdk_integration_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/tests/system/include
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/doctest
        ${PROJECT_SOURCE_DIR}/externals/nolhmann
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET pixie_sdk_integration_test_runner USE_PLX FORCE_DEBUG)
