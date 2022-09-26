cmake_minimum_required(VERSION 3.23)

project(benchmarking)

set(CMAKE_CXX_STANDARD 20)

add_library(benchmarker INTERFACE)
target_include_directories(benchmarker
    INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_executable(benchmarker_test test.cpp)
target_link_libraries(benchmarker_test PUBLIC benchmarker)
