#------------- BUILD pixie.cpp --------------------------
add_executable(pixie_sdk_example
        src/pixie_sdk_example.cpp
        src/pixie_api_select.cpp
        $<TARGET_OBJECTS:EasyLoggingObjLib>)
target_include_directories(pixie_sdk_example PUBLIC
        include/
        ${PROJECT_SOURCE_DIR}/sdk/include/
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/vendor/args
        ${PROJECT_SOURCE_DIR}/vendor/easylogging)
xia_configure_target(TARGET pixie_sdk_example USE_PLX LIBS PixieSDK LINUX_LIBS -Wl,--no-as-needed)
