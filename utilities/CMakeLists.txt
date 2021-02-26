#------------- BUILD pixie.cpp --------------------------
add_executable(pixie src/pixie.cpp ${PROJECT_SOURCE_DIR}/vendor/easylogging/easylogging++.cc)
target_include_directories(pixie PUBLIC
        ${PROJECT_SOURCE_DIR}/common/
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/vendor/args
        ${PROJECT_SOURCE_DIR}/vendor/easylogging
        ${PLX_INCLUDE_DIR})

IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(pixie PUBLIC PLX_LIBRARY_DIR)
ENDIF()

target_link_libraries(pixie PUBLIC PixieSDK ${PLX_SHARED_LIB})
target_compile_definitions(pixie PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(PixieSDK PUBLIC PRINT_DEBUG_MSG)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(pixie PUBLIC dl m)
    target_compile_definitions(pixie PUBLIC PLX_LINUX)
    target_compile_options(pixie PUBLIC -g -Wall)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(pixie PUBLIC winmm)
    target_compile_definitions(pixie PUBLIC PLX_WINDOWS)
endif ()
