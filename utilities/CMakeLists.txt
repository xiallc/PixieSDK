#------------- BUILD pixie.cpp --------------------------
add_executable(pixie src/pixie.cpp)
target_include_directories(pixie PUBLIC
        ${PROJECT_SOURCE_DIR}/common/
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/vendor/args
        ${PLX_INCLUDE_DIR})

IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(pixie PUBLIC PLX_LIBRARY_DIR)
ENDIF()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(pixie PUBLIC PixieSDK ${PLX_SHARED_LIB} dl m)
    target_compile_definitions(pixie PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
    target_compile_options(pixie PUBLIC -g -Wall)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(pixie PUBLIC PixieSDK ${PLX_SHARED_LIB} winmm)
    target_compile_definitions(pixie PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_WINDOWS PRINT_DEBUG_MSG)
endif ()
