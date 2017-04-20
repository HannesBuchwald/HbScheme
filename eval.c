//
// Created by admin on 20.04.17.
//

#include "hbscheme.h"

int trace_flag = C_FALSE;
static int callDepth = 0;

#define INITIAL_EVAL_STACK_SIZE     1024

static SCM_OBJ* evalStack = NULL;
static SCM_OBJ* evalSP = NULL;
static SCM_OBJ* evalStackTop = NULL;

static inline void
PUSH(SCM_OBJ val) {
    *evalSP++ = val;
    if (evalSP >= evalStackTop) {
	FATAL("stack overflow");
    }
}

static inline SCM_OBJ
POP() {
    if (evalSP <= evalStack) {
	FATAL("stack underflow");
    }
    return *--evalSP;
}

static inline void
DROP(int n) {
    evalSP -= n;
    if (evalSP < evalStack) {
	FATAL("stack underflow");
    }
}

void
initializeEvalStack() {
    evalStack = (SCM_OBJ*)malloc(sizeof(SCM_OBJ) * INITIAL_EVAL_STACK_SIZE);
    evalSP = evalStack;
    evalStackTop = &(evalStack[INITIAL_EVAL_STACK_SIZE]);
}

//
// functions
//
static void
builtinCons(int numArgs) {
    SCM_OBJ evaluatedCar, evaluatedCdr;
    SCM_OBJ newCons;

    if (numArgs != 2) {
	scm_error("(cons): two arguments expected", NULL);
    }
    evaluatedCdr = POP();
    evaluatedCar = POP();

    newCons = new_cons(evaluatedCar, evaluatedCdr);
    PUSH (newCons);
}

static void
builtinCar(int numArgs) {
    SCM_OBJ evaluatedArg;

    if (numArgs != 1) {
	scm_error("(car): one arguments expected", NULL);
    }
    evaluatedArg = POP();
    PUSH( car(evaluatedArg) );
}

static void
builtinCdr(int numArgs) {
    SCM_OBJ evaluatedArg;

    if (numArgs != 1) {
	scm_error("(cdr): one arguments expected", NULL);
    }
    evaluatedArg = POP();
    PUSH( cdr(evaluatedArg) );
}

static void
builtinPlus(int numArgs) {
    int sum = 0;

    while (--numArgs >= 0) {
	SCM_OBJ valueOfNextArg;

	valueOfNextArg = POP();
	if (isInteger(valueOfNextArg)) {
	    sum = sum + intValue(valueOfNextArg);
	} else {
	    scm_error("(+): non numeric argument: ",valueOfNextArg);
	}
    }
    PUSH( new_integer(sum) );
}

static void
builtinTimes(int numArgs) {
    int product = 1;

    while (--numArgs >= 0) {
	SCM_OBJ valueOfNextArg;

	valueOfNextArg = POP();
	if (isInteger(valueOfNextArg)) {
	    product = product * intValue(valueOfNextArg);
	} else {
	    scm_error("(*): non numeric argument: ",valueOfNextArg);
	}
    }
    PUSH( new_integer(product) );
}

static void
builtinMinus(int numArgs) {
    int diff = 0;
    SCM_OBJ valueOfNextArg;

    if (numArgs == 0) {
	scm_error("(-): at least one argument expected",NULL);
    }

    valueOfNextArg = POP();
    if (!isInteger(valueOfNextArg)) {
	scm_error("(-): non numeric argument: ",valueOfNextArg);
    }
    diff = intValue(valueOfNextArg);
    diff = -diff;

    if (numArgs == 1) {
	PUSH( new_integer( diff ) );
	return;
    }

    while (--numArgs > 1) {
	valueOfNextArg = POP();

	if (isInteger(valueOfNextArg)) {
	    diff = diff - intValue(valueOfNextArg);
	} else {
	    scm_error("(-): non numeric argument: ",valueOfNextArg);
	}
    }

    // the first arg: positive!
    valueOfNextArg = POP();
    if (isInteger(valueOfNextArg)) {
	diff = diff + intValue(valueOfNextArg);
    } else {
	scm_error("(-): non numeric argument: ",valueOfNextArg);
    }
    PUSH (new_integer(diff));
}

static void
builtinEq(int numArgs) {
    if (numArgs != 2) {
	scm_error("(eq?): two arguments expected", NULL);
    }
    PUSH ((POP() == POP()) ? SCM_TRUE : SCM_FALSE);
}

static void
builtinEqNr(int numArgs) {
    SCM_OBJ arg1, arg2;

    if (numArgs != 2) {
	scm_error("(=): two arguments expected", NULL);
    }
    arg2 = POP();
    arg1 = POP();
    if (! isInteger(arg1)) {
	scm_error("(=): numeric argument expected: ", arg1);
    }
    if (arg1 == arg2) {
	PUSH (SCM_TRUE);
	return;
    }
    if (! isInteger(arg2)) {
	scm_error("(=): numeric argument expected: ", arg2);
    }
    PUSH ((intValue(arg1) == intValue(arg2)) ? SCM_TRUE : SCM_FALSE);
}

static void
builtinDisplay(int numArgs) {
    while (--numArgs >= 0) {
	SCM_OBJ evaluatedArg;

	evaluatedArg = POP();
	scm_print(stdout, evaluatedArg, PRINT_DISPLAY);
    }
    PUSH( SCM_VOID );
}

static void
builtinTraceOn(int numArgs) {
    DROP(numArgs);
    trace_flag = C_TRUE;
    PUSH( SCM_VOID );
}

static void
builtinTraceOff(int numArgs) {
    DROP(numArgs);
    trace_flag = C_FALSE;
    PUSH( SCM_VOID );
}


//
// syntax
//
static void
builtinDefine(SCM_OBJ args) {
    SCM_OBJ theUnevaluatedNameArg, theUnevaluatedExprArg;
    SCM_OBJ evaluatedExpr;

    if (isNil(args)) {
	scm_error("(define): two arguments expected", NULL);
    }
    theUnevaluatedNameArg = car(args);

    args = cdr(args);
    if (isNil(args)) {
	scm_error("(define): two arguments expected", NULL);
    }
    theUnevaluatedExprArg = car(args);

    if (! isNil(cdr(args))) {
	scm_error("(define): more than two arguments given", NULL);
    }

    if (! isSymbol(theUnevaluatedNameArg)) {
	scm_error("(define): non-symbol argument: ", theUnevaluatedNameArg);
    }
    evaluatedExpr = scm_eval(theUnevaluatedExprArg);

    add_binding(theUnevaluatedNameArg, evaluatedExpr);
    PUSH( SCM_VOID );
}

static void
builtinLambda(SCM_OBJ args) {
    SCM_OBJ theUnevaluatedArglist, theUnevaluatedBodyList;

    if (isNil(args)) {
	scm_error("(lambda): at least two arguments expected", NULL);
    }
    theUnevaluatedArglist = car(args);

    args = cdr(args);
    if (isNil(args)) {
	scm_error("(lambda): at least two arguments expected", NULL);
    }
    theUnevaluatedBodyList = args;

    PUSH( new_userDefinedFunction(theUnevaluatedArglist, theUnevaluatedBodyList) );
}

static void
builtinQuote(SCM_OBJ args) {
    SCM_OBJ theUnevaluatedArg;

    if (isNil(args)) {
	scm_error("(quote): one argument expected", NULL);
    }
    theUnevaluatedArg = car(args);

    if (! isNil(cdr(args))) {
	scm_error("(quote): more than one argument given", NULL);
    }

    PUSH( theUnevaluatedArg );
}

static void
builtinIf(SCM_OBJ args) {
    SCM_OBJ theUnevaluatedCondition, theUnevaluatedIfExpr, theUnevaluatedElseExpr;
    SCM_OBJ evaluatedCondition;
    SCM_OBJ val;

    if (isNil(args)) {
	scm_error("(if): three arguments expected", NULL);
    }
    theUnevaluatedCondition = car(args);

    args = cdr(args);
    if (isNil(args)) {
	scm_error("(if): three arguments expected", NULL);
    }
    theUnevaluatedIfExpr = car(args);

    args = cdr(args);
    if (isNil(args)) {
	scm_error("(if): three arguments expected", NULL);
    }
    theUnevaluatedElseExpr = car(args);

    if (! isNil(cdr(args))) {
	scm_error("(if): more than three arguments given", NULL);
    }

    evaluatedCondition = scm_eval(theUnevaluatedCondition);

    if (isTrue(evaluatedCondition)) {
	val = scm_eval(theUnevaluatedIfExpr);
    } else {
	val = scm_eval(theUnevaluatedElseExpr);
    }
    PUSH(val);
}

//
// evaluator
//
void
scm_evalList(SCM_OBJ expr) {
    SCM_OBJ unevaluatedFunc = car(expr);
    SCM_OBJ unevaluatedArgs = cdr(expr);
    SCM_OBJ func;

    func = scm_eval(unevaluatedFunc);
    switch (tagOf(func)) {
	case TAG_BUILTINFUNC:
	    {
		int numArgs = 0;

		while (! isNil(unevaluatedArgs)) {
		    SCM_OBJ nextUnevaluatedArg = car(unevaluatedArgs);

		    unevaluatedArgs = cdr(unevaluatedArgs);
		    PUSH( scm_eval(nextUnevaluatedArg) );
		    numArgs++;
		}
		(*func->scm_builtinFunc.code)(numArgs);
		return;
	    }

	case TAG_BUILTINSYNTAX:
	    (*func->scm_builtinSyntax.code)(unevaluatedArgs);
	    return;

	default:
	    scm_error("non function in function slot: ", func);
    }
}

void
scm_eval2(SCM_OBJ expr) {
    SCM_OBJ val;

    if (trace_flag) {
	int i;

	for (i=0; i<callDepth; i++) fprintf(stderr, "  ");
	fprintf(stderr, ">>>> "); scm_print(stderr, expr, PRINT_WRITE); fprintf(stderr, "\n");
    }

    switch (tagOf(expr)) {
	case TAG_SYMBOL:
	    val = get_binding(expr);
	    if (val == NULL) {
		scm_error("no binding for: ", expr);
	    }
	    PUSH( val );
	    return;

	case TAG_CONS:
	    callDepth++;
	    scm_evalList(expr);
	    callDepth--;
	    return;

	default:
	    PUSH( expr );
	    return;
    }
}

SCM_OBJ
scm_eval(SCM_OBJ expr) {
    scm_eval2(expr);
    return POP();
}

void
initializeBuiltinFunctions() {
    SCM_OBJ fn;

    fn = new_builtinFunc(builtinPlus);
    add_binding(new_symbol("+"), fn);
    add_binding(new_symbol("plus"), fn);

    fn = new_builtinFunc(builtinTimes);
    add_binding(new_symbol("*"), fn);

    fn = new_builtinFunc(builtinMinus);
    add_binding(new_symbol("-"), fn);
    add_binding(new_symbol("minus"), fn);

    // ----------

    fn = new_builtinFunc(builtinCons);
    add_binding(new_symbol("cons"), fn);

    fn = new_builtinFunc(builtinCar);
    add_binding(new_symbol("car"), fn);
    fn = new_builtinFunc(builtinCdr);
    add_binding(new_symbol("cdr"), fn);

    fn = new_builtinFunc(builtinEq);
    add_binding(new_symbol("eq?"), fn);

    fn = new_builtinFunc(builtinEqNr);
    add_binding(new_symbol("="), fn);

    fn = new_builtinFunc(builtinDisplay);
    add_binding(new_symbol("display"), fn);

    fn = new_builtinFunc(builtinTraceOn);
    add_binding(new_symbol("traceOn"), fn);
    fn = new_builtinFunc(builtinTraceOff);
    add_binding(new_symbol("traceOff"), fn);

    // ----------

    fn = new_builtinSyntax(builtinDefine);
    add_binding(new_symbol("define"), fn);

    fn = new_builtinSyntax(builtinIf);
    add_binding(new_symbol("if"), fn);

    fn = new_builtinSyntax(builtinQuote);
    add_binding(new_symbol("quote"), fn);

    fn = new_builtinSyntax(builtinLambda);
    add_binding(new_symbol("lambda"), fn);
}
