include_directories(
  ${INCLUDE_DIR}
)

add_executable(test_graph
  test_graph.cpp
)

target_link_libraries(test_graph
  ${PROJECT_NAME}
  GTest::gtest_main
  pthread
)
