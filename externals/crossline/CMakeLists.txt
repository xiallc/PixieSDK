add_library(CrosslineObj OBJECT src/crossline.c)
target_compile_definitions(CrosslineObj PUBLIC -D_CRT_SECURE_NO_WARNINGS)
target_include_directories(CrosslineObj PUBLIC ${PROJECT_SOURCE_DIR}/externals/crossline/include)
install(FILES include/crossline.h LICENSE.txt DESTINATION include/crossline)
