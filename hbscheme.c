/*
 * hbscheme.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    20.04.2017
 *
 */



/**************** includes *********************/


#include "hbscheme.h"
#include "setjmp.h"

#include "test.h"
#include "eval.h"
#include "environment.h"
#include "memory.h"





/**************** variables & objects *********************/


#ifdef DEBUG
# define DBG_PRINT(x) fprintf x
#else
# define DBG_PRINT(x) /* as nothing */
#endif

static jmp_buf getMeBackToRepl;
SCM_OBJ input, result;




/**************** main function *********************/


/*
 * called from the system
 */
int main() {


    // init and test the system
    init();
    unitTests();


    // user interaction
    printf("Welcome to hbScheme\n");




    if (setjmp(getMeBackToRepl)) {
        DBG_PRINT((stderr, "back in REPL.\n"));
    } else {
        DBG_PRINT(("after setjmp.\n"));
    }


    // create new fileStream which is listen to commandline input
    scm_stream stdInputStream = new_fileStream(stdin);

    // forever loop
    for (;;) {
        fprintf(stdout, "> ");
        input = scm_read(stdInputStream);
        result = scm_eval(input);
        if (! isVoid(result)) {
            scm_print(stdout, result, PRINT_WRITE);
            fprintf(stdout, "\n");
        }
    }

    // never reach
    exit(0);
}





// called from eval
void backToRepl() {
    DBG_PRINT((stderr, "going back to REPL...\n"));
    longjmp(getMeBackToRepl, 1);
}






/**************** local function *********************/

/*
 * init the system
 */
void init() {
    initSymbolTable();
    initEnvironment();
    initEvalStack();
    initBuiltinFunctions();
}
