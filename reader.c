/*
 * reader.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    31.03.2017
*/


#include "hbscheme.h"
#include "reader.h"




/*******  functions **********/


stream newFileStream(FILE *input) {
    stream s = (stream)malloc(sizeof(struct stream_struct));
    s->type = FILESTREAM;
    s->fileStream = input;
    s->peekChar = 0;
    return s;
}