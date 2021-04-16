#------------- BUILD pixie.cpp --------------------------
add_executable(pixie
        src/pixie.cpp
        src/pixie_api_select.cpp
        $<TARGET_OBJECTS:EasyLoggingObjLib>)
target_include_directories(pixie PUBLIC
        ${PROJECT_SOURCE_DIR}/common/
        ${PROJECT_SOURCE_DIR}/vendor/analog_devices
        ${PROJECT_SOURCE_DIR}/vendor/args
        ${PROJECT_SOURCE_DIR}/vendor/easylogging)
xia_configure_target(TARGET pixie USE_PLX LIBS PixieSDK LINUX_LIBS -Wl,--no-as-needed)
