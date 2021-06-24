add_subdirectory(easylogging)

install(DIRECTORY args DESTINATION include)
install(DIRECTORY easylogging DESTINATION include PATTERN "*.cc" EXCLUDE PATTERN "*.txt" EXCLUDE)
install(DIRECTORY nolhmann DESTINATION include)
