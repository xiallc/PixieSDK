add_subdirectory(core)
add_subdirectory(crate)
add_subdirectory(module)
add_subdirectory(qa)

add_executable(pixie16_omnitool
        pixie16_omnitool.cpp
        $<TARGET_OBJECTS:PixieSdkObjLib>
        $<TARGET_OBJECTS:PixieSdkCommonObjLib>
        ${OMNITOOL_CRATE_SOURCES}
        ${OMNITOOL_MODULE_SOURCES}
        ${OMNITOOL_QA_SOURCES}
        ${OMNITOOL_CORE_SOURCES}
)

target_include_directories(pixie16_omnitool PUBLIC
        ${PROJECT_SOURCE_DIR}/tools/omnitool/include/
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/)

xia_configure_target(TARGET pixie16_omnitool USE_PLX)

install(TARGETS pixie16_omnitool RUNTIME DESTINATION bin)

