cmake_minimum_required (VERSION 3.16)

project(
  "DataStructures" 
  VERSION 1.0
  DESCRIPTION "A header-only C++ library containing implementations of various data structures."
  HOMEPAGE_URL "https://github.com/christosgalano/Data-Structures"
  LANGUAGES CXX
)

SET(CMAKE_INSTALL_PREFIX ".")

# Set directories
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
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
  
# Create static library
add_library(${PROJECT_NAME} INTERFACE)

# Include header files
target_include_directories(${PROJECT_NAME} INTERFACE ${INCLUDE_DIR})

# Install include directory
# install(DIRECTORY ${INCLUDE_DIR}/ DESTINATION ${INSTALL_INCLUDE_DIR})
  
# Enable testing - optional
option(ENABLE_TESTING "Build unit tests" OFF)
if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
