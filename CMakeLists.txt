cmake_minimum_required (VERSION 3.5)
project(DataStructures)

option(BUILD_TEST "Build the unit tests" OFF)

# we want select our own prefix to prevent naming issues
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INSTALL_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/bin/${CMAKE_BUILD_TYPE}")
set(INSTALL_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/lib/${CMAKE_BUILD_TYPE}")
set(INSTALL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(externals)
add_subdirectory(src)
if(BUILD_TEST)
  add_subdirectory(test)
endif()
