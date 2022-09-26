cmake_minimum_required(VERSION 3.23)

project(benchmarking)

set(CMAKE_CXX_STANDARD 20)

add_library(benchmarker INTERFACE benchmarker.h)

add_executable(benchmarker_test test.cpp)
target_link_libraries(benchmarker_test INTERFACE benchmarker)
