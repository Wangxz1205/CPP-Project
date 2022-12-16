cmake_minimum_required(VERSION 3.16)

project(main)

include_directories(include)

find_package(OpenBLAS REQUIRED)

SET(CMAKE_CXX_COMPILER "g++")

Set(CMAKE_CXX_FLAGS "-mavx512f -mfma -O3 -Wpsabi -march=native")

include_directories(/opt/OpenBLAS/include/)
link_directories(/opt/OpenBLAS/lib/)

add_executable(main main.cpp matrix.cpp)

target_link_libraries(main openblas)