cmake_minimum_required(VERSION 3.11)
project(pixie_sdk)

link_directories(/usr/local/broadcom/current/PlxApi/Library/)

#------------- BUILD SYS --------------------------
add_library(Pixie16Sys SHARED
        ${PROJECT_SOURCE_DIR}/sys/communication.c
        ${PROJECT_SOURCE_DIR}/sys/i2cm24c64.c
        ${PROJECT_SOURCE_DIR}/sys/pixie16sys.c
        ${PROJECT_SOURCE_DIR}/sys/tools.c)
target_include_directories(Pixie16Sys PUBLIC
        ${PROJECT_SOURCE_DIR}/sys
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/common/include/
        /usr/local/broadcom/current/Include/)
target_link_libraries(Pixie16Sys PUBLIC :PlxApi.so dl)
target_compile_definitions(Pixie16Sys PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(Pixie16Sys PUBLIC -g -Wall)

#------------- BUILD Ideation --------------------------
add_executable(ideation ideation.cpp api/source/pixie_api.cpp)
target_include_directories(ideation PUBLIC
        ${PROJECT_SOURCE_DIR}/api/include
        ${PROJECT_SOURCE_DIR}/sys
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/common/include/
        ${PROJECT_SOURCE_DIR}/hal/include
        /usr/local/broadcom/current/Include/)
target_link_libraries(ideation PUBLIC Pixie16Sys :PlxApi.so dl)
target_compile_definitions(ideation PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(ideation PUBLIC -g -Wall)

#------------- BUILD APP --------------------------
add_library(Pixie16App SHARED
        ${PROJECT_SOURCE_DIR}/api/pixie16app.cpp)
target_include_directories(Pixie16App PUBLIC
        ${PROJECT_SOURCE_DIR}/api
        ${PROJECT_SOURCE_DIR}/sys
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/common/include/
        ${PROJECT_SOURCE_DIR}/hal/include
        /usr/local/broadcom/current/Include/)
target_link_libraries(Pixie16App PUBLIC Pixie16Sys :PlxApi.so)
target_compile_definitions(Pixie16App PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(Pixie16App PUBLIC -g -Wall)

#------------- BUILD pixie.cpp --------------------------
add_executable(pixie ${PROJECT_SOURCE_DIR}/utilities/src/pixie.cpp)
target_include_directories(pixie PUBLIC
        ${PROJECT_SOURCE_DIR}/utilities/include
        ${PROJECT_SOURCE_DIR}/api
        ${PROJECT_SOURCE_DIR}/common/include/
        ${PROJECT_SOURCE_DIR}/sys
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        /usr/local/broadcom/current/Include/)
target_link_libraries(pixie PUBLIC Pixie16App Pixie16Sys :PlxApi.a dl m)
target_compile_definitions(pixie PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(pixie PUBLIC -g -Wall)

#------------- BUILD TESTS --------------------------
add_executable(test tests/test.cpp
        tests/unit/api/test_pixie16app.cpp
        tests/unit/api/test_utilities.cpp
        tests/unit/sys/test_pixie16sys.cpp
        tests/unit/sys/test_tools.cpp
        tests/unit/sys/test_i2cm24c64.cpp
        tests/unit/sys/test_communication.cpp)
target_include_directories(test PUBLIC
        ${PROJECT_SOURCE_DIR}/vendor/doctest
        ${PROJECT_SOURCE_DIR}/utilities/include
        ${PROJECT_SOURCE_DIR}/common/include/
        ${PROJECT_SOURCE_DIR}/api
        ${PROJECT_SOURCE_DIR}/api/include
        ${PROJECT_SOURCE_DIR}/sys
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        /usr/local/broadcom/current/Include/)
target_link_libraries(test PUBLIC Pixie16App Pixie16Sys pixie_sdk :PlxApi.a dl m)
target_compile_definitions(test PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
target_compile_options(test PUBLIC -g -Wall)
