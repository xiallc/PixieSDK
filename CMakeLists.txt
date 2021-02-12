cmake_minimum_required(VERSION 3.11)
project(pixie_sdk)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "${CMAKE_SYSTEM_NAME} is a supported environment.")
else()
    message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is an unsupported environment!")
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/xia/${PROJECT_NAME} CACHE PATH "..." FORCE)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
message(STATUS "Install Path: ${CMAKE_INSTALL_PREFIX}")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Build type, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif (NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Cmake/modules/")

find_package(BroadcomAPI REQUIRED)
IF(${CMAKE_VERSION} VERSION_LESS "3.13")
    link_directories(${PLX_LIBRARY_DIR})
ENDIF(${CMAKE_VERSION} VERSION_LESS "3.13")

add_subdirectory(sdk)
add_subdirectory(tests)
add_subdirectory(utilities)
