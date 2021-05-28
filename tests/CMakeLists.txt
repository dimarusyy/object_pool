enable_testing()

add_executable(
  pool-tests
  pool-tests.cpp
)
target_link_libraries(
  pool-tests
  gtest_main
  object-pool
)

include(GoogleTest)
gtest_discover_tests(pool-tests)