#------------- BUILD pixie.cpp --------------------------
add_executable(pixie ${PROJECT_SOURCE_DIR}/utilities/src/pixie.cpp)
target_include_directories(pixie PUBLIC
        ${PROJECT_SOURCE_DIR}/utilities/include
        ${PROJECT_SOURCE_DIR}/api
        ${PROJECT_SOURCE_DIR}/sys
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PLX_INCLUDE_DIR})
target_link_libraries(pixie PUBLIC PixieSDK ${PLX_STATIC_LIB} dl m)
IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(pixie PUBLIC PLX_LIBRARY_DIR)
ENDIF()
target_compile_definitions(pixie PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(pixie PUBLIC -g -Wall)