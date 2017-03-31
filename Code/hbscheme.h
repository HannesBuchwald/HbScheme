/*
 * main.h
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    31.03.2017
*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int CBOOL;
typedef int SCM_CHAR;

typedef union scm_object *SCM_OBJ;

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
};

enum streamType {
    FILESTREAM,
    STRINGSTREAM
};

struct scm_stream_struct {
    enum streamType type;
    FILE *fileStream;
    char *theString;
    int indexInString;
};
typedef struct scm_stream_struct* scm_stream;

#define FATAL(msg) fatal(msg, __FILE__, __LINE__)

#define ASSERT(cond, msg) ASSERT2(cond, msg, __FILE__, __LINE__)

#define ASSERT2(cond, msg, fileName, lineNr) \
    if (! (cond) ) { \
	fprintf(stderr, "%s:%d assertion failed: %s", \
			fileName, lineNr, msg); \
	abort(); \
    }

static inline CBOOL
hasTag(SCM_OBJ obj, scm_tag whichTag) {
    return (obj->scm_any.tag == whichTag);
}

static inline CBOOL
isInteger(SCM_OBJ obj) {
    return hasTag(obj, TAG_INT);
}

static inline CBOOL
isSymbol(SCM_OBJ obj) {
    return hasTag(obj, TAG_SYMBOL);
}

#define ASSERT_SYMBOL(obj) \
    ASSERT2(isSymbol(obj), "not a symbol", __FILE__, __LINE__)






/********** function definitions ***********/



// main functions
extern void fatal(char* msg, char* fileName, int lineNr);


// test functions
extern void unitTest();
extern void performanceTest();


// reader functions
extern SCM_OBJ scm_readString();
extern SCM_OBJ scm_read(scm_stream input);
extern long a2l(char *);


// memory functions
extern SCM_OBJ new_integer();
extern SCM_OBJ new_symbol(char *);
extern SCM_OBJ new_eof(), new_true(), new_false(), new_nil();


// utils functions
extern long millisecondTime();
