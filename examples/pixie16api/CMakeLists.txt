if (BUILD_PIXIE16_API)
    add_executable(example_pixie16_sdk
            example_pixie16api.cpp
            $<TARGET_OBJECTS:EasyLoggingObjLib>)
    target_include_directories(example_pixie16_sdk PUBLIC
            ${PROJECT_SOURCE_DIR}/sdk/include/
            ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET example_pixie16_sdk USE_PLX LIBS Pixie16Api LINUX_LIBS -Wl,--no-as-needed)
    install(TARGETS example_pixie16_sdk RUNTIME DESTINATION bin)
endif ()

if (BUILD_LEGACY)
    add_executable(example_pixie16_legacy
            example_pixie16api.cpp
            $<TARGET_OBJECTS:EasyLoggingObjLib>)
    target_include_directories(example_pixie16_legacy PUBLIC
            ${PROJECT_SOURCE_DIR}/api/legacy/include/
            ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET example_pixie16_legacy USE_PLX LIBS Pixie16App
            LINUX_LIBS -Wl,--no-as-needed
            COMPILE_DEFS LEGACY_EXAMPLE)
    install(TARGETS example_pixie16_legacy RUNTIME DESTINATION bin)
endif ()

if (BUILD_SDK OR BUILD_LEGACY)
    install(FILES example_pixie16api.py DESTINATION bin)
endif ()