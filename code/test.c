/*
 * test.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    20.04.2017
*/


/**************** includes *********************/


#include "../header/hbscheme.h"
#include "../header/test.h"
#include "../header/environment.h"
#include "../header/eval.h"
#include "../header/memory.h"
#include "../header/reader.h"
#include "../header/utilities.h"





/**************** global function *********************/

void unitTests() {

    a2lTest();
    constsTest();
    readTest();
    symbolTableTest();
    environmentTest();
    evalTest();
    functionTest();
    syntaxTest();

    fprintf(stderr, "Done (selftest)\n");
}








/**************** local function *********************/


/*
 * convert string to integer testing
 */
static void a2lTest() {
    ASSERT(a2l("0") == 0, "a2l error");
    ASSERT(a2l("123456789") == 123456789, "a2l error");
    ASSERT(a2l("999999") == 999999, "a2l error");
    ASSERT(a2l("2432902008176640000") == 2432902008176640000, "a2l error");

#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "Done (a2lTest)\n");
#endif
}




/*
 * constants testing
 */
static void constsTest() {

    ASSERT(new_nil() == SCM_NIL, "nil check failed")
    ASSERT(new_true() == SCM_TRUE, "true check failed")
    ASSERT(new_false() == SCM_FALSE, "false check failed")
    ASSERT(new_void() == SCM_VOID, "void check failed")
    ASSERT(new_eof() == SCM_EOF, "eof check failed")

    ASSERT(tagOf(SCM_NIL) == TAG_NIL, "nil tag check failed")
    ASSERT(tagOf(SCM_TRUE) == TAG_TRUE, "true tag check failed")
    ASSERT(tagOf(SCM_FALSE) == TAG_FALSE, "false tag check failed")
    ASSERT(tagOf(SCM_EOF) == TAG_EOF, "eof tag check failed")
    ASSERT(tagOf(SCM_VOID) == TAG_VOID, "void tag check failed")

    ASSERT(hasTag(SCM_NIL, TAG_NIL), "nil tag check failed")
    ASSERT(hasTag(SCM_TRUE, TAG_TRUE), "true tag check failed")
    ASSERT(hasTag(SCM_FALSE, TAG_FALSE), "false tag check failed")
    ASSERT(hasTag(SCM_EOF, TAG_EOF), "eof tag check failed")
    ASSERT(hasTag(SCM_VOID, TAG_VOID), "void tag check failed")

    ASSERT(isTrue(SCM_TRUE), "true tag check failed")
    ASSERT(isFalse(SCM_FALSE), "false tag check failed")
    ASSERT(isNil(SCM_NIL), "nil tag check failed")

#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "Done (constsTest)\n");
#endif
}



/*
 * reader testing
 */
static void readTest() {

    SCM_OBJ rslt;

    rslt = scm_readFromCString("");
    ASSERT(rslt == SCM_EOF, "empty read check failed")

    rslt = scm_readFromCString("    ");
    ASSERT(rslt == SCM_EOF, "empty read check failed")

    rslt = scm_readFromCString("123");
    ASSERT(rslt != NULL, "int check failed (NULL result)")
    ASSERT(tagOf(rslt) == TAG_INT, "int tag check failed")
    ASSERT(isInteger(rslt), "int tag check failed")
    ASSERT(!isSymbol(rslt), "int tag check failed")
    ASSERT(!isCons(rslt), "int tag check failed")
    ASSERT(!isString(rslt), "int tag check failed")
    ASSERT(rslt->scm_integer.iVal == 123, "int check failed")

    rslt = scm_readFromCString("    123");
    ASSERT(isInteger(rslt), "int tag check failed")
    ASSERT(rslt->scm_integer.iVal == 123, "int check failed")

    rslt = scm_readFromCString("999");
    ASSERT(isInteger(rslt), "int tag check failed")
    ASSERT(rslt->scm_integer.iVal == 999, "int check failed")

    rslt = scm_readFromCString("99999999");
    ASSERT(isInteger(rslt), "int tag check failed")
    ASSERT(rslt->scm_integer.iVal == 99999999, "int check failed")

    rslt = scm_readFromCString("-99999999");
    ASSERT(isInteger(rslt), "int tag check failed")
    ASSERT(rslt->scm_integer.iVal == -99999999, "int check failed")


    rslt = scm_readFromCString(" \"hello\" ");
    ASSERT(tagOf(rslt) == TAG_STRING, "string tag check failed")
    ASSERT(isString(rslt), "string tag check failed")
    ASSERT(strcmp(rslt->scm_string.chars, "hello") == 0, "string check failed")

    rslt = scm_readFromCString(" \"hel\\nlo\" ");
    ASSERT(isString(rslt), "string tag check failed")
    ASSERT(strcmp(rslt->scm_string.chars, "hel\nlo") == 0, "string check failed")



    //
    // tag testing
    //
    SCM_OBJ sym_abcde1, sym_abcde2, sym_abcde3,
            sym_bbb1, sym_bbb2, symx1, symx2;

    // tur
    rslt = scm_readFromCString(" #t ");
    ASSERT(rslt == SCM_TRUE, "true tag check failed")

    // false
    rslt = scm_readFromCString(" #f ");
    ASSERT(rslt == SCM_FALSE, "false tag check failed")

    // nill
    rslt = scm_readFromCString(" nil ");
    ASSERT(rslt == SCM_NIL, "nil tag check failed")

    rslt = scm_readFromCString(" #void ");
    ASSERT(rslt == SCM_VOID, "void tag check failed")


    // blank
    rslt = scm_readFromCString(" abc ");
    ASSERT(rslt->scm_any.tag == TAG_SYMBOL, "symbol tag check failed")
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(rslt->scm_symbol.chars, "abc") == 0, "symbol check failed")
    ASSERT(strcmp(symbolValue(rslt), "abc") == 0, "symbol check failed")

    // if
    rslt = scm_readFromCString(" if ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "if") == 0, "symbol check failed")

    rslt = scm_readFromCString(" define ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "define") == 0, "symbol check failed")

    rslt = scm_readFromCString(" 1+ ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "1+") == 0, "symbol check failed")

    rslt = scm_readFromCString(" + ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "+") == 0, "symbol check failed")

    rslt = scm_readFromCString(" - ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "-") == 0, "symbol check failed")

    rslt = scm_readFromCString(" --->---<--- ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "--->---<---") == 0, "symbol check failed")

    sym_abcde1 = scm_readFromCString("abcde ");
    sym_abcde2 = scm_readFromCString("abcde ");
    ASSERT(sym_abcde1 == sym_abcde2, "symbol identity check failed")

    sym_bbb1 = scm_readFromCString("   bbb ");
    sym_bbb2 = scm_readFromCString("bbb ");
    ASSERT(sym_bbb1 == sym_bbb2, "symbol identity check failed")

    sym_abcde3 = scm_readFromCString("abcde ");
    ASSERT(sym_abcde3 == sym_abcde1, "symbol identity check failed")

    symx1 = scm_readFromCString("abcde ");
    symx2 = scm_readFromCString(" bbb ");
    ASSERT(symx1 != symx2, "symbol identity check failed")



    //
    // list testing
    //
    SCM_OBJ list_2, list_3;

    rslt = scm_readFromCString("()");
    ASSERT(rslt == SCM_NIL, "empty list read check failed")

    rslt = scm_readFromCString(" (123) ");
    ASSERT(tagOf(rslt) == TAG_CONS, "empty list read check failed")
    ASSERT(isCons(rslt), "empty list read check failed")
    ASSERT(isInteger(rslt->scm_cons.car), "car list read check failed")
    ASSERT(rslt->scm_cons.cdr == SCM_NIL, "cdr list read check failed")
    ASSERT(intValue(car(rslt)) == 123, "inval list read check failed")

    rslt = scm_readFromCString(" (a b c) ");
    ASSERT(tagOf(rslt) == TAG_CONS, "empty list read check failed")
    ASSERT(isCons(rslt), "empty list read check failed")
    ASSERT(isSymbol(car(rslt)), "car list read check failed")
    ASSERT(isCons(cdr(rslt)), "car list read check failed")
    ASSERT(isSymbol(car(cdr(rslt))), "car list read check failed")
    ASSERT(isCons(cdr(cdr(rslt))), "car list read check failed")
    ASSERT(isSymbol(car(cdr(cdr(rslt)))), "car list read check failed")
    ASSERT(isNil(cdr(cdr(cdr(rslt)))), "car list read check failed")

    rslt = scm_readFromCString(" (a (b (c))) ");
    ASSERT(isCons(rslt), "empty list read check failed")
    ASSERT(isSymbol(car(rslt)), "car list read check failed")
    ASSERT(isCons(cdr(rslt)), " read check failed")
    ASSERT(isNil(cdr(cdr(rslt))), " read check failed")
    list_2 = car(cdr(rslt));
    ASSERT(isSymbol(car(list_2)), "read check failed")
    ASSERT(isCons(cdr(list_2)), " read check failed")
    list_3 = car(cdr(list_2));
    ASSERT(isSymbol(car(list_3)), "read check failed")


#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "Done (readerTest)\n");
#endif
}



/*
 * symbolTable testing
 */
static void symbolTableTest() {

    int size = 10000; // init 1000 objects
    char symName[64];
    SCM_OBJ sym1, sym2;
    long start, end, sum;

    start = millisecondTime();

    for (int i = 0; i < size; i++) {
        sprintf(symName, "abc%d", i);
        new_symbol(symName);
    }
    end = millisecondTime();

#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "time to add 10000 new symbols: %dms\n", (int)(end-start));
#endif

    start = millisecondTime();
    for (int i = 0; i < size; i++) {
        sprintf(symName, "abc%d", i);
        new_symbol(symName);
    }
    end = millisecondTime();

#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "time to lookup 10000 existing symbols: %dms\n", (int)(end-start));
#endif

    // check identity
    for (int i = 0; i < size; i++) {
        sprintf(symName, "abc%d", i);
        sym1 = new_symbol(symName);
        sym2 = new_symbol(symName);
        ASSERT(sym1 == sym2, "symbol table check");
    }

#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "Done (symbolTableTest)\n");
#endif

}






/*
 * environment testing
 */
static void environmentTest() {

    SCM_OBJ result;

    add_binding(new_symbol("a"), new_integer(120));
    add_binding(new_symbol("b"), new_integer(200));

    result = get_binding(new_symbol("a"));
    ASSERT(isInteger(result), "getbinding error");
    ASSERT(intValue(result) == 120, "getbinding error");

    result = get_binding(new_symbol("b"));
    ASSERT(isInteger(result), "getbinding error");
    ASSERT(intValue(result) == 200, "getbinding error");

    result = get_binding(new_symbol("c"));
    ASSERT(result == NULL, "getbinding undefined error");

    {
        int i;

        for (i = 0; i < 10000; i++) {
            char name[32];
            SCM_OBJ k;

            sprintf(name, "s%d", i);
            k = new_symbol(name);
            add_binding(k, new_integer(i));
        }
    }

    result = get_binding(new_symbol("s500"));
    ASSERT(intValue(result) == 500, "binding error");

#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "Done (environmentTest)\n");
#endif

}



/*
 * eval testing
 */
static void evalTest() {

    SCM_OBJ result;

    result = scm_eval(scm_readFromCString("0"));
    ASSERT(isInteger(result), "eval const error");
    ASSERT(intValue(result) == 0, "eval const error");

    result = scm_eval(scm_readFromCString("-1"));
    ASSERT(isInteger(result), "eval const error");
    ASSERT(intValue(result) == -1, "eval const error");

    result = scm_eval(scm_readFromCString("#t"));
    ASSERT(isTrue(result), "eval const error");

    result = scm_eval(scm_readFromCString("#f"));
    ASSERT(isFalse(result), "eval const error");

    result = scm_eval(scm_readFromCString("#void"));
    ASSERT(isVoid(result), "eval const error");

    result = scm_eval(scm_readFromCString("nil"));
    ASSERT(isNil(result), "eval const error");

    result = scm_eval(scm_readFromCString("a"));
    ASSERT(isInteger(result), "eval symbol error");
    ASSERT(intValue(result) == 120, "eval symbol error");

#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "Done (evalTest)\n");
#endif
}



/*
 * function testing
 */
static void functionTest() {

    SCM_OBJ result;

    result = scm_eval(scm_readFromCString("(+)"));
    ASSERT(isInteger(result), "eval (+) error");
    ASSERT(intValue(result) == 0, "eval (+) error");

    result = scm_eval(scm_readFromCString("(+ 1)"));
    ASSERT(isInteger(result), "eval (+ 1) error");
    ASSERT(intValue(result) == 1, "eval (+ 1) error");

    result = scm_eval(scm_readFromCString("(+ 1 2)"));
    ASSERT(isInteger(result), "eval (+ 1 2) error");
    ASSERT(intValue(result) == 3, "eval (+ 1 2) error");

    result = scm_eval(scm_readFromCString("(+ 1 2 3 4)"));
    ASSERT(isInteger(result), "eval (+ 1 2 3 4) error");
    ASSERT(intValue(result) == 10, "eval (+ 1 2 3 4) error");

    result = scm_eval(scm_readFromCString("(- 1)"));
    ASSERT(isInteger(result), "eval (- 1) error");
    ASSERT(intValue(result) == -1, "eval (- 1) error");

    result = scm_eval(scm_readFromCString("(- 1 3)"));
    ASSERT(isInteger(result), "eval (- 1 3) error");
    ASSERT(intValue(result) == -2, "eval (- 1 3) error");

    result = scm_eval(scm_readFromCString("(- 1 2 3 4)"));
    ASSERT(isInteger(result), "eval (- 1 2 3 4) error");
    ASSERT(intValue(result) == -8, "eval (- 1 2 3 4) error");

    result = scm_eval(scm_readFromCString("(*)"));
    ASSERT(isInteger(result), "eval (*) error");
    ASSERT(intValue(result) == 1, "eval (*) error");

    result = scm_eval(scm_readFromCString("(* 2)"));
    ASSERT(isInteger(result), "eval (* 2) error");
    ASSERT(intValue(result) == 2, "eval (* 2) error");

    result = scm_eval(scm_readFromCString("(* 2 5)"));
    ASSERT(isInteger(result), "eval (* 2 5) error");
    ASSERT(intValue(result) == 10, "eval (* 2 5) error");

    result = scm_eval(scm_readFromCString("(* 2 5 10 20)"));
    ASSERT(isInteger(result), "eval (* 2 5 10 20) error");
    ASSERT(intValue(result) == 2000, "eval (* 2 5 10 20) error");

    result = scm_eval(scm_readFromCString("(cons 1 2)"));
    ASSERT(isCons(result), "eval (cons 1 2) error");
    ASSERT(isInteger(car(result)), "eval (cons 1 2) error");
    ASSERT(isInteger(cdr(result)), "eval (cons 1 2) error");
    ASSERT(intValue(car(result)) == 1, "eval (cons 1 2) error");
    ASSERT(intValue(cdr(result)) == 2, "eval (cons 1 2) error");

    result = scm_eval(scm_readFromCString("(car (cons 1 2))"));
    ASSERT(isInteger(result), "eval (car (cons 1 2)) error");
    ASSERT(intValue(result) == 1, "eval (car (cons 1 2)) error");

    result = scm_eval(scm_readFromCString("(cdr (cons 1 2))"));
    ASSERT(isInteger(result), "eval (cdr (cons 1 2)) error");
    ASSERT(intValue(result) == 2, "eval (cdr (cons 1 2)) error");

    result = scm_eval(scm_readFromCString("(eq? #t #t)"));
    ASSERT(result == SCM_TRUE, "eval (eq? #t #t) error");

    result = scm_eval(scm_readFromCString("(eq? #t #f)"));
    ASSERT(result == SCM_FALSE, "eval (eq? #t #f) error");

    result = scm_eval(scm_readFromCString("(= 123 123)"));
    ASSERT(result == SCM_TRUE, "eval (= 123 123) error");

    result = scm_eval(scm_readFromCString("(= 123 124)"));
    ASSERT(result == SCM_FALSE, "eval (= 123 124) error");


#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "Done (functionTest)\n");
#endif
}




/*
 * syntax testing
 */
static void syntaxTest() {


    SCM_OBJ result;

    result = scm_eval(scm_readFromCString("(quote (1 2))"));
    ASSERT(isCons(result), "eval (quote (1 2)) error");
    ASSERT(isInteger(car(result)), "eval (quote (1 2)) error");
    ASSERT(isCons(cdr(result)), "eval (quote (1 2)) error");
    ASSERT(isInteger(car(cdr(result))), "eval (quote (1 2)) error");
    ASSERT(isNil(cdr(cdr(result))), "eval (quote (1 2)) error");
    ASSERT(intValue(car(result)) == 1, "eval (quote (1 2)) error");
    ASSERT(intValue(car(cdr(result))) == 2, "eval (quote (1 2)) error");

    result = scm_eval(scm_readFromCString("'(1 2)"));
    ASSERT(isCons(result), "eval '(1 2) error");
    ASSERT(isInteger(car(result)), "eval '(1 2) error");
    ASSERT(isCons(cdr(result)), "eval '(1 2) error");
    ASSERT(isInteger(car(cdr(result))), "eval '(1 2) error");
    ASSERT(isNil(cdr(cdr(result))), "eval '(1 2) error");
    ASSERT(intValue(car(result)) == 1, "eval '(1 2) error");
    ASSERT(intValue(car(cdr(result))) == 2, "eval '(1 2) error");

    result = scm_eval(scm_readFromCString("(define xx 10)"));
    ASSERT(isVoid(result), "eval (define xx 10) error");

    result = scm_eval(scm_readFromCString("xx"));
    ASSERT(isInteger(result), "eval (define xx 10) xx error");
    ASSERT(intValue(result) == 10, "eval (define xx 10) xx error");

    result = scm_eval(scm_readFromCString("(if #t 1 0)"));
    ASSERT(isInteger(result), "eval (if #t 1 0) error");
    ASSERT(intValue(result) == 1, "eval (if #t 1 0)");

    result = scm_eval(scm_readFromCString("(if #f 1 0)"));
    ASSERT(isInteger(result), "eval (if #f 1 0) error");
    ASSERT(intValue(result) == 0, "eval (if #f 1 0)");

#ifdef PRINT_TEST_DETAILS
    fprintf(stderr, "Done (syntaxTest)\n");
#endif
}





// a place to place a breakpoint in the IDE/debugger
void breakpoint() {
    printf("bpnt\n");
}