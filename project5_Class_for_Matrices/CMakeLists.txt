cmake_minimum_required(VERSION 3.16)

project(main)

include_directories(include)

SET(CMAKE_CXX_COMPILER "g++")

add_executable(main main.cpp matrix.cpp)


