include_directories(
  ${SOURCE_DIR}
  ${GTEST_INCLUDE_DIR}
)

add_executable(test_map
  test_map.cpp
)

target_link_libraries(test_map
  lib${PROJECT_NAME}
  ${GTEST_LIB_DIR}/libgtest.a
  pthread
)

install(TARGETS test_map DESTINATION ${INSTALL_BIN_DIR})