add_library(Pixie16DataObjLib OBJECT list_mode.cpp)
set_property(TARGET Pixie16DataObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(Pixie16DataObjLib PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/ ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET Pixie16DataObjLib CONFIG_OBJ)

add_library(Pixie16Data SHARED $<TARGET_OBJECTS:Pixie16DataObjLib> $<TARGET_OBJECTS:PixieSdkObjLib>)
target_include_directories(Pixie16Data PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/
        ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET Pixie16Data USE_PLX)
install(TARGETS Pixie16Data LIBRARY DESTINATION lib)