#---- Legacy System test for testing direct communication to modules.
add_executable(test_direct_communication
        src/test_direct_communication.cpp
        src/system_test_utilities.cpp
	$<TARGET_OBJECTS:PixieSdkObjLib>
        $<TARGET_OBJECTS:LegacyPixieSdkObjLib>
        $<TARGET_OBJECTS:EasyLoggingObjLib>)
target_include_directories(test_direct_communication PUBLIC
        include
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/analog_devices
        ${PROJECT_SOURCE_DIR}/externals/args
        ${PROJECT_SOURCE_DIR}/externals/easylogging
        ${PROJECT_SOURCE_DIR}/externals/nolhmann)
xia_configure_target(TARGET test_direct_communication USE_PLX)

#---- System test for module construction
add_executable(test_api src/test_api
        src/simulation.cpp
        $<TARGET_OBJECTS:PixieSdkObjLib>
        $<TARGET_OBJECTS:LegacyPixieSdkObjLib>)
target_include_directories(test_api PUBLIC
        include
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/args
        ${PROJECT_SOURCE_DIR}/externals/nolhmann)
xia_configure_target(TARGET test_api USE_PLX)
