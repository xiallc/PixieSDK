if (BUILD_SDK)
    if (BUILD_PIXIE16_API)
        add_executable(p16_246 src/p16_246.cpp)
        target_include_directories(p16_246 PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
        xia_configure_target(TARGET p16_246 USE_PLX LIBS PixieSDK)

        add_executable(p16-1142 src/p16-1142.cpp
                $<TARGET_OBJECTS:Pixie16ApiObjLib>
                $<TARGET_OBJECTS:PixieDataObjLib>)
        target_include_directories(p16-1142 PUBLIC
                ${PROJECT_SOURCE_DIR}/sdk/include
                ${PROJECT_SOURCE_DIR}/externals/
                ${PLX_INCLUDE_DIR})
        xia_configure_target(TARGET p16-1142 USE_PLX LIBS PixieSDK)
    endif ()
endif ()
