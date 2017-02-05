/*
 * Printer
 *
 * Author:       Hannes Buchwald
 * Version:      0.0.1
 * Last edit:    05.02.2017
*/


#include <stdio.h>

#include "hbscheme.h"


void
scm_print(OBJ o) {



    printf("result: %s\n", o->u.fileStream.string);

}