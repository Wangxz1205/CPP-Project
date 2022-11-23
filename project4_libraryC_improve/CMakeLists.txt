cmake_minimum_required(VERSION 3.16)

project(a)

include_directories(include)

SET(CMAKE_C_COMPILER "gcc")

Set(CMAKE_C_FLAGS "-mavx512f")

add_executable(a main.c src/func.c)

