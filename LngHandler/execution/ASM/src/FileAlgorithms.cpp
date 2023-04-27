#include "FileAlgorithms.h"
#include "LOG.h"

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <algorithm>

#define NASSERT

//-----------------------------------------------------------------------------

bool OpenFile( FILE** file, const char* fileName, 
                            const char* typeOpen )
{
    *file = fopen( fileName, typeOpen );

    if( !(*file) )
    {
        printf( "Error while opening the file: \"%s\" with \"%s\" mode...\n", fileName, typeOpen );  
        
        return 0; 
    }

    return 1;
} 

//-----------------------------------------------------------------------------

long int GetFileSize (FILE* file) //fstat
{
    $LOG_LVL_UP

    //{ ASSERT
    assert (file != NULL);
    //}

    long int curPos = ftell (file);

    fseek (file, 0, SEEK_END);

    long int fileSize = ftell (file);

    fseek (file, curPos, SEEK_SET);

    FLOG ("fileSize = %d", fileSize);

    return fileSize;
}

//-----------------------------------------------------------------------------

long int GetFileSizeFromStat (FILE* file) 
{
    $LOG_LVL_UP

    //{ ASSERT
    assert (file != NULL);
    //}

    struct stat fileInfo = {0};

    fstat (fileno (file), &fileInfo);

    long int fileSize = fileInfo.st_size;

    return fileSize;
} 

//-----------------------------------------------------------------------------

long int ReadAllFile (FILE* file, char** str)
{
    $LOG_LVL_UP

    // ASSERT
    assert (file != NULL);
    assert (str  != NULL);
    //

    long int fileSize = GetFileSizeFromStat (file);
    
    *str = (char*) calloc (sizeof (char), fileSize + 1);

    long int rightRead = fread (*str, sizeof (char), fileSize, file);

    if (rightRead < fileSize)
        realloc (str, sizeof (char) * (rightRead + 1)); // Windows specific, \r remove

    (*str)[rightRead] = '\0';

    return rightRead;
}

//-----------------------------------------------------------------------------
