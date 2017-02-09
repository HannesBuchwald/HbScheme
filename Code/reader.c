

/*
 * Reader
 *
 * Author:       Hannes Buchwald
 * Version:      0.0.1
 * Last edit:    09.02.2017
*/




#include <stdlib.h>
#include "hbscheme.h"
#include <string.h>




// check String
char readString(OBJ o) {

    if(o->u.fileStream.tag == T_FILESTREAM) {

        if(strchr(o->u.fileStream.string, 'exit')) {
            scm_print_Close();
            exit(0);
        }
    }
}


// Pack inputstring and inputsize in OBJ
OBJ scm_readt(OBJ o, char* buffer, size_t character){

    o->u.fileStream.string = buffer;
    o->u.fileStream.size_t = character;

    readString(o);
    return o;
};