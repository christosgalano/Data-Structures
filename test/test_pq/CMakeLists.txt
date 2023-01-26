include_directories(
  ${INCLUDE_DIR}
)

add_executable(test_pq
  test_pq.cpp
)

target_link_libraries(test_pq
  ${PROJECT_NAME}
  GTest::gtest_main
  pthread
)
