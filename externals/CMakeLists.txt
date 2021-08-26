if (BUILD_EXAMPLES)
    add_subdirectory(easylogging)
endif ()

install(DIRECTORY args DESTINATION include)
install(DIRECTORY nolhmann DESTINATION include)
