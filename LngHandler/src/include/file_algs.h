#ifndef FILE_ALGORITHMS_H
#define FILE_ALGORITHMS_H

#include <stdio.h>

bool OpenFile (FILE** file, const char* fileName, 
                            const char* typeOpen, 
                            const char* errorOpenning = "An error during opening file...\n");

long int GetFileSize         (FILE* file);
long int GetFileSizeFromStat (FILE* file);

long int ReadAllFile (FILE* file, char** str);

#endif
