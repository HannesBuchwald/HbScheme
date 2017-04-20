//
// Created by admin on 20.04.17.
//

#ifndef HBSCHEME_EVAL_H
#define HBSCHEME_EVAL_H

#endif //HBSCHEME_EVAL_H



/**************** variables & objects & constants *********************/

#define INITIAL_EVAL_STACK_SIZE     1024



/**************** global functions *********************/

void initEvalStack();
void initBuiltinFunctions();
SCM_OBJ scm_eval(SCM_OBJ expr);



/**************** local functions *********************/


void scm_eval2(SCM_OBJ expr);
void scm_evalList(SCM_OBJ expr);

void scm_error(char*, SCM_OBJ);

// push, pop, drop in functions
static inline void PUSH(SCM_OBJ val);
static inline SCM_OBJ POP();
static inline void DROP(int n);



// built in functions
static void builtinPlus(int numArgs);
static void builtinMinus(int numArgs);
static void builtinMultiplication(int numArgs);

static void builtinEq(int numArgs);
static void builtinEqNr(int numArgs);

static void builtinCons(int numArgs);
static void builtinCar(int numArgs);
static void builtinCdr(int numArgs);

static void builtinDisplay(int numArgs);
static void builtinTraceOn(int numArgs);
static void builtinTraceOff(int numArgs);


static void builtinDefine(SCM_OBJ args);
static void builtinLambda(SCM_OBJ args);
static void builtinQuote(SCM_OBJ args);
static void builtinIf(SCM_OBJ args);


