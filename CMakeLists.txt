cmake_minimum_required(VERSION 3.12)
project(untitled6)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(googletest-master)
include_directories(googletest-master/googletest/include)
include_directories(googletest-master/googlemock/include)
add_executable(untitled6 main.cpp Tests/BinaryHeap.h Tests/Test.cpp)
target_link_libraries(untitled6 gtest gtest_main)