#include "hbscheme.h"
#include "setjmp.h"

#ifdef DEBUG
# define DBG_PRINT(x) fprintf x
#else
# define DBG_PRINT(x) /* as nothing */
#endif

static jmp_buf getMeBackToRepl;

SCM_OBJ SCM_NIL = NULL;
SCM_OBJ SCM_TRUE = NULL;
SCM_OBJ SCM_FALSE = NULL;
SCM_OBJ SCM_VOID = NULL;
SCM_OBJ SCM_EOF = NULL;
SCM_OBJ SCM_quoteSymbol = NULL;
static scm_stream stdInputStream = NULL;

void
scm_error(char* msg, SCM_OBJ optionalArg) {
    fprintf(stderr, "%s", msg);
    if (optionalArg != NULL) {
        scm_print(stderr, optionalArg, PRINT_WRITE);
    }
    fprintf(stderr, "\n");
    DBG_PRINT((stderr, "going back to REPL...\n"));
    longjmp(getMeBackToRepl, 1);
}

static void
initializeSystem() {
    stdInputStream = new_fileStream(stdin);
    SCM_NIL = new_nil();
    SCM_TRUE = new_true();
    SCM_FALSE = new_false();
    SCM_VOID = new_void();
    SCM_EOF = new_eof();

    initializeSymbolTable();

    SCM_quoteSymbol = new_symbol("quote");

    initializeEvalStack();
    initializeEnvironment();
    initializeBuiltinFunctions();
}

static void
repl() {
    SCM_OBJ expr, rslt;

    for (;;) {
        fprintf(stdout, "> "); // fflush(stdout);
        expr = scm_read(stdInputStream);
        rslt = scm_eval(expr);
        if (! isVoid(rslt)) {
            scm_print(stdout, rslt, PRINT_WRITE);
            fprintf(stdout, "\n");
        }
    }
}

int
main() {
    initializeSystem();

    printf("Welcome to our (incomplete) scheme\n");
    scm_selftest();

    // -----

    if (setjmp(getMeBackToRepl)) {
        DBG_PRINT((stderr, "back in REPL.\n"));
    } else {
        DBG_PRINT(("after setjmp.\n"));
    }
    repl();

    exit(0);
}

void
fatal(char* msg, char* fileName, int lineNr) {
    fprintf(stderr, "%s:%d: %s\n", fileName, lineNr, msg);
    abort();
}
