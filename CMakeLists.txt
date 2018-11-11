cmake_minimum_required(VERSION 3.10)
project(hybird)

set(CMAKE_CXX_STANDARD 11)

add_executable(hybird main.cpp Partition_Graph_coloring.h Partition_Graph_coloring.cpp Graph_Coloring.h Graph_Coloring.cpp Partition_Coloring_Problem.h segtree_heap.h)