cmake_minimum_required(VERSION 3.16)

project(a)

include_directories(include)

add_executable(a main.cpp src/data.cpp src/trans.cpp src/cal.cpp src/mul.cpp src/add.cpp src/min.cpp src/div.cpp src/cmp.cpp src/judgefuzhi.cpp src/judgeother.cpp)