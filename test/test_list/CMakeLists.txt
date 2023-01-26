include_directories(
  ${SOURCE_DIR}
)

add_executable(test_list
  test_list.cpp
)

target_link_libraries(test_list
  ${PROJECT_NAME}
  GTest::gtest_main
  pthread
)
