/*
 * main.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    31.03.2017
*/



#include "hbscheme.h"
#include "memory.h"
#include "environment.h"
#include "reader.h"
#include "test.h"




/******* local variable definition **********/
static stream inputStream = NULL;



/******* main *********/

int main(int argc, char **argv) {

    init();

    // Welcome text
    printf("Welcome to hbScheme (developer state) \n");

    test();


    exit(0);
}







/******* functions *********/



/*
 * Init the system
 */
void init() {
    initSymbolTable();
    initEnvironment();
    inputStream = newFileStream(stdin);
}

