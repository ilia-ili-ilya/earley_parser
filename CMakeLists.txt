cmake_minimum_required(VERSION 3.28)
project(earley_parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(earley_parser main.cpp earley.h tests.h)

