/*
 * Printer
 *
 * Author:       Hannes Buchwald
 * Version:      0.0.1
 * Last edit:    09.02.2017
*/


#include <stdio.h>
#include "hbscheme.h"


void scm_print(OBJ o) {

    if(o->u.fileStream.size_t != NULL) {
        printf("%zu characters were read.\n",o->u.fileStream.size_t);
    }

    if(o->u.fileStream.string != NULL) {
        printf("You typed: %s\n", o->u.fileStream.string);
    }
}


void scm_print_Close() {
    printf("Your hbScheme Interpreter is closing now");
}