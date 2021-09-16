if (BUILD_LEGACY)
    add_executable(test_direct_communication
            src/test_direct_communication.cpp
            src/system_test_utilities.cpp
            $<TARGET_OBJECTS:LegacyObjLib>)
    target_include_directories(test_direct_communication PUBLIC
            include
            ${PROJECT_SOURCE_DIR}/sdk/include/legacy
            ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET test_direct_communication USE_PLX LEGACY)
endif ()

if (BUILD_SDK)
    add_executable(test_api src/test_api.cpp $<TARGET_OBJECTS:PixieSdkObjLib>)
    target_include_directories(test_api PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET test_api USE_PLX)

    if (BUILD_PIXIE16_API)
        add_executable(p16_246 src/p16_246.cpp)
        target_include_directories(p16_246 PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
        xia_configure_target(TARGET p16_246 USE_PLX LIBS PixieSDK)

        add_executable(p16_371 src/p16_371.cpp)
        target_include_directories(p16_371 PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
        xia_configure_target(TARGET p16_371 USE_PLX LIBS Pixie16Api)
    endif ()
endif ()

