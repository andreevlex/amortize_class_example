cmake_minimum_required(VERSION 3.6)
project(Amortize)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/UseAmortize.cpp src/Amortize.cpp src/Amortize.h)
add_executable(Amortize ${SOURCE_FILES})