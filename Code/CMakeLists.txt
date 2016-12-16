cmake_minimum_required(VERSION 3.7)
project(Code)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES hbscheme.c hbscheme.h memory.c reader.c print.c eval.c)

add_executable(Code ${SOURCE_FILES})