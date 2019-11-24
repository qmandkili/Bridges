cmake_minimum_required(VERSION 3.15)
project(graph-bridges)

set(CMAKE_CXX_STANDARD 14)

add_executable(graph-bridges main.cpp include/Edge.h source/Edge.cpp include/Sorts.h source/Sorts.cpp include/Utils.h source/Utils.cpp include/IO.h source/IO.cpp include/Algo.h source/Algo.cpp)