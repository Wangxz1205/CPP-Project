cmake_minimum_required(VERSION 3.16)

project(a)

include_directories(include)

SET(CMAKE_CXX_COMPILER "g++")

add_executable(a main.cpp src/matrix.cpp)


