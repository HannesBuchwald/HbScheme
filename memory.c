/*
 * memory.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    31.03.2017
 */


#include "hbscheme.h"
#include "memory.h"



/********* definition variable **********/
static OBJ *symbolTable = NULL;
static int symbolTableSize = 0;

// Counter for known symbols
static int knownSymbolCounter = 0;




/*
 * Init symboltable
 */
void initSymbolTable() {

    symbolTable = (OBJ *)malloc(sizeof(OBJ)* SYMBOLTABLE_SIZE);
    symbolTableSize = SYMBOLTABLE_SIZE;
    knownSymbolCounter = 0;

    memset(symbolTable, 0, (sizeof(OBJ) * SYMBOLTABLE_SIZE));

}