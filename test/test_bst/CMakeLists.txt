include_directories(
  ${SOURCE_DIR}
)

add_executable(test_bst
  test_bst.cpp
)

target_link_libraries(test_bst
  ${PROJECT_NAME}
  GTest::gtest_main
  pthread
)
