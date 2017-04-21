/*
 * environment.h
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    20.04.2017
 *
 */



/**************** structs *********************/

struct tableEntry {
    SCM_OBJ key;
    SCM_OBJ value;
};

struct environment {
    int numSlotsInUse;
    int numSlotsAllocated;
    struct tableEntry entries[1];
};








/**************** global functions *********************/

void initEnvironment();
void initWellKnownObjects();

SCM_OBJ get_binding(SCM_OBJ key);
void add_binding(SCM_OBJ key, SCM_OBJ value);









/**************** local functions *********************/

static struct environment* allocateEnvironment(int size);
void resizeEnvironment();


static inline unsigned long H(SCM_OBJ o);








