#------------- BUILD TESTS --------------------------
add_executable(test test.cpp
        unit/app/test_pixie16app.cpp
        unit/app/test_utilities.cpp
        unit/sys/test_pixie16sys.cpp
        unit/sys/test_tools.cpp
        unit/sys/test_i2cm24c64.cpp
        unit/sys/test_communication.cpp)
target_include_directories(test PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/vendor/doctest
        ${PLX_INCLUDE_DIR})
IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(test PUBLIC ${PLX_LIBRARY_DIR})
ENDIF()

if (LINUX)
    target_link_libraries(test PUBLIC PixieSDK ${PLX_SHARED_LIB} dl m)
    target_compile_definitions(test PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
    target_compile_options(test PUBLIC -g -Wall)
else ()
    target_link_libraries(test PUBLIC PixieSDK ${PLX_SHARED_LIB} winmm)
    target_compile_definitions(test PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_WINDOWS PRINT_DEBUG_MSG)
endif ()

