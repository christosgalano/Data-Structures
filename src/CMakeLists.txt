# build and install the project lib

add_library(lib${PROJECT_NAME} INTERFACE)

target_include_directories(lib${PROJECT_NAME} INTERFACE src/*.hpp) 

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${INSTALL_INCLUDE_DIR} FILES_MATCHING PATTERN "*.h*")

# install(TARGETS lib${PROJECT_NAME} DESTINATION ${INSTALL_LIB_DIR}) -- in case the user wants to add .cpp files in the library
