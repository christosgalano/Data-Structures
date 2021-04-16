include_directories(
  ${SOURCE_DIR}
  ${GTEST_INCLUDE_DIR}
)

add_executable(test_pq
  test_pq.cpp
)

target_link_libraries(test_pq
  lib${PROJECT_NAME}
  ${GTEST_LIB_DIR}/libgtest.a
  pthread
)

install(TARGETS test_pq DESTINATION ${INSTALL_BIN_DIR})