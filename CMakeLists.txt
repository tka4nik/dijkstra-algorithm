cmake_minimum_required(VERSION 3.20)
project(dijkstra)

set(CMAKE_CXX_STANDARD 17)

add_executable(dijkstra dijkstra/dijkstra.cpp dijkstra/main.cpp)
target_include_directories(dijkstra PUBLIC graph/)

