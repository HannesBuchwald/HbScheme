#
# Cmake setup
#
# Author:       Hannes Buchwald
# Project:      Scheme Interpreter (University of Media)
# Version:      0.0.1
# Last edit:    05.02.2017



# Set cmake requrements
cmake_minimum_required(VERSION 3.7)

# Project Name
project(mySchemeInterpreter)

# cmake flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Include all files to build
set(SOURCE_FILES hbscheme.c hbscheme.h memory.c reader.c print.c eval.c)

#
enable_testing()

add_executable(mySchemeInterpreter ${SOURCE_FILES})
