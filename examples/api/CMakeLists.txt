if (BUILD_PIXIE16_API)
    add_library(ExampleUtilsObj OBJECT common/example_utils.cpp)
    target_include_directories(ExampleUtilsObj PUBLIC
            common/
            ${PROJECT_SOURCE_DIR}/sdk/include/
            ${PROJECT_SOURCE_DIR}/externals/
    )
    xia_configure_target(TARGET ExampleUtilsObj CONFIG_OBJ)

    set(EXAMPLE_SOURCES
            01_system_setup/init.cpp
            01_system_setup/boot_crate.cpp
            02_configuration/adjust_offsets.cpp
            02_configuration/rw_parameter.cpp
            03_data_acquisition/acq_adc_trace.cpp
            03_data_acquisition/acq_baselines.cpp
            03_data_acquisition/acq_mca.cpp
            03_data_acquisition/acq_list_mode.cpp
            04_advanced_features/simulate_crate.cpp
            04_advanced_features/config_list_mode_fifo.cpp
            04_advanced_features/sysctl_queries.cpp
    )

    foreach (EXAMPLE_SRC IN LISTS EXAMPLE_SOURCES)
        get_filename_component(TARGET_NAME ${EXAMPLE_SRC} NAME_WE)
        set(FULL_TARGET_NAME "ex_${TARGET_NAME}")
        add_executable(${FULL_TARGET_NAME} ${EXAMPLE_SRC} $<TARGET_OBJECTS:ExampleUtilsObj>)
        target_include_directories(${FULL_TARGET_NAME} PUBLIC
                common/
                ${PROJECT_SOURCE_DIR}/sdk/include/
                ${PROJECT_SOURCE_DIR}/externals/
        )
        xia_configure_target(TARGET ${FULL_TARGET_NAME} USE_PLX LIBS Pixie16Api)
        install(TARGETS ${FULL_TARGET_NAME} RUNTIME DESTINATION bin)
    endforeach ()
endif ()