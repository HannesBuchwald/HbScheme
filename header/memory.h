/*
 * memory.h
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    21.04.2017
*/


/**************** global functions *********************/

void initSymbolTable();



// all possible symbols
SCM_OBJ new_nil();
SCM_OBJ new_false();
SCM_OBJ new_true();

SCM_OBJ new_eof();
SCM_OBJ new_void();

SCM_OBJ new_symbol(char *);
SCM_OBJ new_integer(int iVal);
SCM_OBJ new_string(char *);
SCM_OBJ new_cons(SCM_OBJ theCar, SCM_OBJ theCdr);


SCM_OBJ new_builtinFunc(VOIDFUNC);
SCM_OBJ new_builtinSyntax(VOIDFUNC);
SCM_OBJ new_userDefinedFunction(SCM_OBJ, SCM_OBJ);



/**************** local functions *********************/


static SCM_OBJ new_singleton(scm_tag tag);
static SCM_OBJ really_a_new_symbol(char* chars);
static SCM_OBJ getSymbol(char *chars);
static inline long haschSize(char *cp);
static void remember_symbol(SCM_OBJ theSymbolToRemember);
static void possiblyGrowSymbolTable();