if (BUILD_INTEGRATION_TESTS OR BUILD_UNIT_TESTS)
    add_library(BaseTestRunnerObjLib OBJECT base_test_runner.cpp ../sdk/include/pixie/utils/string.hpp ../sdk/include/pixie/utils/time.hpp ../sdk/include/pixie/utils/io.hpp ../sdk/include/pixie/utils/crc.hpp ../sdk/include/pixie/utils/numerics.hpp ../sdk/include/pixie/utils/path.hpp)
    target_include_directories(BaseTestRunnerObjLib PUBLIC
            ${PROJECT_SOURCE_DIR}/externals/doctest)
    xia_configure_target(TARGET BaseTestRunnerObjLib CONFIG_OBJ)
endif ()

if (BUILD_INTEGRATION_TESTS)
    add_subdirectory(integration)
endif ()

if (BUILD_SDK_UNIT_TESTS)
    add_subdirectory(unit)
endif ()

if (BUILD_SYSTEM_TESTS)
    add_subdirectory(system)
endif ()