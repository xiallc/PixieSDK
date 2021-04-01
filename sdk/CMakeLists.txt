add_library(PixieSDK SHARED
        src/communication.c
        src/i2cm24c64.c
        src/pcf8574.c
        src/pixie16app.c
        src/pixie16sys.c
        src/tools.c
        src/utilities.c
        # C++
        src/pixie.cpp
        src/pixie_crate.cpp
        src/pixie_module.cpp
        src/pixie_fw.cpp
        src/pixie_param.cpp
        src/pixie_hw.cpp
        src/pixie_error.cpp
        src/hw/dsp.cpp
        src/hw/fpga_comms.cpp
        src/hw/fpga_fippi.cpp
        src/hw/fpga.cpp
        src/hw/i2c_bitbash.cpp
        src/hw/i2cm24c64.cpp
        src/hw/pcf8574.cpp)
target_include_directories(PixieSDK PUBLIC
        include/
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PLX_INCLUDE_DIR})

IF (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(PixieSDK PUBLIC ${PLX_LIBRARY_DIR})
ENDIF ()

target_link_libraries(PixieSDK PUBLIC ${PLX_SHARED_LIB})
target_compile_definitions(PixieSDK PUBLIC PLX_LITTLE_ENDIAN PCI_CODE)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(PixieSDK PUBLIC PRINT_DEBUG_MSG)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(PixieSDK PUBLIC dl m)
    target_compile_definitions(PixieSDK PUBLIC PLX_LINUX)
    target_compile_options(PixieSDK PUBLIC -g -Wall -Wextra)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(PixieSDK PUBLIC winmm)
    target_compile_definitions(PixieSDK PUBLIC PLX_WINDOWS )
endif ()

file(GLOB PIXIESDK_PUBLIC_HEADERS include/*.h
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices/def21160.h)
set_target_properties(PixieSDK PROPERTIES PUBLIC_HEADER "${PIXIESDK_PUBLIC_HEADERS}")
install(TARGETS PixieSDK LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include)
