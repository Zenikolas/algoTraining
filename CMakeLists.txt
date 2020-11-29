cmake_minimum_required(VERSION 3.17)
project(algoTraining)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
find_package(GTest REQUIRED)

add_executable(list-serialization list-serialization.cpp)
add_executable(remove-dups removedups.cpp)
add_executable(binary-representation binary-representation.cpp )

target_link_libraries(list-serialization GTest::GTest GTest::Main)
target_link_libraries(remove-dups GTest::GTest GTest::Main)
target_link_libraries(binary-representation GTest::GTest GTest::Main)

gtest_discover_tests(list-serialization)
gtest_discover_tests(remove-dups)
gtest_discover_tests(binary-representation)