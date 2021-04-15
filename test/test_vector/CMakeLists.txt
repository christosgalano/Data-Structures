include_directories(
  ${SOURCE_DIR}
  ${GTEST_INCLUDE_DIR}
)

add_executable(test_vector
  test_vector.cpp
)

target_link_libraries(test_vector
  lib${PROJECT_NAME}
  ${GTEST_LIB_DIR}/libgtest.a
  pthread
)

install(TARGETS test_vector DESTINATION ${INSTALL_BIN_DIR})