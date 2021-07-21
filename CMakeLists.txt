cmake_minimum_required(VERSION 3.10)
project(PixieSDK)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "${CMAKE_SYSTEM_NAME} is a supported environment.")
else ()
    message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is an unsupported environment!")
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/xia/${PROJECT_NAME} CACHE PATH "..." FORCE)
    endif ()
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
message(STATUS "Install Path: ${CMAKE_INSTALL_PREFIX}")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Build type, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif (NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if (${CMAKE_BUILD_TYPE} MATCHES Release)
    string(REGEX REPLACE "-O3" "-O2" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REGEX REPLACE "-O3" "-O2" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endif ()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Cmake/modules/;${PROJECT_SOURCE_DIR}/Cmake/functions")
include(xia_configure_target)

find_package(BroadcomAPI REQUIRED)
IF (${CMAKE_VERSION} VERSION_LESS "3.13")
    link_directories(${PLX_LIBRARY_DIR})
ENDIF (${CMAKE_VERSION} VERSION_LESS "3.13")

add_subdirectory(sdk)
add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(externals)
add_subdirectory(bin)

option(PIXIESDK_DOCO "Tells CMake to build documentation" OFF)
if (PIXIESDK_DOCO)
    add_subdirectory(doc)
endif ()