cmake_minimum_required(VERSION 3.10)
project(hybird)

set(CMAKE_CXX_STANDARD 11)

add_executable(hybird main.cpp C_P.h C_P.cpp P_P.h P_P.cpp PCP.h)