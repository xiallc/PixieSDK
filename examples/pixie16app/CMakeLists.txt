add_executable(example_pixie16app
        example_pixie16app.cpp
        $<TARGET_OBJECTS:EasyLoggingObjLib>)
target_include_directories(example_pixie16app PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/legacy/include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/args
        ${PROJECT_SOURCE_DIR}/externals/easylogging)
xia_configure_target(TARGET example_pixie16app USE_PLX LIBS Pixie16App LINUX_LIBS -Wl,--no-as-needed)

file(GLOB PIXIE16APP_EXAMPLE_PUBLIC_HEADERS
        ${PROJECT_SOURCE_DIR}/externals/easylogging/easylogging++.h
        ${PROJECT_SOURCE_DIR}/externals/args/args.hxx)
set_target_properties(example_pixie16app PROPERTIES PUBLIC_HEADER "${PIXIE16APP_EXAMPLE_PUBLIC_HEADERS}")

install(TARGETS example_pixie16app DESTINATION bin PUBLIC_HEADER DESTINATION include)
