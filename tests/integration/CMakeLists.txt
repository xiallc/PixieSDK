add_executable(pixie_sdk_integration_test_runner $<TARGET_OBJECTS:BaseTestRunnerObjLib>
        src/test_parameter_read_write.cpp
        $<TARGET_OBJECTS:PixieSdkObjLib>
        $<TARGET_OBJECTS:PixieSdkCommonObjLib>
        )
target_include_directories(pixie_sdk_integration_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET pixie_sdk_integration_test_runner USE_PLX FORCE_DEBUG)

add_executable(test_pixie16_c src/test_pixie16.c)
set_property(TARGET test_pixie16_c PROPERTY C_STANDARD 99)
target_include_directories(test_pixie16_c PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET test_pixie16_c USE_PLX FORCE_DEBUG LIBS Pixie16Api)

add_custom_command(TARGET test_pixie16_c POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tests/integration/data
        $<TARGET_FILE_DIR:test_pixie16_c>/data)

