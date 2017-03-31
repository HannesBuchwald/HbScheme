/*
 * main.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    31.03.2017
*/



#include "hbscheme.h"


static void initializeSystem() {
    initializeSymbolTable();
}

// the program starts here
int main(int argc, char **argv) {
    initializeSystem();
    unitTest();
    performanceTest();
    printf("Welcome to our (incomplete) scheme\n");

    exit(0);
}



void fatal(char* msg, char* fileName, int lineNr) {
    fprintf(stderr, "%s:%d: %s\n", fileName, lineNr, msg);
    abort();
}
