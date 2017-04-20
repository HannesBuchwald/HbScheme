#include "hbscheme.h"

// #define PRINT_TIMES

// a place to place a breakpoint in the IDE/debugger
void
breakpoint() {
    printf("bpnt\n");
}

void
scm_selftest() {
    SCM_OBJ rslt,
            sym_abcde1, sym_abcde2, sym_abcde3,
            sym_bbb1, sym_bbb2, symx1, symx2,
            l2, l3;

    // check string to integer conversion

    ASSERT( a2l("0") == 0, "a2l error");
    ASSERT( a2l("1") == 1, "a2l error");
    ASSERT( a2l("123456") == 123456, "a2l error");
    ASSERT( a2l("999999") == 999999, "a2l error");
    ASSERT( a2l("2432902008176640000") == 2432902008176640000, "a2l error");

    // -------
    // constant objects (singletons)
    // -------

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

    // -------
    // reader checks
    // -------

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
    ASSERT( strcmp(rslt->scm_string.chars, "hello")==0, "string check failed")

    rslt = scm_readFromCString(" \"hel\\nlo\" ");
    ASSERT(isString(rslt), "string tag check failed")
    ASSERT( strcmp(rslt->scm_string.chars, "hel\nlo")==0, "string check failed")

    // -------

    rslt = scm_readFromCString(" #t ");
    ASSERT(rslt == SCM_TRUE, "true tag check failed")

    rslt = scm_readFromCString(" #f ");
    ASSERT(rslt == SCM_FALSE, "false tag check failed")

    rslt = scm_readFromCString(" nil ");
    ASSERT(rslt == SCM_NIL, "nil tag check failed")

    rslt = scm_readFromCString(" #void ");
    ASSERT(rslt == SCM_VOID, "void tag check failed")

    rslt = scm_readFromCString(" abc ");
    ASSERT(rslt->scm_any.tag == TAG_SYMBOL, "symbol tag check failed")
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(rslt->scm_symbol.chars, "abc")==0, "symbol check failed")
    ASSERT(strcmp(symbolValue(rslt), "abc")==0, "symbol check failed")

    rslt = scm_readFromCString(" if ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "if")==0, "symbol check failed")

    rslt = scm_readFromCString(" define ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "define")==0, "symbol check failed")

    rslt = scm_readFromCString(" 1+ ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "1+")==0, "symbol check failed")

    rslt = scm_readFromCString(" + ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "+")==0, "symbol check failed")

    rslt = scm_readFromCString(" - ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "-")==0, "symbol check failed")

    rslt = scm_readFromCString(" --->---<--- ");
    ASSERT(isSymbol(rslt), "symbol tag check failed")
    ASSERT(strcmp(symbolValue(rslt), "--->---<---")==0, "symbol check failed")

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

    // -------
    // symbol table timing
    // -------

    // create 10000 symbols...
    {
        int i;
        char symName[32];
        SCM_OBJ sym1, sym2;
        long tStart, tEnd;

        tStart = millisecondTime();

        for (i=0; i<10000; i++) {
            sprintf(symName, "abc%d", i);
            new_symbol(symName);
        }
        tEnd = millisecondTime();
#ifdef PRINT_TIMES
        fprintf(stderr, "time to add 10000 new symbols: %dms\n", (int)(tEnd-tStart));
#endif
        tStart = millisecondTime();
        for (i=0; i<10000; i++) {
            sprintf(symName, "abc%d", i);
            new_symbol(symName);
        }
        tEnd = millisecondTime();
#ifdef PRINT_TIMES
        fprintf(stderr, "time to lookup 10000 existing symbols: %dms\n", (int)(tEnd-tStart));
#endif
        // check identity
        for (i=0; i<10000; i++) {
            sprintf(symName, "abc%d", i);
            sym1 = new_symbol(symName);
            sym2 = new_symbol(symName);
            ASSERT(sym1==sym2, "symbol table check");
        }
    }

    // -------
    // more reader tests
    // -------

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
    l2 = car(cdr(rslt));
    ASSERT(isSymbol(car(l2)), "read check failed")
    ASSERT(isCons(cdr(l2)), " read check failed")
    l3 = car(cdr(l2));
    ASSERT(isSymbol(car(l3)), "read check failed")

    // -------
    // environment tests
    // -------

    add_binding(new_symbol("a"), new_integer(120));
    add_binding(new_symbol("b"), new_integer(200));

    rslt = get_binding( new_symbol("a") );
    ASSERT(isInteger(rslt), "getbinding error");
    ASSERT(intValue(rslt) == 120, "getbinding error");

    rslt = get_binding( new_symbol("b") );
    ASSERT(isInteger(rslt), "getbinding error");
    ASSERT(intValue(rslt) == 200, "getbinding error");

    rslt = get_binding( new_symbol("c") );
    ASSERT( rslt == NULL, "getbinding undefined error");

    {
        int i;

        for (i=0; i<10000; i++) {
            char name[32];
            SCM_OBJ k;

            sprintf(name, "s%d", i);
            k = new_symbol(name);
            add_binding(k, new_integer(i));
        }
    }

    rslt = get_binding( new_symbol("s500") );
    ASSERT( intValue(rslt) == 500, "binding error");

    // -------
    // eval tests
    // -------

    // simple constants and symbols

    rslt = scm_eval( scm_readFromCString("0") );
    ASSERT( isInteger(rslt), "eval const error");
    ASSERT( intValue(rslt) == 0, "eval const error");

    rslt = scm_eval( scm_readFromCString("-1") );
    ASSERT( isInteger(rslt), "eval const error");
    ASSERT( intValue(rslt) == -1, "eval const error");

    rslt = scm_eval( scm_readFromCString("#t") );
    ASSERT( isTrue(rslt), "eval const error");

    rslt = scm_eval( scm_readFromCString("#f") );
    ASSERT( isFalse(rslt), "eval const error");

    rslt = scm_eval( scm_readFromCString("#void") );
    ASSERT( isVoid(rslt), "eval const error");

    rslt = scm_eval( scm_readFromCString("nil") );
    ASSERT( isNil(rslt), "eval const error");

    rslt = scm_eval( scm_readFromCString("a") );
    ASSERT( isInteger(rslt), "eval symbol error");
    ASSERT( intValue(rslt) == 120, "eval symbol error");

    // calling functions

    rslt = scm_eval( scm_readFromCString("(+)") );
    ASSERT( isInteger(rslt), "eval (+) error");
    ASSERT( intValue(rslt) == 0, "eval (+) error");

    rslt = scm_eval( scm_readFromCString("(+ 1)") );
    ASSERT( isInteger(rslt), "eval (+ 1) error");
    ASSERT( intValue(rslt) == 1, "eval (+ 1) error");

    rslt = scm_eval( scm_readFromCString("(+ 1 2)") );
    ASSERT( isInteger(rslt), "eval (+ 1 2) error");
    ASSERT( intValue(rslt) == 3, "eval (+ 1 2) error");

    rslt = scm_eval( scm_readFromCString("(+ 1 2 3 4)") );
    ASSERT( isInteger(rslt), "eval (+ 1 2 3 4) error");
    ASSERT( intValue(rslt) == 10, "eval (+ 1 2 3 4) error");

    rslt = scm_eval( scm_readFromCString("(- 1)") );
    ASSERT( isInteger(rslt), "eval (- 1) error");
    ASSERT( intValue(rslt) == -1, "eval (- 1) error");

    rslt = scm_eval( scm_readFromCString("(- 1 3)") );
    ASSERT( isInteger(rslt), "eval (- 1 3) error");
    ASSERT( intValue(rslt) == -2, "eval (- 1 3) error");

    rslt = scm_eval( scm_readFromCString("(- 1 2 3 4)") );
    ASSERT( isInteger(rslt), "eval (- 1 2 3 4) error");
    ASSERT( intValue(rslt) == -8, "eval (- 1 2 3 4) error");

    rslt = scm_eval( scm_readFromCString("(*)") );
    ASSERT( isInteger(rslt), "eval (*) error");
    ASSERT( intValue(rslt) == 1, "eval (*) error");

    rslt = scm_eval( scm_readFromCString("(* 2)") );
    ASSERT( isInteger(rslt), "eval (* 2) error");
    ASSERT( intValue(rslt) == 2, "eval (* 2) error");

    rslt = scm_eval( scm_readFromCString("(* 2 5)") );
    ASSERT( isInteger(rslt), "eval (* 2 5) error");
    ASSERT( intValue(rslt) == 10, "eval (* 2 5) error");

    rslt = scm_eval( scm_readFromCString("(* 2 5 10 20)") );
    ASSERT( isInteger(rslt), "eval (* 2 5 10 20) error");
    ASSERT( intValue(rslt) == 2000, "eval (* 2 5 10 20) error");

    rslt = scm_eval( scm_readFromCString("(cons 1 2)") );
    ASSERT( isCons(rslt), "eval (cons 1 2) error");
    ASSERT( isInteger(car(rslt)), "eval (cons 1 2) error");
    ASSERT( isInteger(cdr(rslt)), "eval (cons 1 2) error");
    ASSERT( intValue(car(rslt)) == 1, "eval (cons 1 2) error");
    ASSERT( intValue(cdr(rslt)) == 2, "eval (cons 1 2) error");

    rslt = scm_eval( scm_readFromCString("(car (cons 1 2))") );
    ASSERT( isInteger(rslt), "eval (car (cons 1 2)) error");
    ASSERT( intValue(rslt) == 1, "eval (car (cons 1 2)) error");

    rslt = scm_eval( scm_readFromCString("(cdr (cons 1 2))") );
    ASSERT( isInteger(rslt), "eval (cdr (cons 1 2)) error");
    ASSERT( intValue(rslt) == 2, "eval (cdr (cons 1 2)) error");

    rslt = scm_eval( scm_readFromCString("(eq? #t #t)") );
    ASSERT( rslt == SCM_TRUE, "eval (eq? #t #t) error");

    rslt = scm_eval( scm_readFromCString("(eq? #t #f)") );
    ASSERT( rslt == SCM_FALSE, "eval (eq? #t #f) error");

    rslt = scm_eval( scm_readFromCString("(= 123 123)") );
    ASSERT( rslt == SCM_TRUE, "eval (= 123 123) error");

    rslt = scm_eval( scm_readFromCString("(= 123 124)") );
    ASSERT( rslt == SCM_FALSE, "eval (= 123 124) error");

    // builtin syntax

    rslt = scm_eval( scm_readFromCString("(quote (1 2))") );
    ASSERT( isCons(rslt), "eval (quote (1 2)) error");
    ASSERT( isInteger(car(rslt)), "eval (quote (1 2)) error");
    ASSERT( isCons(cdr(rslt)), "eval (quote (1 2)) error");
    ASSERT( isInteger(car(cdr(rslt))), "eval (quote (1 2)) error");
    ASSERT( isNil(cdr(cdr(rslt))), "eval (quote (1 2)) error");
    ASSERT( intValue(car(rslt)) == 1, "eval (quote (1 2)) error");
    ASSERT( intValue(car(cdr(rslt))) == 2, "eval (quote (1 2)) error");

    rslt = scm_eval( scm_readFromCString("'(1 2)") );
    ASSERT( isCons(rslt), "eval '(1 2) error");
    ASSERT( isInteger(car(rslt)), "eval '(1 2) error");
    ASSERT( isCons(cdr(rslt)), "eval '(1 2) error");
    ASSERT( isInteger(car(cdr(rslt))), "eval '(1 2) error");
    ASSERT( isNil(cdr(cdr(rslt))), "eval '(1 2) error");
    ASSERT( intValue(car(rslt)) == 1, "eval '(1 2) error");
    ASSERT( intValue(car(cdr(rslt))) == 2, "eval '(1 2) error");

    rslt = scm_eval( scm_readFromCString("(define xx 10)") );
    ASSERT( isVoid(rslt), "eval (define xx 10) error");

    rslt = scm_eval( scm_readFromCString("xx") );
    ASSERT( isInteger(rslt), "eval (define xx 10) xx error");
    ASSERT( intValue(rslt) == 10, "eval (define xx 10) xx error");

    rslt = scm_eval( scm_readFromCString("(if #t 1 0)") );
    ASSERT( isInteger(rslt), "eval (if #t 1 0) error");
    ASSERT( intValue(rslt) == 1, "eval (if #t 1 0)");

    rslt = scm_eval( scm_readFromCString("(if #f 1 0)") );
    ASSERT( isInteger(rslt), "eval (if #f 1 0) error");
    ASSERT( intValue(rslt) == 0, "eval (if #f 1 0)");

    fprintf(stderr, "Done (selftest)\n");
}
