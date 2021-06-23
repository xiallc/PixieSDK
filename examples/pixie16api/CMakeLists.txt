add_executable(example_pixie16api
        example_pixie16api.cpp
        $<TARGET_OBJECTS:EasyLoggingObjLib>)
target_include_directories(example_pixie16api PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include/
        ${PROJECT_SOURCE_DIR}/sdk/legacy/include/
        ${PROJECT_SOURCE_DIR}/api/pixie16/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/args
        ${PROJECT_SOURCE_DIR}/externals/easylogging)
xia_configure_target(TARGET example_pixie16api USE_PLX LIBS Pixie16Api LINUX_LIBS -Wl,--no-as-needed)

file(GLOB PIXIESDK_EXAMPLE_PUBLIC_HEADERS include/*.hpp
        ${PROJECT_SOURCE_DIR}/externals/easylogging/easylogging++.h
        ${PROJECT_SOURCE_DIR}/externals/args/args.hxx)
set_target_properties(example_pixie16api PROPERTIES PUBLIC_HEADER "${PIXIESDK_EXAMPLE_PUBLIC_HEADERS}")

install(TARGETS example_pixie16api DESTINATION bin PUBLIC_HEADER DESTINATION include)
install(FILES example_pixie16api.py DESTINATION bin)