cmake_minimum_required(VERSION 3.21)
project(04)

set(CMAKE_CXX_STANDARD 14)

add_executable(04 main.cpp jsontest.cpp gperf.cpp lua_shared.cpp lua_jit.cpp msgpacktest.cpp protobufftest.cpp pybind11test.cpp zmqtest.cpp)
