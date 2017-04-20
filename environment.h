//
// Created by admin on 08.04.17.
//

#ifndef HBSCHEME_ENVIRONMENT_H
#define HBSCHEME_ENVIRONMENT_H

#endif //HBSCHEME_ENVIRONMENT_H


#define ENVIRONMENT_SIZE    3




struct tableEntry {
    OBJ key;
    OBJ value;
};

struct environment {
    int numberOfUsedSlots;
    int numberOfAllocatedSlots;
    struct tableEntry entries[1];
};



struct environment *allocateEnvironment(int size);


void initEnvironment();
