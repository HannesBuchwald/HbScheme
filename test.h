//
// Created by admin on 08.04.17.
//






#define ASSERT(cond, msg) ASSERT2(cond, msg, __FILE__, __LINE__)
#define ASSERT2(cond, msg, fileName, lineNr) \
    if (! (cond) ) { \
	fprintf(stderr, "%s:%d assertion failed: %s", \
			fileName, lineNr, msg); \
	abort(); \
    }





/*******  global functions ********/

extern void test();






/******* local functions ***********/

void unitTests();
void a2lTest(char *input);




