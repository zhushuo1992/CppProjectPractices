cmake_minimum_required(VERSION 3.21)
project(ArrowMemPool)

set(CMAKE_CXX_STANDARD 17)

add_executable(ArrowMemPool main.cpp visibility.h marcos.h io_util.h status.h io_util.cpp string_builder.cpp string_builder.h allocator.cpp allocator.h memory_pool.cpp memory_pool.h)
