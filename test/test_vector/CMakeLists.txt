include_directories(
  ${SOURCE_DIR}
)

add_executable(test_vector
  test_vector.cpp
)

target_link_libraries(test_vector
  ${PROJECT_NAME}
  GTest::gtest_main
  pthread
)
