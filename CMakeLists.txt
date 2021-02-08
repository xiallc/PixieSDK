cmake_minimum_required(VERSION 3.11)
project(pixie_sdk)

#---- Setup our install path
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/xia/${PROJECT_NAME} CACHE PATH "..." FORCE)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#---- Setup the default build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Build type, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif (NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#---- Add the module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Cmake/modules/")

#---- Check for the Broadcom APIs
find_package(BroadcomAPI REQUIRED)
IF(${CMAKE_VERSION} VERSION_LESS "3.13")
    link_directories(${PLX_LIBRARY_DIR})
ENDIF(${CMAKE_VERSION} VERSION_LESS "3.13")

#---- Build everything
add_subdirectory(sys)
add_subdirectory(app)
add_subdirectory(utilities)
add_subdirectory(tests)