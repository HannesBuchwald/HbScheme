/*
 * constants.h
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    20.04.2017
*/


/*
 * print testdetails
 */
//#define PRINT_TEST_DETAILS;




/******* hbscheme ********/


#define C_TRUE      1
#define C_FALSE     0




/******* Compare ********/


#define FATAL(msg) fatal(msg, __FILE__, __LINE__)

#define ASSERT(cond, msg) ASSERT2(cond, msg, __FILE__, __LINE__)

#define ASSERT2(cond, msg, fileName, lineNr) \
    if (! (cond) ) { \
	fprintf(stderr, "%s:%d assertion failed: %s", \
			fileName, lineNr, msg); \
	abort(); \
    }

#define ASSERT_SYMBOL(obj) \
    ASSERT2(isSymbol(obj), "not a symbol", __FILE__, __LINE__)

#define ASSERT_CONS(obj) \
    ASSERT2(isCons(obj), "not a cons", __FILE__, __LINE__)

#define ASSERT_TAG(obj,t) \
    ASSERT2(hasTag(obj,t), "unexpected tag", __FILE__, __LINE__)





/******* DEBUG ********/

#ifdef DEBUG
# define DBG_PRINT(x) fprintf x
#else
# define DBG_PRINT(x) /* as nothing */
#endif




/******* environment ********/

#define INITIAL_ENVIRONMENT_SIZE    3





/******* eval ********/

#define INITIAL_EVAL_STACK_SIZE     1024






/******* memory ********/

#define INITIAL_SYMBOLTABLE_SIZE    1009
#define USE_REAL_HASH


/******* utilities ********/

#define USE_GETTIMEOFDAY