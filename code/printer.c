/*
 * printer.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    21.04.2017
*/



/**************** includes *********************/

#include "../header/hbscheme.h"
#include "../header/printer.h"




/**************** local functions *********************/

static void scm_printListRest(FILE* outFile, SCM_OBJ theCons, enum printHowEnum printHow) {
    scm_print(outFile, car(theCons), printHow);
    if (isNil(cdr(theCons))) {
        fprintf(outFile, ")");
        return;
    }
    if (! isCons(cdr(theCons))) {
        fprintf(outFile, " . ");
        scm_print(outFile, cdr(theCons), printHow);
        fprintf(outFile, ")");
        return;
    }

    fprintf(outFile, " ");
    scm_printListRest(outFile, cdr(theCons), printHow);
}




/**************** global functions *********************/



void scm_printList(FILE* outFile, SCM_OBJ theCons, enum printHowEnum printHow) {
    fprintf(outFile, "(");
    scm_printListRest(outFile, theCons, printHow);
}

void scm_print(FILE* outFile, SCM_OBJ o, enum printHowEnum printHow) {
    switch (tagOf(o)) {
        case TAG_INT:
            fprintf(outFile, "%ld", intValue(o));
            break;

        case TAG_FLOAT:
            fprintf(outFile, "%lf", floatValue(o));
            break;

        case TAG_SYMBOL:
            fprintf(outFile, "%s", symbolValue(o));
            break;

        case TAG_STRING:
            if (printHow == PRINT_WRITE) {
                fprintf(outFile, "\"%s\"", stringValue(o));
            } else {
                fprintf(outFile, "%s", stringValue(o));
            }
            break;

        case TAG_CONS:
            scm_printList(outFile, o, printHow);
            break;

        case TAG_NIL:
            fprintf(outFile, "()");
            break;

        case TAG_TRUE:
            fprintf(outFile, "#t");
            break;

        case TAG_FALSE:
            fprintf(outFile, "#f");
            break;

        case TAG_EOF:
            fprintf(outFile, "#eof");
            break;

        case TAG_VOID:
            fprintf(outFile, "#void");
            break;

        case TAG_BUILTINFUNC:
            fprintf(outFile, "<some builtin function>");
            break;

        case TAG_BUILTINSYNTAX:
            fprintf(outFile, "<some builtin syntax>");
            break;

        case TAG_USERDEFINEDFUNC:
            fprintf(outFile, "(lambda ");
            scm_print(outFile, o->scm_userDefinedFunc.argList, printHow);
            fprintf(outFile, " ");
            scm_printListRest(outFile, o->scm_userDefinedFunc.bodyList, printHow);
            break;

        default:
            fprintf(outFile, "print unimplemented");
            break;

    }
}

void fatal(char* msg, char* fileName, int lineNr) {
    fprintf(stderr, "%s:%d: %s\n", fileName, lineNr, msg);
    abort();
}



