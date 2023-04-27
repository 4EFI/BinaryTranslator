//#define NLOG

#include "StrAlgorithms.h"
#include "FileAlgorithms.h"
#include "LOG.h"

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <algorithm>

// Text
//-----------------------------------------------------------------------------

void TextInit(Text* text)
{
    text->isCopyStr   = true;
    text->isCopyLines = true;
    
    text->buffer   = NULL; 
    text->strSize  = 0; 
    text->lines    = NULL;
    text->numLines = 0;   
}

//-----------------------------------------------------------------------------

void TextDestr (Text* text)
{
    if (!text->isCopyStr)   free (text->buffer);
    if (!text->isCopyLines) free (text->lines);
}

//-----------------------------------------------------------------------------

long int TextSetFileStr (Text* text, FILE* file)
{
    //{ ASSERT
    assert (file != NULL);
    assert (text != NULL);
    //}   

    text->strSize = ReadAllFile (file, &(text->buffer)); 

    if (text->buffer != NULL) text->isCopyStr = false;

    return text->strSize;
}

//-----------------------------------------------------------------------------

long int TextSetFileLines (Text* text, FILE* file)
{
    //{ ASSERT
    assert (file != NULL);
    assert (text != NULL);
    //}
    
    if (text->buffer == NULL) text->strSize  = TextSetFileStr (text, file);    
    
    if (text->buffer != NULL) text->numLines = DivideStr (text->buffer, &(text->lines));

    for (int i = 0; i < text->numLines; i++)
    {
        text->lines[i].numLeftIgnSyms  = NumLeftIgnoredSyms  (text->lines[i].str, 0);
        text->lines[i].numRightIgnSyms = NumRightIgnoredSyms (text->lines[i].str, strlen( text->lines[i].str ) - 1);
    }

    text->isCopyLines = false;

    return text->numLines;
}

//-----------------------------------------------------------------------------
// End Text 

int DivideStr (char* str, String** arrStrs, char symNewLine)
{
    $LOG_LVL_UP

    //{ ASSERT
    assert (str != NULL);
    //}

    int numStrs = GetNumStrs (str, symNewLine);

    *arrStrs = (String*)calloc (sizeof (String), numStrs);

    int pos = 0, curStr = 0;

    for (int i = 0; ; i++)
    {
        if (str[i] == symNewLine || str[i] == '\0')
        {
            (*arrStrs)[curStr].str = &str[i - pos];
            (*arrStrs)[curStr].len = pos;

            // If end of str
            if (str[i] == '\0') break;

            (*arrStrs)[curStr].str[pos] = '\0';
            // Windows \r specific
            if (pos > 0 && (*arrStrs)[curStr].str[pos - 1] == '\r')
            {
                (*arrStrs)[curStr].str[pos - 1] = '\0';

                LOG ("asdfasf");
            }

            pos = 0;
            curStr++;

            continue;
        }

        pos++;
    }

    return numStrs;
}

//-----------------------------------------------------------------------------

int GetNumStrs (const char *str, char symNewLine)
{
    $LOG_LVL_UP

    //{ ASSERT
    assert (str != NULL);
    //}

    int numStrs = 0;

    for (int i = 0; ; i++)
    {
        if (str[i] == symNewLine || str[i] == '\0')
        {
            numStrs++;

            if (str[i] == '\0') break; 
        }
    }

    return numStrs;
}

//-----------------------------------------------------------------------------

int NumLeftIgnoredSyms (const char* str, int pos,  const char* ignoredSymbols)
{
    //{ ASSERT
    assert (str            != NULL);
    assert (ignoredSymbols != NULL);
    //}

    int numLeftIgnSyms = 0;

    for (int i = pos; ; i++)
    {
        // If end of str
        if (str[i] == '\0') break;

        if ( !strchr (ignoredSymbols, str[i]) ) break;

        numLeftIgnSyms++;
    }

    return numLeftIgnSyms;
}

//-----------------------------------------------------------------------------

int NumRightIgnoredSyms (const char* str, int pos, const char* ignoredSymbols)
{
    //{ ASSERT
    assert( str            != NULL );
    assert( ignoredSymbols != NULL );
    //}

    int numIgnoredSyms = 0;

    for( int i = pos; i >= 0; i-- )
    {
        if( str[i] == '\0' || !strchr( ignoredSymbols, str[i] ) ) break;

        numIgnoredSyms++;
    }

    return numIgnoredSyms;
}

//-----------------------------------------------------------------------------

char* CheckSymInStr (const char* str, char sym, const char* ignSyms)
{
    for (int i = 0; ; i++)
    {
        if (str[i] == '\0') break;

        if (strchr (ignSyms, str[i])) continue;

        if (str[i] == sym) return (char*)(&str[i]); 
        else break;
    }  

    return 0;
}

//-----------------------------------------------------------------------------

void BubbleSort (void* arr, size_t num, size_t size, int (*comparator)(const void* arr1, const void* arr2))
{
    $LOG_LVL_UP
    
    //{ ASSERT
    assert (arr != NULL);
    //}

    for (size_t i = 0; i < num; i++)
    {
        for (size_t j = 0; j < num - i - 1; j++)
        {
            if (comparator ((char*)arr + j * size, (char*)arr + (j + 1) * size) > 0)
            {            
                Swap ((char*)arr + j * size, (char*)arr + (j + 1) * size, size);
            }
        }
    }
}

//-----------------------------------------------------------------------------

void QuickSort (void* arr, size_t num, size_t size, int (*comparator)(const void* arr1, const void* arr2))
{
    $LOG_LVL_UP

    //{ ASSERT
    assert (arr != NULL);
    //}
    
    size_t beginArr = 0;
    size_t endArr   = num - 1;

    void* middle = (char*)arr + (num / 2) * size;

    while (beginArr <= endArr)
    {    
        while ( comparator ( (char*)arr + beginArr * size, middle) < 0 ) 
        {
            beginArr++;
        }

        while ( comparator ( (char*)arr + endArr   * size, middle) > 0 ) 
        {
            endArr--;
        }

        if (beginArr <= endArr) 
        {
            // Swap (arr[beginArr], arr[endArr])
            Swap ((char*)arr + beginArr * size, (char*)arr + endArr * size, size);

            beginArr++;
            endArr--;
        }
    } 

    if (endArr   > 0      ) QuickSort ((char*)arr,                   endArr + 1,        size, comparator);
    if (beginArr < num - 1) QuickSort ((char*)arr + beginArr * size, num    - beginArr, size, comparator);
}

//-----------------------------------------------------------------------------

void Swap (void* a, void* b, size_t size)
{
    //{ ASSERT
    assert (a != NULL);
    assert (b != NULL);
    //}
    
    char* str1 = (char*)a;
    char* str2 = (char*)b;
    char  temp = '\0';

    for (size_t i = 0; i < size; i++)
    {
        temp = str1[i];

        str1[i] = str2[i];
        str2[i] = temp;
    }
}

//-----------------------------------------------------------------------------

void CopyLines (String** strTo, const String* strFrom, int numLines)
{
    //{ ASSERT
    assert (strTo   != NULL);
    assert (strFrom != NULL);
    //}
    
    *strTo = (String*)calloc (numLines, sizeof (String)); 
    
    for (int i = 0; i < numLines; i++)
    {
        (*strTo)[i] = strFrom[i];
    }
}

//-----------------------------------------------------------------------------

void PrintLines (FILE* file, String* arrStrs, int numLines)
{
    $LOG_LVL_UP

    //{ ASSERT
    assert (file    != NULL);
    assert (arrStrs != NULL);
    //}
    
    int pos = 0;
    
    for (int i = 0; i < numLines; i++)
    {
        if ( arrStrs[i].str[0] == '\0' )
        {
            continue;
        }

        pos++;
        
        fprintf (file, "%4d) \"%s\"\n", pos, arrStrs[i].str);
    }
}

//-----------------------------------------------------------------------------

int StrReverseCmp (const char* str1, size_t len1, const char* str2, size_t len2)
{
    $LOG_LVL_UP
    
    //{ ASSERT
    assert (str1 != NULL);
    assert (str2 != NULL);
    //}

    if (len1 <= 0) len1 = strlen (str1);
    if (len2 <= 0) len2 = strlen (str2);
    
    size_t lenMin = std::min (len1, len2);

    for (size_t i = 1; i <= lenMin; i++)
    {        
        if      (str1[len1 - i] > str2[len2 - i]) return  1;
        else if (str1[len1 - i] < str2[len2 - i]) return -1;
    }

    if (len1 < len2) return -1;
    if (len1 > len2) return  1;

    return 0;
}

//-----------------------------------------------------------------------------

int WordPosInArray (const char* word, const char* arr[], int sizeArr, int n)
{
    //{ ASSERT
    assert (word != NULL);
    assert (arr  != NULL);
    //}

    if (n <= 0) n = strlen (word) + 1;

    for (int i = 0; i < sizeArr; i++)
    {
        if (strncmp (word, arr[i], n) == 0)
        {
            return i;
        }
    }

    return -1;
}

//-----------------------------------------------------------------------------

int StringPosInArray (String* str, String arr[], int sizeArr, int n)
{
    //{ ASSERT
    assert (str != NULL);
    assert (arr != NULL);
    //}

    if (n <= 0) n = str->len + 1;

    for (int i = 0; i < sizeArr; i++)
    {
        if (strncmp (str->str, arr[i].str, n) == 0)
        {
            return i;
        }
    }

    return -1;
}

//-----------------------------------------------------------------------------