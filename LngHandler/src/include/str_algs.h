#ifndef STR_ALGORITHM_H
#define STR_ALGORITHM_H

#include <stdio.h>

//-----------------------------------------------------------------------------

struct String
{
    char* str; 
    int   len;

    int numLeftIgnSyms;
    int numRightIgnSyms;
};

//-----------------------------------------------------------------------------

struct Text
{
    bool isCopyStr;
    bool isCopyLines; 

    char*    buffer;
    long int strSize;
    
    String*  lines;
    long int numLines; 
};

void TextInit  (Text* text);
void TextDestr (Text* text);

long int TextSetFileStr   (Text* text, FILE* file);
long int TextSetFileLines (Text* text, FILE* file);

//-----------------------------------------------------------------------------

static const char IgnoredSymbols[] = " .,()[]{}<>\\/|~`!?:;-'";

//-----------------------------------------------------------------------------

int DivideStr (char* str, String** arrStrs, char symNewLine = '\n');

int GetNumStrs (const char* str, char symNewLine = '\n');

int NumLeftIgnoredSyms  (const char* str, const char* ignoredSymbols = IgnoredSymbols);
int NumRightIgnoredSyms (const char* str, const char* ignoredSymbols = IgnoredSymbols);

int RemoveIgnoredSyms (char** str, int iBegin, int iEnd);

void BubbleSort (void* arr, size_t num, size_t size, int (*comparator)(const void* arr1, const void* arr2));
void QuickSort  (void* arr, size_t num, size_t size, int (*comparator)(const void* arr1, const void* arr2));

void Swap (void* a, void* b, size_t size);

void CopyLines (String** strTo, const String* strFrom, int numLines);

void PrintLines (FILE* file, String* arrStrs, int numLines);

int StrReverseCmp (const char* str1, size_t len1, const char* str2, size_t len2);

//-----------------------------------------------------------------------------

#endif
