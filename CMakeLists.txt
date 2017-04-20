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
        hbscheme.c
        memory.c
        environment.c
        test.c
        reader.c
        utils.c
        )




##########################################
#           Operations                   #
###########################################

add_executable(hbScheme ${SOURCE_FILES})