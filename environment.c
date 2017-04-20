//
// Created by admin on 08.04.17.
//


#include "hbscheme.h"
#include "environment.h"



/************  Definition *************/

struct environment *environment = NULL;











/************  Function *************/

void initEnvironment() {
    environment = allocateEnvironment(ENVIRONMENT_SIZE);
}




// Allocated the size of the new Environment with 0 slots
struct environment *allocateEnvironment(int size) {

    int numberOfBytes;
    int numberOfSlots = 0;
    struct environment *newEnvironment;

    numberOfBytes = sizeof(struct environment) + ((size-1) *sizeof(struct tableEntry));
    newEnvironment = (struct environment*) malloc(numberOfBytes);
    memset(newEnvironment, numberOfSlots, numberOfBytes);

    newEnvironment->numberOfUsedSlots = numberOfSlots;
    newEnvironment->numberOfAllocatedSlots = size;
    return newEnvironment;
}