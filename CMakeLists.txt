# Makefile
#
# Author:       Hannes Buchwald
# Project:      hbScheme Interpreter (University of Media)
# Version:      0.0.1
# Last edit:    31.03.2017


##########################################
#           Editable options             #
###########################################

cmake_minimum_required(VERSION 3.7)
project(hbScheme)

set(CMAKE_CXX_STANDARD 11)


set(SOURCE_FILES
        code/hbscheme.c
        code/memory.c
        code/environment.c
        code/test.c
        code/reader.c
        code/utilities.c
        code/eval.c code/printer.c header/constants.h header/printer.h header/eval.h header/utilities.h)




##########################################
#           Operations                   #
###########################################

add_executable(hbScheme ${SOURCE_FILES})