cmake_minimum_required(VERSION 3.10...3.30)

project(YorkTest LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

add_executable(york_tests
  display_test.cpp
  input_test.cpp
)
target_link_libraries(york_tests PRIVATE
  york
  GTest::gtest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(york_tests)
