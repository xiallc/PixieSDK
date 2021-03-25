#------ System test for testing direct communiction to modules.
add_executable(test_direct_communication src/test_direct_communication.cpp
        src/system_test_utilities.cpp
        ${PROJECT_SOURCE_DIR}/vendor/easylogging/easylogging++.cc)

target_include_directories(test_direct_communication PUBLIC include ${PROJECT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/vendor/args ${PROJECT_SOURCE_DIR}/vendor/easylogging)

IF (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(test_direct_communication PUBLIC PLX_LIBRARY_DIR)
ENDIF ()

target_link_libraries(test_direct_communication PUBLIC PixieSDK ${PLX_SHARED_LIB})
target_compile_definitions(test_direct_communication PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PRINT_DEBUG_MSG)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(test_direct_communication PUBLIC dl m)
    target_compile_definitions(test_direct_communication PUBLIC PLX_LINUX)
    target_compile_options(test_direct_communication PUBLIC -g -Wall)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(test_direct_communication PUBLIC winmm)
    target_compile_definitions(test_direct_communication PUBLIC PLX_WINDOWS)
endif ()

#------- System test for module construction
add_executable(pixie-modules src/pixie-modules)
target_include_directories(pixie-modules PUBLIC ${PROJECT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/vendor/args)

IF (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    target_link_directories(pixie-modules PUBLIC PLX_LIBRARY_DIR)
ENDIF ()

target_link_libraries(pixie-modules PUBLIC PixieSDK ${PLX_SHARED_LIB})
target_compile_definitions(pixie-modules PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PRINT_DEBUG_MSG)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(pixie-modules PUBLIC dl m)
    target_compile_definitions(pixie-modules PUBLIC PLX_LINUX)
    target_compile_options(pixie-modules PUBLIC -g -Wall)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(pixie-modules PUBLIC winmm)
    target_compile_definitions(pixie-modules PUBLIC PLX_WINDOWS)
endif ()