cmake_minimum_required(VERSION 3.1)
project(rt_graph CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(rt_graph src/rt_graph.cpp)
set_property(TARGET rt_graph PROPERTY CXX_STANDARD 11)
set_property(TARGET rt_graph PROPERTY CXX_STANDARD_REQUIRED TRUE)

target_include_directories(rt_graph PUBLIC include)
