cmake_minimum_required(VERSION 3.10)
project(hybird)

set(CMAKE_CXX_STANDARD 11)

add_executable(hybird main.cpp P_G_C.h P_G_C.cpp G_C.h G_C.cpp PCP.h)