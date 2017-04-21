/*
 * memory.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    21.04.2017
*/



/**************** includes *********************/

#include "hbscheme.h"
#include "memory.h"
#include "printer.h"




/**************** variables & objects *********************/

static SCM_OBJ *symbolTable = NULL;
static int symbolTableSize = 0;
static int numKnownSymbols = 0;





/**************** global functions *********************/

/*
 * in the symboltable are stored all the validated characters
 */
void initSymbolTable() {
    symbolTable = (SCM_OBJ *)malloc(sizeof(SCM_OBJ) * INITIAL_SYMBOLTABLE_SIZE);
    symbolTableSize = INITIAL_SYMBOLTABLE_SIZE;
    numKnownSymbols = 0;
    memset(symbolTable, 0, (sizeof(SCM_OBJ) * INITIAL_SYMBOLTABLE_SIZE));
}





SCM_OBJ new_symbol(char* chars) {
    SCM_OBJ existingSymbol = getSymbol(chars);

    if (existingSymbol == NULL) {
        SCM_OBJ newSym = really_a_new_symbol(chars);

        remember_symbol(newSym);
        return newSym;
    }
    return existingSymbol;
}

SCM_OBJ new_string(char* chars) {
    int len = (int) strlen(chars);

    unsigned int nBytes = sizeof(struct scm_string)
                          - 1 /* eins zuviel in struct*/
                          + len
                          + 1 /*0-byte*/;

    SCM_OBJ o = (SCM_OBJ)malloc(nBytes);

    o->scm_string.tag = TAG_STRING;
    strcpy(o->scm_string.chars, chars);
    o->scm_string.chars[len] = '\0';
    return o;
}

SCM_OBJ new_integer(int iVal) {
    SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_integer));

    o->scm_integer.tag = TAG_INT;
    o->scm_integer.iVal = iVal;
    return o;
}

SCM_OBJ new_cons(SCM_OBJ theCar, SCM_OBJ theCdr) {
    SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_cons));

    o->scm_cons.tag = TAG_CONS;
    o->scm_cons.car = theCar;
    o->scm_cons.cdr = theCdr;
    return o;
}



SCM_OBJ new_eof() {
    return new_singleton(TAG_EOF);
}

SCM_OBJ new_true() {
    return new_singleton(TAG_TRUE);
}

SCM_OBJ new_false() {
    return new_singleton(TAG_FALSE);
}

SCM_OBJ new_void() {
    return new_singleton(TAG_VOID);
}

SCM_OBJ new_nil() {
    return new_singleton(TAG_NIL);
}



SCM_OBJ new_userDefinedFunction(SCM_OBJ argList, SCM_OBJ bodyList) {
    SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_userDefinedFunc));

    o->scm_userDefinedFunc.tag = TAG_USERDEFINEDFUNC;
    o->scm_userDefinedFunc.argList = argList;
    o->scm_userDefinedFunc.bodyList = bodyList;
    return o;
}

SCM_OBJ new_builtinFunc(VOIDFUNC funcPtr) {
    SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_builtinFunc));

    o->scm_builtinFunc.tag = TAG_BUILTINFUNC;
    o->scm_builtinFunc.code = funcPtr;
    return o;
}

SCM_OBJ new_builtinSyntax(VOIDFUNC syntaxPtr) {
    SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_builtinSyntax));

    o->scm_builtinSyntax.tag = TAG_BUILTINSYNTAX;
    o->scm_builtinSyntax.code = syntaxPtr;
    return o;
}







/**************** local functions *********************/

// Resize the SymbolTable
static void growSymbolTable() {
    int newSize = (symbolTableSize * 2) + 1;
    SCM_OBJ *newTable = (SCM_OBJ *)malloc(sizeof(SCM_OBJ) * newSize);
    int i;

    memset(newTable, 0, (sizeof(SCM_OBJ) * newSize));

    for (i=0; i<symbolTableSize; i++) {
        SCM_OBJ slotValue = symbolTable[i];

        if (slotValue != NULL) {
            int newIdx, newIdx0;

            newIdx = newIdx0 = haschSize(slotValue->scm_symbol.chars) % newSize;
            for (;;) {
                if (newTable[newIdx] == NULL) {
                    newTable[newIdx] = slotValue;
                    break;
                }
                newIdx++;
                if (newIdx == newSize) {
                    newIdx = 0;
                }
                if (newIdx == newIdx0) {
                    // cannot happen
                    FATAL("new table full");
                }
            }
        }
    }
    free(symbolTable);
    symbolTable = newTable;
    symbolTableSize = newSize;
}

static void possiblyGrowSymbolTable() {
    if (numKnownSymbols > (symbolTableSize * 3 / 4)) {
        growSymbolTable();
    }
}

static void remember_symbol(SCM_OBJ theSymbolToRemember) {
    ASSERT_SYMBOL(theSymbolToRemember);
    int idx0, idx;

    idx0 = idx = haschSize(theSymbolToRemember->scm_symbol.chars) % symbolTableSize;

    for (;;) {
        SCM_OBJ slotValue = symbolTable[idx];

        if (slotValue == NULL) {
            symbolTable[idx] = theSymbolToRemember;
            numKnownSymbols++;
            possiblyGrowSymbolTable();
            return;
        }
        idx++;
        if (idx == symbolTableSize) {
            idx = 0;
        }
        if (idx == idx0) {
            // cannot happen
            FATAL("symboltable full");
        }
    }

    if (numKnownSymbols == symbolTableSize) {
        if (symbolTableSize == 0) {
            symbolTable = (SCM_OBJ *)malloc(sizeof(SCM_OBJ) * INITIAL_SYMBOLTABLE_SIZE);
            symbolTableSize = INITIAL_SYMBOLTABLE_SIZE;
        } else {
            int newSize = symbolTableSize * 2;
            symbolTable = (SCM_OBJ*)realloc(symbolTable, sizeof(SCM_OBJ) * newSize);
            symbolTableSize = newSize;
        }
    }
    symbolTable[numKnownSymbols++] = theSymbolToRemember;
}



static SCM_OBJ really_a_new_symbol(char* chars) {
    unsigned int len = (unsigned int) strlen(chars);

    unsigned int nBytes = (unsigned int)sizeof(struct scm_symbol)
                          - 1 /* eins zuviel in struct*/
                          + len
                          + 1 /*0-byte*/;

    SCM_OBJ o = (SCM_OBJ)malloc(nBytes);

    o->scm_symbol.tag = TAG_SYMBOL;
    strcpy(o->scm_symbol.chars, chars);
    o->scm_symbol.chars[len] = '\0';
    return o;
}

static SCM_OBJ getSymbol(char *chars) {
    int idx, idx0;

    idx = idx0 = haschSize(chars) % symbolTableSize;

    for (;;) {
        SCM_OBJ existingAtI = symbolTable[idx];

        if (existingAtI == NULL) {
            return NULL;
        }
        if (strcmp(existingAtI->scm_symbol.chars, chars) == 0) {
            return existingAtI;
        }
        idx++;
        if (idx == symbolTableSize) {
            idx = 0;
        }
        if (idx == idx0) {
            return NULL;
        }
    }
}



// One and Only Symbol instance
static SCM_OBJ new_singleton(scm_tag tag) {
    static SCM_OBJ singleton[MAX_TAG];

    if (singleton[tag] == NULL) {
        SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_special));

        o->scm_integer.tag = tag;
        singleton[tag] = o;
    }
    return singleton[tag];
}



// Hashing function
static inline long haschSize(char *cp) {
    long hVal = 0;
    char c;

#ifdef USE_REAL_HASH
    while ((c = *cp++) != '\0') {
        hVal = (hVal*63)+(c & 0xFF);
    }
#endif
    return(hVal);
}