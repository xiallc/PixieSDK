#------------- BUILD pixie_sdk_example --------------------------
add_executable(pixie_sdk_example
        src/pixie_sdk_example.cpp
        src/pixie_api_select.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_error.cpp
        ${PROJECT_SOURCE_DIR}/sdk/src/pixie_log.cpp
        $<TARGET_OBJECTS:EasyLoggingObjLib>)
target_include_directories(pixie_sdk_example PUBLIC
        include/
        ${PROJECT_SOURCE_DIR}/sdk/include/
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/args
        ${PROJECT_SOURCE_DIR}/externals/easylogging)
xia_configure_target(TARGET pixie_sdk_example USE_PLX LIBS PixieSDK LINUX_LIBS -Wl,--no-as-needed)

#------------- INSTALL pixie_sdk_example --------------------------
file(GLOB PIXIESDK_EXAMPLE_PUBLIC_HEADERS include/*.hpp
        ${PROJECT_SOURCE_DIR}/externals/easylogging/easylogging++.h
        ${PROJECT_SOURCE_DIR}/externals/args/args.hxx)
set_target_properties(pixie_sdk_example PROPERTIES PUBLIC_HEADER "${PIXIESDK_EXAMPLE_PUBLIC_HEADERS}")
install(TARGETS pixie_sdk_example DESTINATION bin PUBLIC_HEADER DESTINATION include)

install(DIRECTORY share DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES src/pixie_sdk_example.py DESTINATION bin)