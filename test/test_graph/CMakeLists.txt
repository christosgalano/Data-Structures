include_directories(
  ${SOURCE_DIR}
  ${GTEST_INCLUDE_DIR}
)

add_executable(test_graph
  test_graph.cpp
)

target_link_libraries(test_graph
  lib${PROJECT_NAME}
  ${GTEST_LIB_DIR}/libgtest.a
  pthread
)

install(TARGETS test_graph DESTINATION ${INSTALL_BIN_DIR})