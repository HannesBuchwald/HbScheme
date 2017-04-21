/*
 * reader.h
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    20.04.2017
 *
 */




/****** definitions ************/

#ifndef HBSCHEME_READER_H
#define HBSCHEME_READER_H

#endif //HBSCHEME_READER_H



#define EOF_CHAR    ((SCM_CHAR)-1)
#define INITIAL_BUFFER_SIZE 32





/****** enums ************/


enum streamType {
    FILESTREAM,
    STRINGSTREAM
};





/****** structs ************/


struct scm_stream_struct {
    enum streamType type;
    FILE *fileStream;
    char *theString;
    int indexInString;
    int peekChar;
};



/****** type definition ************/


typedef struct scm_stream_struct* scm_stream;


typedef struct bufferStruct {
    char *memory;
    int bufferSize;
    int fillCount;
} buffer;






/****** global functions ************/


scm_stream new_fileStream(FILE*);

SCM_OBJ scm_readFromCString(char* input);
SCM_OBJ scm_readString(scm_stream inStream);
SCM_OBJ scm_read(scm_stream input);
long a2l(char *cp);





/****** local functions ************/

static scm_stream new_stringStream(char* inString);
static SCM_OBJ scm_readList(scm_stream inStream);

static void allocBuffer(buffer *b, int initialSize);
static void growBuffer(buffer *b);
static void putBuffer(buffer *b, SCM_CHAR ch);

static CBOOL isSeparator(SCM_CHAR aChar);
static CBOOL allDigits(char *cp);


static SCM_CHAR readCharacter(scm_stream inStream);
static void unreadCharacter(scm_stream inStream, SCM_CHAR charToUnread);


static SCM_CHAR skipSeparators(scm_stream inStream);
