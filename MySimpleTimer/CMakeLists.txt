cmake_minimum_required(VERSION 3.21)
project(MySimpleTimer)

set(CMAKE_CXX_STANDARD 14)

add_executable(MySimpleTimer main.cpp my_simple_timer.cpp my_simple_timer.h ThreadPool.h)
