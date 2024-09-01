cmake_minimum_required(VERSION 3.16)

project(fetalibTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

enable_testing()

# ---- Tests ----

file(GLOB_RECURSE fetalib_test_sources "src/*.cpp")
add_executable(fetalib_test ${fetalib_test_sources})
target_link_libraries(fetalib_test 
  PRIVATE
  GTest::gtest_main
  fetalib::fetalib)
target_compile_features(fetalib_test PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(fetalib_test)

# ---- End-of-file commands ----

add_folders(Test)
