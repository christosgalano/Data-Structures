include_directories(
  ${SOURCE_DIR}
)

add_executable(test_trie
  test_trie.cpp
)

target_link_libraries(test_trie
  ${PROJECT_NAME}
  GTest::gtest_main
  pthread
)
