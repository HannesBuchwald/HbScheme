/*
 * hbscheme.h
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    20.04.2017
*/



/**************** includes *********************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"






/**************** type definition *********************/


typedef int CBOOL;
typedef int SCM_CHAR;
typedef union scm_object *SCM_OBJ;
typedef SCM_OBJ (*SCM_OBJFUNC)();
// typedef int (*INTFUNC)();
typedef void (*VOIDFUNC)();
// typedef void* (*VOIDPTRFUNC)();

enum scm_tag_enum {
    TAG_INT = 0,
    TAG_FLOAT,
    TAG_SYMBOL,
    TAG_STRING,
    TAG_CONS,
    TAG_NIL,
    TAG_TRUE,
    TAG_FALSE,
    TAG_EOF,
    TAG_VOID,
    TAG_BUILTINFUNC,
    TAG_BUILTINSYNTAX,
    TAG_USERDEFINEDFUNC,
    MAX_TAG
};

typedef enum scm_tag_enum scm_tag;

struct scm_any {
    scm_tag tag;
};

struct scm_integer {
    scm_tag tag;
    long iVal;
};

struct scm_float {
    scm_tag tag;
    double fVal;
};

struct scm_builtinFunc {
    scm_tag tag;
    VOIDFUNC code;
};

struct scm_builtinSyntax {
    scm_tag tag;
    VOIDFUNC code;
};

struct scm_userDefinedFunc {
    scm_tag tag;
    SCM_OBJ argList;
    SCM_OBJ bodyList;
};

struct scm_special {
    scm_tag tag;
};

struct scm_symbol {
    scm_tag tag;
    char chars[1];
};

struct scm_string {
    scm_tag tag;
    char chars[1];
};

struct scm_string10 {
    scm_tag tag;
    char chars[10];
};

struct scm_cons {
    scm_tag tag;
    SCM_OBJ car;
    SCM_OBJ cdr;
};

union scm_object {
    struct scm_any scm_any;
    struct scm_integer scm_integer;
    struct scm_float scm_float;
    struct scm_special scm_special;
    struct scm_symbol scm_symbol;
    struct scm_string scm_string;
    struct scm_string10 scm_string10;
    struct scm_cons scm_cons;
    struct scm_builtinFunc scm_builtinFunc;
    struct scm_builtinSyntax scm_builtinSyntax;
    struct scm_userDefinedFunc scm_userDefinedFunc;
};








//
// wellknown objects
//
extern SCM_OBJ SCM_NIL;
extern SCM_OBJ SCM_TRUE;
extern SCM_OBJ SCM_FALSE;
extern SCM_OBJ SCM_VOID;
extern SCM_OBJ SCM_EOF;
extern SCM_OBJ SCM_quoteSymbol;

//
// globals
//
extern CBOOL trace_flag;



//
// useful inline functions
//
static inline scm_tag  tagOf(SCM_OBJ obj) {
    return (obj->scm_any.tag);
}

static inline CBOOL hasTag(SCM_OBJ obj, scm_tag whichTag) {
    return (tagOf(obj) == whichTag);
}

static inline CBOOL isInteger(SCM_OBJ obj) {
    return hasTag(obj, TAG_INT);
}

static inline CBOOL isFloat(SCM_OBJ obj) {
    return hasTag(obj, TAG_FLOAT);
}

static inline CBOOL isNil(SCM_OBJ obj) {
    return obj == SCM_NIL;      // hasTag(obj, TAG_NIL);
}

static inline CBOOL isVoid(SCM_OBJ obj) {
    return obj == SCM_VOID;     // hasTag(obj, TAG_VOID);
}

static inline CBOOL isTrue(SCM_OBJ obj) {
    return obj == SCM_TRUE;     // hasTag(obj, TAG_TRUE);
}

static inline CBOOL isFalse(SCM_OBJ obj) {
    return obj == SCM_FALSE;    // hasTag(obj, TAG_FALSE);
}

static inline CBOOL isSymbol(SCM_OBJ obj) {
    return hasTag(obj, TAG_SYMBOL);
}

static inline CBOOL isString(SCM_OBJ obj) {
    return hasTag(obj, TAG_STRING);
}

static inline CBOOL isCons(SCM_OBJ obj) {
    return hasTag(obj, TAG_CONS);
}

static inline long intValue(SCM_OBJ obj) {
    ASSERT(isInteger(obj), "not an integer");
    return (obj->scm_integer.iVal);
}

static inline double floatValue(SCM_OBJ obj) {
    ASSERT(isFloat(obj), "not a a float");
    return (obj->scm_float.fVal);
}

static inline char *symbolValue(SCM_OBJ obj) {
    ASSERT(isSymbol(obj), "not a symbol");
    return (obj->scm_symbol.chars);
}

static inline char*stringValue(SCM_OBJ obj) {
    ASSERT(isString(obj), "not a string");
    return (obj->scm_string.chars);
}

static inline SCM_OBJ car(SCM_OBJ obj) {
    ASSERT(isCons(obj), "not a cons cell");
    return (obj->scm_cons.car);
}

static inline SCM_OBJ cdr(SCM_OBJ obj) {
    ASSERT(isCons(obj), "not a cons cell");
    return (obj->scm_cons.cdr);
}







/**************** global functions *********************/


void backToRepl();




/**************** local functions *********************/

void init();


