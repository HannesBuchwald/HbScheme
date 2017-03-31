/*
 * reader.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    31.03.2017
*/


#include "hbscheme.h"

#define EOF_CHAR    ((SCM_CHAR)-1)
#define INITIAL_BUFFER_SIZE 32

static scm_stream
new_stringStream(char* inString) {
    scm_stream s = (scm_stream)malloc(sizeof(struct scm_stream_struct));
    s->type = STRINGSTREAM;
    s->theString = inString;
    s->indexInString = 0;
    return s;
}

typedef struct bufferStruct {
    char *memory;
    int bufferSize;
    int fillCount;
} buffer;

static void allocBuffer(buffer *b, int initialSize) {
    b->memory = malloc(initialSize);
    b->bufferSize = initialSize;
    b->fillCount = 0;
}

static void growBuffer(buffer *b) {
    int newSize = b->bufferSize * 2;
    b->memory = realloc(b->memory, newSize);
    b->bufferSize = newSize;
}

static void putBuffer(buffer *b, SCM_CHAR ch) {
    if ((b->fillCount+1) == b->bufferSize) {
	growBuffer(b);
    }
    b->memory[b->fillCount++] = (char)ch;
    b->memory[b->fillCount] = '\0';
}

static CBOOL isSeparator(SCM_CHAR aChar) {
    switch (aChar) {
	case ' ':
	case '\t':
	case '\r':
	case '\n':
	    return 1;
	default:
	    return 0;
    }
}

static CBOOL allDigits(char *cp) {
    char c;

    while ((c = *cp++) != '\0') {
	if ((c < '0') || (c > '9')) return 0;
    }
    return 1;
}

static SCM_CHAR readCharacter(scm_stream inStream) {
    switch (inStream->type) {

    case FILESTREAM:
	    FATAL("unhandled stream type");
	    return 0;

    case STRINGSTREAM:
	    {
		int index = inStream->indexInString;
		SCM_CHAR nextChar = inStream->theString[index];
		if (nextChar == '\0') {
		    return EOF_CHAR;
		}
		inStream->indexInString = ++index;
		return nextChar;
	    }

	default:
	    FATAL("unhandled stream type");
    }
    // only to make the compiler happy
    return 0;
}

static SCM_CHAR skipSeparators(scm_stream inStream) {
    SCM_CHAR nextChar;

    do {
	nextChar = readCharacter(inStream);
	if (nextChar == EOF_CHAR) return EOF_CHAR;
    } while (isSeparator(nextChar));
    return nextChar;
}

long a2l(char *cp) {
    long val = 0;
    char c;

    while ((c = *cp++) != '\0') {
	val = val*10 + (c - '0');
    }
    return val;
}


SCM_OBJ scm_readString(char* input) {
    return scm_read( new_stringStream(input) );
}

SCM_OBJ scm_read(scm_stream inStream) {
    buffer b;
    SCM_CHAR nextChar;
    char *inputToken;

    allocBuffer(&b, INITIAL_BUFFER_SIZE);
    nextChar = skipSeparators(inStream);
    if (nextChar == EOF_CHAR) {
	return new_eof();
    }

    for (;;) {
	if (nextChar == EOF_CHAR)
	    break;
	if (isSeparator(nextChar)
	    || (nextChar == '(')
	    || (nextChar == ')')) {
	    break;
	}
	putBuffer(&b, nextChar);
	nextChar = readCharacter(inStream);
    }

    if (allDigits(b.memory)) {
	long iVal = a2l(b.memory);
	return new_integer(iVal);
    }
    inputToken = b.memory;

    if ((inputToken[0] == '-') && allDigits(inputToken+1)) {
	long iVal = a2l(inputToken+1);
	return new_integer(-iVal);
    }

    if (inputToken[0] == '#') {
	if (strcmp(inputToken, "#t") == 0) {
	    return new_true();
	}
	if (strcmp(inputToken, "#f") == 0) {
	    return new_false();
	}
    }
    if (strcmp(inputToken, "nil") == 0) {
	return new_nil();
    }
    return new_symbol(inputToken);
}
