#------------- BUILD APP --------------------------
add_library(Pixie16App SHARED
        ${PROJECT_SOURCE_DIR}/app/pixie16app.c
        ${PROJECT_SOURCE_DIR}/app/utilities.c)
target_include_directories(Pixie16App PUBLIC
        ${PROJECT_SOURCE_DIR}/app
        ${PROJECT_SOURCE_DIR}/sys
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PLX_INCLUDE_DIR})
target_link_libraries(Pixie16App PUBLIC Pixie16Sys ${PLX_SHARED_LIB})
IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(Pixie16App PUBLIC PLX_LIBRARY_DIR)
ENDIF()
target_compile_definitions(Pixie16App PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(Pixie16App PUBLIC -g -Wall)