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