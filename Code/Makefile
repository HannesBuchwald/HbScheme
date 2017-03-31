
# Makefile
#
# Author:       Hannes Buchwald
# Project:      hbScheme Interpreter (University of Media)
# Version:      0.0.1
# Last edit:    31.03.2017






##########################################
#           Editable options             #
###########################################


## Compiler options
CC = clang  
CFLAGS= -g -pedantic -Wall -std=c99
APPLICATION = hbSheme


## Compiling files
OBJS=hbscheme.o hbscheme_test.o \
     reader.o memory.o \
     utils.o





##########################################
#           Operations                   #
###########################################


## Clean up
clean:
	rm -f $(OBJS)



## include main header file to all files 
$(OBJS): hbscheme.h


## Linking all object files togehter
$(APPLICATION): $(OBJS)
	$(CC) $(DEBUG_FLAGS) -o $(APPLICATION) $(OBJS)








## run in eclipse  
all: $(APPLICATION)