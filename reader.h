//
// Created by admin on 08.04.17.
//

#ifndef HBSCHEME_READER_H
#define HBSCHEME_READER_H

#endif //HBSCHEME_READER_H


enum streamType {
    FILESTREAM,
    STRINGSTREAM
};


struct stream_struct {
    enum streamType type;
    FILE *fileStream;
    char *theString;
    int indexInString;
    int peekChar;
};




typedef struct stream_struct* stream;






/****** global functions ************/

extern stream newFileStream(FILE *);

