install(DIRECTORY args DESTINATION include)
install(DIRECTORY nolhmann DESTINATION include)
install(DIRECTORY cs DESTINATION include)
install(DIRECTORY semver DESTINATION include)
install(DIRECTORY cppcodec DESTINATION include)

# Crossline gets added as a subdirectory since we actually need to compile it.
add_subdirectory(crossline)
