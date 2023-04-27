#ifndef FILE_ALGORITHMS_H
#define FILE_ALGORITHMS_H

#include <stdio.h>

bool OpenFile( FILE** file, const char* fileName, 
                            const char* typeOpen );

long int GetFileSize        ( FILE* file );
long int GetFileSizeFromStat( FILE* file );

long int ReadAllFile( FILE* file, char** str );

#endif
