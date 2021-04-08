#------------- BUILD TESTS --------------------------
add_executable(legacy_unit_test_runner legacy_unit_test_runner.cpp
        legacy/test_pixie16app.cpp
        legacy/test_utilities.cpp
        legacy/test_tools.cpp
        )
target_include_directories(legacy_unit_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/vendor/doctest
        ${PLX_INCLUDE_DIR})

IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(legacy_unit_test_runner PUBLIC ${PLX_LIBRARY_DIR})
ENDIF()

target_link_libraries(legacy_unit_test_runner PUBLIC ${PLX_SHARED_LIB})
target_compile_definitions(legacy_unit_test_runner PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PRINT_DEBUG_MSG)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(legacy_unit_test_runner PUBLIC PixieSDK ${PLX_SHARED_LIB} dl m)
    target_compile_definitions(legacy_unit_test_runner PUBLIC PLX_LINUX)
    target_compile_options(legacy_unit_test_runner PUBLIC -g -Wall -Wextra)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(legacy_unit_test_runner PUBLIC PixieSDK ${PLX_SHARED_LIB} winmm)
    target_compile_definitions(legacy_unit_test_runner PUBLIC PLX_WINDOWS)
endif ()
