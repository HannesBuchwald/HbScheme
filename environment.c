/*
 * environment.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    20.04.2017
 *
 */




/**************** includes *********************/

#include "hbscheme.h"
#include "environment.h"
#include "memory.h"



/**************** variables & objects *********************/

struct environment* environment = NULL;

SCM_OBJ SCM_NIL = NULL;
SCM_OBJ SCM_TRUE = NULL;
SCM_OBJ SCM_FALSE = NULL;
SCM_OBJ SCM_VOID = NULL;
SCM_OBJ SCM_EOF = NULL;
SCM_OBJ SCM_quoteSymbol = NULL;





/**************** global functions *********************/




/** init functions **/

void initEnvironment() {
    environment = allocateEnvironment(INITIAL_ENVIRONMENT_SIZE);

    initWellKnownObjects();
}

void initWellKnownObjects() {
    SCM_NIL = new_nil();
    SCM_TRUE = new_true();
    SCM_FALSE = new_false();
    SCM_VOID = new_void();
    SCM_EOF = new_eof();
    SCM_quoteSymbol = new_symbol("quote"); //?

}





/** binding functions **/

void add_binding(SCM_OBJ theKey, SCM_OBJ newValue) {
    int i0, idx;
    struct tableEntry *entryPtr;

    i0 = idx = (int)( H(theKey) % environment->numSlotsAllocated);
    for (;;) {
        entryPtr = &(environment->entries[idx]);
        if (entryPtr->key == NULL) {
            entryPtr->key = theKey;
            entryPtr->value = newValue;
            environment->numSlotsInUse++;
            if (environment->numSlotsInUse > (environment->numSlotsAllocated * 3 / 4)) {
                resizeEnvironment();
            }
            return;
        }
        if (entryPtr->key == theKey) {
            entryPtr->value = newValue;
            return;
        }

        idx = (idx + 1) % environment->numSlotsAllocated;
        if (idx == i0) {
            FATAL("cannot happen");
        }
    }

    FATAL("unimpl.");
}


SCM_OBJ get_binding(SCM_OBJ searchedKey) {
    int i0, idx;
    struct tableEntry *entryPtr;

    ASSERT(isSymbol(searchedKey), "non-symbol key");

    // fprintf(stdout, "the searchedKey is %s {%p}\n", symbolValue(searchedKey), searchedKey);

    i0 = idx = (int)( H(searchedKey) % environment->numSlotsAllocated );
    for (;;) {
        entryPtr = &(environment->entries[idx]);
        if (entryPtr->key == searchedKey) {
            return (entryPtr->value);
        }
        if (entryPtr->key == NULL) {
            return NULL;
        }
        idx = (idx+1) % environment->numSlotsAllocated;
        if (idx == i0) {
            FATAL("cannot happen");
        }
    }
}









/**************** local functions *********************/



static struct environment* allocateEnvironment(int size) {
    unsigned int numBytes;
    struct environment* newEnv;

    numBytes = (unsigned int)sizeof(struct environment)
               + ((size-1)*(unsigned int)sizeof(struct tableEntry));
    newEnv = (struct environment*) malloc(numBytes);
    memset(newEnv, 0, numBytes);

    newEnv->numSlotsInUse = 0;
    newEnv->numSlotsAllocated = size;
    return newEnv;
}


void resizeEnvironment() {
    int oldIdx, oldSize, newSize;
    struct environment *newEnvironment;

    oldSize = environment->numSlotsAllocated;
    newSize = oldSize * 2 + 1;

    newEnvironment = allocateEnvironment(newSize);
    for (oldIdx=0; oldIdx<oldSize; oldIdx++) {
        SCM_OBJ oldKey;

        oldKey = environment->entries[oldIdx].key;
        if (oldKey != NULL) {
            SCM_OBJ oldValue;
            int newI0, newIdx;

            oldValue = environment->entries[oldIdx].value;

            newI0 = newIdx = (int)( H(oldKey) % newSize );
            for (;;) {
                if (newEnvironment->entries[newIdx].key == NULL) {
                    newEnvironment->entries[newIdx].key = oldKey;
                    newEnvironment->entries[newIdx].value = oldValue;
                    break;
                }
                newIdx = (newIdx + 1) % newSize;
                if (newIdx == newI0) {
                    FATAL("cannot happen");
                }
            }
        }
    }
    newEnvironment->numSlotsInUse = environment->numSlotsInUse;
    environment = newEnvironment;
}



static inline unsigned long H(SCM_OBJ o) {
    return (unsigned long)o;
}
