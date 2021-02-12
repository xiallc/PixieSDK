#------------- BUILD SYS --------------------------
add_library(Pixie16Sys SHARED
        ${PROJECT_SOURCE_DIR}/sys/communication.c
        ${PROJECT_SOURCE_DIR}/sys/i2cm24c64.c
        ${PROJECT_SOURCE_DIR}/sys/pixie16sys.c
        ${PROJECT_SOURCE_DIR}/sys/tools.c
        ${PROJECT_SOURCE_DIR}/sys/pcf8574.c)
target_include_directories(Pixie16Sys PUBLIC
        ${PROJECT_SOURCE_DIR}/sys
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PLX_INCLUDE_DIR})
target_link_libraries(Pixie16Sys PUBLIC ${PLX_SHARED_LIB} dl)
IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(Pixie16Sys PUBLIC {PLX_LIBRARY_DIR})
ENDIF()
target_compile_definitions(Pixie16Sys PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(Pixie16Sys PUBLIC -g -Wall)

file(GLOB PIXIE16SYS_PUBLIC_HEADERS *.h)
set_target_properties(Pixie16Sys PROPERTIES PUBLIC_HEADER "${PIXIE16SYS_PUBLIC_HEADERS}")
install(TARGETS Pixie16Sys LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include)