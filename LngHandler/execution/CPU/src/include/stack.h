#ifndef STACK_H
#define STACK_H

#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

//---------------------------------------------------------------------------

const int TabSize = 4;

//---------------------------------------------------------------------------

extern const char* CurFileName;
extern const char* CurFuncName;
extern int         CurLine;

//---------------------------------------------------------------------------

extern const Elem_t StackDataPoisonValue;

extern FILE* StackFileOut;

//---------------------------------------------------------------------------

extern const int ResizeUp;
extern const int ResizeNum;
extern const int ResizeDown;

//---------------------------------------------------------------------------

// No hash protection
#ifndef NHASH
    #define ON_HASH_PROTECTION(...)   __VA_ARGS__  
#else
    #define ON_HASH_PROTECTION(...) 
#endif

// No canary protection
#ifndef NCANARY
    #define ON_CANARY_PROTECTION(...)  __VA_ARGS__  
#else
    #define ON_CANARY_PROTECTION(...) 
#endif

// No dump
#ifdef STACK_DUMP 
    #define ON_STACK_DUMP(...)  __VA_ARGS__
#else
    #define ON_STACK_DUMP(...)
#endif

//---------------------------------------------------------------------------

struct HashIgnore
{
    size_t pos;
    size_t size;
};

//---------------------------------------------------------------------------

struct StackInfo
{
    const char* mainFileName;
    const char* mainFuncName;
    const char* mainStackName;

    double stepResize;

    uint64_t errStatus;
    
    ON_HASH_PROTECTION
    (
        uint64_t hashValue;

        int numHashIgnore; 
        HashIgnore* arrHashIgnorePtr;
    );

    bool isStackValid;
};

struct Stack
{
    // Canary left protection
    ON_CANARY_PROTECTION ( uint64_t canaryLeft; )
    
    StackInfo info;
    
    Elem_t* data;
    
    size_t size;
    size_t capacity;

    // Canary right protection
    ON_CANARY_PROTECTION ( uint64_t canaryRight; )
};

//---------------------------------------------------------------------------

int  _StackCtor (Stack* stack, int dataSize, const char* mainFileName, 
                                               const char* mainFuncName, 
                                               const char* mainStackName);
int   StackDtor (Stack* stack);
void _StackDump (Stack* stack);

uint64_t StackHashProtection (Stack* stack);
uint64_t StackIsValid        (Stack* stack); 

int    StackErrHandler (Stack* stack);
int    StackErrPrint   (Stack* stack, int indent = 0);

int    StackResize     (Stack* stack, int numResize, int sideResize = ResizeNum);

void   StackPush       (Stack* stack, Elem_t value);    
Elem_t StackPop        (Stack* stack);

//---------------------------------------------------------------------------

int PrintSyms (char sym, int num, FILE* file);

//---------------------------------------------------------------------------

size_t NumBytesHashIgnore (void* arrToComp, void* arr, HashIgnore* arrHashIgnorePtr, size_t numHashIgnore);

uint64_t HashProtection (void*       arr, 
                         size_t      size, 
                         HashIgnore* arrHashIgnore = NULL,
                         size_t      numHashIgnore = 0);

int    CanaryDataSet  (void* data, size_t size, uint64_t leftCanary, uint64_t rightCanary);
void*  CanaryRecalloc (void* data, size_t size, uint64_t leftCanary, uint64_t rightCanary);
void*  Recalloc       (void* data, size_t size, int      curSize = 0);
size_t MallocSize     (void* data);

bool CompareDoubles (double a, double b, double accuracy = 1e-6);

//---------------------------------------------------------------------------

#define StackCtor(stack, dataSize) { _StackCtor (stack, dataSize, __FILE__, __PRETTY_FUNCTION__, #stack); }

#define StackDump(stack) { CurFileName = __FILE__;            \
                           CurFuncName = __PRETTY_FUNCTION__; \
                           CurLine     = __LINE__;            \
                           _StackDump (stack); }

//---------------------------------------------------------------------------

#endif