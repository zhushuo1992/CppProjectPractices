cmake_minimum_required(VERSION 3.21)
project(05)

set(CMAKE_CXX_STANDARD 14)

add_executable(05 main.cpp client.cpp cpplang.hpp SalesData.hpp Zmq.hpp Config.hpp SpinLock.hpp srv.cpp Summary.hpp)
