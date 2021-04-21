#---- Build the base test runner object once
add_library(BaseTestRunnerObjLib OBJECT base_test_runner.cpp)
target_include_directories(BaseTestRunnerObjLib PUBLIC
        ${PROJECT_SOURCE_DIR}/externals/doctest)
xia_configure_target(TARGET BaseTestRunnerObjLib CONFIG_OBJ)

add_subdirectory(unit)
add_subdirectory(system)