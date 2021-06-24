add_executable(example_pixie16app
        example_pixie16app.cpp
        $<TARGET_OBJECTS:EasyLoggingObjLib>)
target_include_directories(example_pixie16app PUBLIC
        ${PROJECT_SOURCE_DIR}/api/legacy/include/
        ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET example_pixie16app USE_PLX LIBS Pixie16App LINUX_LIBS -Wl,--no-as-needed)

install(TARGETS example_pixie16app RUNTIME DESTINATION bin)
