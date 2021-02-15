#------------- BUILD TESTS --------------------------
add_executable(test test.cpp
        app/test_pixie16app.cpp
        app/test_utilities.cpp
        sys/test_pixie16sys.cpp
        sys/test_tools.cpp
        sys/test_i2cm24c64.cpp
        sys/test_communication.cpp)
target_include_directories(test PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/vendor/doctest
        ${PLX_INCLUDE_DIR})

IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(test PUBLIC ${PLX_LIBRARY_DIR})
ENDIF()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(test PUBLIC PixieSDK ${PLX_SHARED_LIB} dl m)
    target_compile_definitions(test PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
    target_compile_options(test PUBLIC -g -Wall)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(test PUBLIC PixieSDK ${PLX_SHARED_LIB} winmm)
    target_compile_definitions(test PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_WINDOWS PRINT_DEBUG_MSG)
endif ()