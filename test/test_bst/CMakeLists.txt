include_directories(
  ${SOURCE_DIR}
  ${GTEST_INCLUDE_DIR}
)

add_executable(test_bst
  test_bst.cpp
)

target_link_libraries(test_bst
  lib${PROJECT_NAME}
  ${GTEST_LIB_DIR}/libgtest.a
  pthread
)

install(TARGETS test_bst DESTINATION ${INSTALL_BIN_DIR})