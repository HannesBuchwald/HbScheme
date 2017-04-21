/*
 * pinter.h
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    21.04.2017
*/


/******* enums ********/

enum printHowEnum {
    PRINT_DISPLAY,
    PRINT_WRITE
};


/******* global functions ********/

void fatal(char* msg, char* fileName, int lineNr);


extern void scm_print(FILE*, SCM_OBJ, enum printHowEnum);
