//
// Created by Hannes on 12.12.2016.
//
#include <stdlib.h>
#include "hbscheme.h"


char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';
    return realloc(str, sizeof(char)*len);
}


OBJ scm_read(OBJ inStream) {

    char *input = inputString(inStream->u.fileStream.file, inStream->u.fileStream.size_t);
    inStream->u.fileStream.string = input;

    return inStream;
}