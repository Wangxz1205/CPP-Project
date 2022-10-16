cmake_minimum_required(VERSION 3.16)

project(a)

include_directories(include)

add_executable(a main.cpp data.cpp trans.cpp cal.cpp mul.cpp add.cpp min.cpp div.cpp cmp.cpp judgefuzhi.cpp judgeother.cpp)