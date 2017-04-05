# Makefile
#
# Author:       Hannes Buchwald
# Project:      hbScheme Interpreter (University of Media)
# Version:      0.0.1
# Last edit:    31.03.2017


##########################################
#           Editable options             #
###########################################

cmake_minimum_required(VERSION 3.6)
project(hbScheme)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS"-g -pedantic -Wall -std=c99")


set(SOURCE_FILES
        hbscheme.c
        hbscheme_test.c
        Makefile
        memory.c
        reader.c
        utils.c)




##########################################
#           Operations                   #
###########################################

add_executable(hbScheme ${SOURCE_FILES})