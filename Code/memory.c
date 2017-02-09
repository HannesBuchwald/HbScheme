/*
 * Memory
 *
 * Author:       Hannes Buchwald
 * Version:      0.0.1
 * Last edit:    05.02.2017
*/


#include <stdlib.h>
#include "hbscheme.h"

// called from the main function - read what is typed in
// Set the size of the struct depending on the size of the input file
// Add the inputStream as a File (the inputStream is not evaluated now)
// Add tags as T_FILESTREAM

OBJ newFileStream(FILE *file) {


    if( file == NULL) {
        perror("Unable to allocate buffer");
        exit(1);
    }

    struct schemeFileStream *theStream;
    theStream = (struct schemeFileStream * )(malloc( sizeof(struct schemeFileStream)));
    theStream -> tag = T_FILESTREAM;
    theStream -> file = file;

    return (OBJ)theStream;
}


OBJ newString() {


    struct schemeFileStream *theStream;
    theStream = (struct schemeFileStream * )(malloc(32 * sizeof(char)));
    theStream -> tag = T_FILESTREAM;
    theStream -> string = "";

    return (OBJ)theStream;
}