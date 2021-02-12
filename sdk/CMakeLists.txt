add_library(PixieSDK SHARED
        src/communication.c
        src/i2cm24c64.c
        src/pcf8574.c
        src/pixie16app.c
        src/pixie16sys.c
        src/tools.c
        src/utilities.c)
target_include_directories(PixieSDK PUBLIC
        include/
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PLX_INCLUDE_DIR})
target_link_libraries(PixieSDK PUBLIC ${PLX_SHARED_LIB})
IF (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(PixieSDK PUBLIC PLX_LIBRARY_DIR)
ENDIF ()
target_compile_definitions(PixieSDK PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(PixieSDK PUBLIC -g -Wall)

file(GLOB PIXIESDK_PUBLIC_HEADERS *.h)
set_target_properties(PixieSDK PROPERTIES PUBLIC_HEADER "${PIXIESDK_PUBLIC_HEADERS}")
install(TARGETS PixieSDK LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include)