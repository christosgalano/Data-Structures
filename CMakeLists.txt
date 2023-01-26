cmake_minimum_required (VERSION 3.16)

project(
  "DataStructures" 
  VERSION 1.0
  DESCRIPTION "A header-only c++ library containing implementations of various data structures."
  HOMEPAGE_URL "https://github.com/christosgalano/Data-Structures"
  LANGUAGES CXX
)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INSTALL_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/bin/${CMAKE_BUILD_TYPE}")
set(INSTALL_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/lib/${CMAKE_BUILD_TYPE}")
set(INSTALL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/include")

# Set language standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download GTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
  
# Add source files
add_subdirectory(src)
  
# Enable testing - optional
option(ENABLE_TESTING "Build unit tests" OFF)
if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
