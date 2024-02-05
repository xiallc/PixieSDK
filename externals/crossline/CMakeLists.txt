add_library(CrosslineObj OBJECT src/crossline.c)
target_include_directories(CrosslineObj PUBLIC ${PROJECT_SOURCE_DIR}/externals/crossline/include)
install(FILES include/crossline.h LICENSE.txt DESTINATION include/crossline)