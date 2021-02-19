list(APPEND SYSTEM_TESTS test_direct_communication test_memory_reads_and_writes)

foreach (PROG IN LISTS SYSTEM_TESTS)
    add_executable(${PROG} src/${PROG}.cpp)
    target_include_directories(${PROG} PUBLIC ${PROJECT_SOURCE_DIR}/common ${PROJECT_SOURCE_DIR}/vendor/args)

    IF (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
        target_link_directories(${PROG} PUBLIC PLX_LIBRARY_DIR)
    ENDIF ()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        target_link_libraries(${PROG} PUBLIC PixieSDK ${PLX_SHARED_LIB} dl m)
        target_compile_definitions(${PROG} PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
        target_compile_options(${PROG} PUBLIC -g -Wall)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        target_link_libraries(${PROG} PUBLIC PixieSDK ${PLX_SHARED_LIB} winmm)
        target_compile_definitions(${PROG} PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_WINDOWS PRINT_DEBUG_MSG)
    endif ()
endforeach()

