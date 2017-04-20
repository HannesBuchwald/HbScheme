/*
 * main_test.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    31.03.2017
 */




#include "hbscheme.h"
#include "test.h"



//
// main test function
//
void test() {

    unitTests();

}





void unitTests() {

    char a2lTestCases[4] = {"0", "10", "999", "7498759837489579384"};

    a2lTest(a2lTestCases);

}






void a2lTest(char *input) {




   // ASSERT( a2l("0") == 0, "a2l error");

}