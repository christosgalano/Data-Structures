include_directories(
  ${SOURCE_DIR}
  ${GTEST_INCLUDE_DIR}
)

add_executable(test_trie
  test_trie.cpp
)

target_link_libraries(test_trie
  lib${PROJECT_NAME}
  ${GTEST_LIB_DIR}/libgtest.a
  pthread
)

install(TARGETS test_trie DESTINATION ${INSTALL_BIN_DIR})