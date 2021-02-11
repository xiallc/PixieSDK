list(APPEND SYSTEM_TESTS
        test_DSPIM_repeated_sglWR_SglRd
        test_DSPIM_repeated_sglWR_BurstRd
        test_EM_CLRrepeatedRdtest
        test_EM_repeatedWrRd
        test_EM_WRrepeatedRd
        test_external_fifo)
foreach (PROG IN LISTS SYSTEM_TESTS)
    add_executable(${PROG} ${PROG}.cpp)
    target_include_directories(${PROG} PUBLIC ${PROJECT_SOURCE_DIR}/common ${PROJECT_SOURCE_DIR}/vendor/args)
    target_link_libraries(${PROG} PUBLIC Pixie16App Pixie16Sys ${PLX_STATIC_LIB} dl m)
    IF (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
        target_link_directories(${PROG} PUBLIC PLX_LIBRARY_DIR)
    ENDIF ()
    target_compile_definitions(${PROG} PUBLIC PLX_LITTLE_ENDIAN PCI_CODE PLX_LINUX PRINT_DEBUG_MSG)
    target_compile_options(${PROG} PUBLIC -g -Wall)
endforeach()