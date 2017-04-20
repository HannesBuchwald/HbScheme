#include "hbscheme.h"
#include "printer.h"
#include "memory.h"

#define EOF_CHAR    ((SCM_CHAR)-1)
#define INITIAL_BUFFER_SIZE 32

static
scm_stream
new_stringStream(char* inString) {
    scm_stream s = (scm_stream)malloc(sizeof(struct scm_stream_struct));
    s->type = STRINGSTREAM;
    s->theString = inString;
    s->indexInString = 0;
    return s;
}

scm_stream
new_fileStream(FILE* inFile) {
    scm_stream s = (scm_stream)malloc(sizeof(struct scm_stream_struct));
    s->type = FILESTREAM;
    s->fileStream = inFile;
    s->peekChar = 0;
    return s;
}

typedef struct bufferStruct {
    char *memory;
    int bufferSize;
    int fillCount;
} buffer;

static void
allocBuffer(buffer *b, int initialSize) {
    b->memory = malloc(initialSize);
    b->bufferSize = initialSize;
    b->fillCount = 0;
}

static void
growBuffer(buffer *b) {
    int newSize = b->bufferSize * 2;
    b->memory = realloc(b->memory, newSize);
    b->bufferSize = newSize;
}

static void
putBuffer(buffer *b, SCM_CHAR ch) {
    if ((b->fillCount+1) == b->bufferSize) {
        growBuffer(b);
    }
    b->memory[b->fillCount++] = (char)ch;
    b->memory[b->fillCount] = '\0';
}

static CBOOL
isSeparator(SCM_CHAR aChar) {
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

static CBOOL
allDigits(char *cp) {
    char c;

    if (*cp == '\0') return 0;

    while ((c = *cp++) != '\0') {
        if ((c < '0') || (c > '9')) return 0;
    }
    return 1;
}

static SCM_CHAR
readCharacter(scm_stream inStream) {
    switch (inStream->type) {
        case FILESTREAM:
        {
            SCM_CHAR nextChar;

            if (inStream->peekChar != 0) {
                nextChar = inStream->peekChar;
                inStream->peekChar = 0;
                return nextChar;
            }

            nextChar = fgetc(inStream->fileStream);
            if (nextChar < 0) {
                return EOF_CHAR;
            }
            return nextChar;
        }

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

static void
unreadCharacter(scm_stream inStream, SCM_CHAR charToUnread) {
    switch (inStream->type) {
        case FILESTREAM:
            ASSERT(inStream->peekChar == 0, "can only unread one char");

            inStream->peekChar = charToUnread;
            return;

        case STRINGSTREAM:
            inStream->indexInString--;
            return;

        default:
            FATAL("unhandled stream type");
    }
}

static SCM_CHAR
skipSeparators(scm_stream inStream) {
    SCM_CHAR nextChar;

    do {
        nextChar = readCharacter(inStream);
        if (nextChar == EOF_CHAR) return EOF_CHAR;
    } while (isSeparator(nextChar));
    return nextChar;
}

long
a2l(char *cp) {
    long val = 0;
    char c;

    while ((c = *cp++) != '\0') {
        val = val*10 + (c - '0');
    }
    return val;
}


SCM_OBJ scm_readFromCString(char* input) {
    return scm_read( new_stringStream(input) );
}

static SCM_OBJ
scm_readList(scm_stream inStream) {
    SCM_CHAR nextChar;
    SCM_OBJ theCar, theCdr;

    nextChar = skipSeparators(inStream);
    if (nextChar == ')') {
        return SCM_NIL;
    }
    unreadCharacter(inStream, nextChar);

    theCar = scm_read(inStream);
    theCdr = scm_readList(inStream);
    return new_cons(theCar, theCdr);
}

SCM_OBJ
scm_readString(scm_stream inStream) {
    buffer b;
    SCM_CHAR nextChar;
    char *theString;

    allocBuffer(&b, INITIAL_BUFFER_SIZE);
    for (;;) {
        nextChar = readCharacter(inStream);
        if (nextChar == EOF_CHAR) {
            fprintf(stderr, "unterminated string");
            return SCM_EOF;
        }

        if (nextChar == '"') {
            theString = b.memory;
            return new_string(theString);
        }
        if (nextChar == '\\') {
            SCM_CHAR escapedChar;

            escapedChar = readCharacter(inStream);
            switch (escapedChar) {
                case 'n':
                    nextChar = '\n';
                    break;
                default:
                    break;
            }
        }
        putBuffer(&b, nextChar);
    }
}

SCM_OBJ scm_read(scm_stream inStream) {
    buffer b;
    SCM_CHAR nextChar;
    char *inputToken;

    allocBuffer(&b, INITIAL_BUFFER_SIZE);
    nextChar = skipSeparators(inStream);
    if (nextChar == EOF_CHAR) {
        return SCM_EOF;
    }

    if (nextChar == '(') {
        return scm_readList(inStream);
    }

    if (nextChar == '"') {
        return scm_readString(inStream);
    }
    if (nextChar == '\'') {
        SCM_OBJ quotedExpr;

        quotedExpr = scm_read(inStream);
        return new_cons(SCM_quoteSymbol, new_cons(quotedExpr, SCM_NIL));
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
    unreadCharacter(inStream, nextChar);

    if (allDigits(b.memory)) {
        long iVal = a2l(b.memory);
        return new_integer(iVal);
    }
    inputToken = b.memory;

    if (inputToken[0] == '-') {
        if (allDigits(inputToken+1)) {
            long iVal = a2l(inputToken+1);
            return new_integer(-iVal);
        }
    }

    if (inputToken[0] == '#') {
        if (strcmp(inputToken, "#t") == 0) {
            return SCM_TRUE;
        }
        if (strcmp(inputToken, "#f") == 0) {
            return SCM_FALSE;
        }
        if (strcmp(inputToken, "#void") == 0) {
            return SCM_VOID;
        }
    }
    if (strcmp(inputToken, "nil") == 0) {
        return SCM_NIL;
    }
    return new_symbol(inputToken);
}
