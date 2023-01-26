include_directories(
  ${INCLUDE_DIR}
)

add_executable(test_map
  test_map.cpp
)

target_link_libraries(test_map
  ${PROJECT_NAME}
  GTest::gtest_main
  pthread
)
