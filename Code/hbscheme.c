/*
 * hbScheme
 *
 * Author:       Hannes Buchwald
 * Project:      Scheme Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    05.02.2017
 * License:      MIT License
 *
 *
 * MIT License
 * Copyright (c) [2017] [Hannes Buchwald]
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "hbscheme.h"


static jmp_buf getMeOutOfHere;


int main() {
    OBJ scm_input = newFileStream(stdin, 10);



    printf("Welcome to HbScheme\n");


//    if (setjmp(getMeOutOfHere) != 0) {
//        printf("here again\n");
//    }
//    printf("after setjmp\n");


    for (;;) {

        //
        printf("> ");

        // read Input stream and store it into a char
        OBJ o = scm_read(scm_input);

        //Evaluate the input
        OBJ rslt = eval(o);

        // print out result
        scm_print(rslt);


        free(o);
        free(rslt);

//        printf("\n");
//        // rslt = eval(o);
//       // scm_print(stdout, rslt);
//        scm_print(stdout, o);
//        printf("\n");
    }


    return 0;
}