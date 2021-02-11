add_executable(test test.cpp
        app/test_pixie16app.cpp
        app/test_utilities.cpp
        sys/test_pixie16sys.cpp
        sys/test_tools.cpp
        sys/test_i2cm24c64.cpp
        sys/test_communication.cpp)
target_include_directories(test PUBLIC
        ${PROJECT_SOURCE_DIR}/api
        ${PROJECT_SOURCE_DIR}/sys
        ${PROJECT_SOURCE_DIR}/vendor/doctest
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PLX_INCLUDE_DIR})
target_link_libraries(test PUBLIC Pixie16App Pixie16Sys ${PLX_STATIC_LIB} dl m)
IF (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(test PUBLIC PLX_LIBRARY_DIR)
ENDIF ()
target_compile_definitions(test PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(test PUBLIC -g -Wall)

