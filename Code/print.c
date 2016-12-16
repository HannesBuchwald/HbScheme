//
// Created by Hannes on 12.12.2016.
//
#include <stdio.h>

#include "hbscheme.h"


void
scm_print(OBJ o) {



    printf("result: %s\n", o->u.fileStream.string);

}