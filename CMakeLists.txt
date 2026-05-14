cmake_minimum_required(VERSION 3.16)
project(dd_pow)

set(CMAKE_CXX_STANDARD 14)

add_executable(dd_pow main.cpp exact_math.h dd.cpp dd.h utils.h)