if (BUILD_SDK)
    add_executable(pixie16_omnitool
        $<TARGET_OBJECTS:PixieSdkObjLib>
        $<TARGET_OBJECTS:PixieSdkCommonObjLib>
        src/pixie16_omnitool.cpp
        src/src/crate/boot.cpp
        src/src/crate/commands.cpp
        src/src/crate/crate.cpp
        src/src/crate/export.cpp
        src/src/crate/fw-report.cpp
        src/src/crate/import.cpp
        src/src/crate/report.cpp
        src/src/module/adc-acq.cpp
        src/src/module/adc-save.cpp
        src/src/module/adj-off.cpp
        src/src/module/bl-acq.cpp
        src/src/module/bl-save.cpp
        src/src/module/copy.cpp
        src/src/module/db.cpp
        src/src/module/hist-resume.cpp
        src/src/module/hist-save.cpp
        src/src/module/hist-start.cpp
        src/src/module/list-mode-save.cpp
        src/src/module/list-resume.cpp
        src/src/module/list-start.cpp
        src/src/module/lset-import.cpp
        src/src/module/lset-load.cpp
        src/src/module/lset-report.cpp
        src/src/module/mod-offline.cpp
        src/src/module/mod-online.cpp
        src/src/module/commands.cpp
        src/src/module/mod-online.cpp
        src/src/module/mset-import.cpp
        src/src/module/override-fw.cpp
        src/src/module/par-read.cpp
        src/src/module/par-write.cpp
        src/src/module/reg-read-write.cpp
        src/src/module/run-active.cpp
        src/src/module/run-end.cpp
        src/src/module/set-dacs.cpp
        src/src/module/stats.cpp
        src/src/module/stats-rpt.cpp
        src/src/module/test.cpp
        src/src/module/var-read.cpp
        src/src/module/var-write.cpp
        src/src/qa/qa.cpp
        src/src/qa/offset-sweep.cpp
        src/src/commands.cpp
        src/src/threads.cpp
        )
    target_include_directories(pixie16_omnitool PUBLIC src/include ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET pixie16_omnitool USE_PLX)

    if (BUILD_PIXIE16_API)
        add_executable(p16_246 src/p16_246.cpp)
        target_include_directories(p16_246 PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
        xia_configure_target(TARGET p16_246 USE_PLX LIBS PixieSDK)
    endif ()
endif ()
