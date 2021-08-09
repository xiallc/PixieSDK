if (BUILD_LEGACY)
    install(DIRECTORY legacy/ DESTINATION include)
endif ()

if (BUILD_SDK)
    install(DIRECTORY pixie/ DESTINATION include)
endif ()

if (BUILD_PIXIE16_SUPPORT)
    install(DIRECTORY pixie16/ DESTINATION include)
endif ()