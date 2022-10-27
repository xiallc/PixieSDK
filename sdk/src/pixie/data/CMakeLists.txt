add_library(PixieDataObjLib OBJECT list_mode.cpp)
set_property(TARGET PixieDataObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(PixieDataObjLib PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/ ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET PixieDataObjLib CONFIG_OBJ)

add_library(PixieData SHARED $<TARGET_OBJECTS:PixieDataObjLib>)
target_link_libraries(PixieData PUBLIC PixieSdkCommonObjLib)
target_include_directories(PixieData PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/
        ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET PixieData)
install(TARGETS PixieData LIBRARY DESTINATION lib)