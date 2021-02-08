cmake_minimum_required(VERSION 3.11)
project(pixie_sdk)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Cmake/modules/")

find_package(BroadcomAPI REQUIRED)

IF(${CMAKE_VERSION} VERSION_LESS "3.13")
    link_directories(${PLX_LIBRARY_DIR})
ENDIF(${CMAKE_VERSION} VERSION_LESS "3.13")

add_subdirectory(sys)
add_subdirectory(app)
add_subdirectory(utilities)
add_subdirectory(tests)