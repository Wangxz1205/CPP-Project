cmake_minimum_required(VERSION 3.16)

project(a)

include_directories(include)

SET(CMAKE_C_COMPILER "gcc")

Set(CMAKE_C_FLAGS "-mavx512f")

set(CMAKE_EXE_LINKER_FLAGS " -lopenblas -lpthread")

add_executable(a main.c src/func.c)

