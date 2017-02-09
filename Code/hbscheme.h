/*
 * Header
 *
 * Author:       Hannes Buchwald
 * Version:      0.0.1
 * Last edit:    09.02.2017
*/


#include <stdio.h>

#define DEBUGGING


#ifdef __WIN32__
typedef int scheme_int32 ;
typedef long long scheme_int64 ;
#else
typedef int scheme_int32;
typedef long scheme_int64;
#endif


#ifdef DEBUGGING
# define ASSERT(expr,msg) { (!(expr)) ? (error(msg, __FILE__, __LINE__),0) : 0; }
#else
# define ASSERT(expr,msg) /* nichts */
#endif

#define TAG(o)      ((o)->u.any.tag)



/* ********************* enum ********************* */
enum tag {
    T_INTEGER,
    T_NIL,
    T_TRUE,
    T_FALSE,
    T_STRING,
    T_SYMBOL,
    T_CONS,
    T_FILESTREAM,
    T_STRINGSTREAM,
    T_BUILTINFUNCTION,
};







/* ********************* structs ********************* */
typedef struct schemeObject *OBJ;

struct schemeFileStream {
    enum tag tag;
    FILE *file;
    char *string;
    int size_t;
    int peekChar;
};


struct schemeObject {
    union {
        struct schemeFileStream fileStream;
    } u;
};



/* ********************* variables ******************* */




/* ********************* method's ********************* */
OBJ newFileStream(FILE*);
OBJ newString();
OBJ scm_read(OBJ);

OBJ scm_readt(OBJ, char*, size_t);

OBJ eval(OBJ);


/* ***** printer method's ******* */
void scm_print(OBJ);
void scm_print_Close();
