cmake_minimum_required(VERSION 3.21)
project(ArrowThreadPool)

set(CMAKE_CXX_STANDARD 17)

add_executable(ArrowThreadPool main.cpp cancel.h status.h visibility.h cancel.cpp marcos.h executor.h functions.h io_util.h io_util.cpp thread_pool.h thread_pool.cpp)
