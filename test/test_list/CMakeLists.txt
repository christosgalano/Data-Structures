include_directories(
  ${SOURCE_DIR}
  ${GTEST_INCLUDE_DIR}
)

add_executable(test_list
  test_list.cpp
)

target_link_libraries(test_list
  lib${PROJECT_NAME}
  ${GTEST_LIB_DIR}/libgtest.a
  pthread
)

install(TARGETS test_list DESTINATION ${INSTALL_BIN_DIR})