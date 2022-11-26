cmake_minimum_required(VERSION 3.16)

project(a)

include_directories(include)

find_package(OpenBLAS REQUIRED)

SET(CMAKE_C_COMPILER "gcc")

Set(CMAKE_C_FLAGS "-mavx512f -O3 -march=native")

include_directories(/opt/OpenBLAS/include/)
link_directories(/opt/OpenBLAS/lib/)

add_executable(a main.c src/func.c)

target_link_libraries(a openblas)

