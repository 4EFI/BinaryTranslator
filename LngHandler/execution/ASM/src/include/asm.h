#ifndef ASM_H
#define ASM_H

#include "config.h"

#include <stdio.h>

#include "StrAlgorithms.h"

//-----------------------------------------------------------------------------

struct Label
{
    int    val;
    String name;
};

struct ASM
{
    Text text;

    Label labels[NumLabels];

    int   codeSize;
    char* code;
};

int AsmCtor( ASM* asm_s );
int AsmDtor( ASM* asm_s );

int AsmGetCmds       ( ASM* asm_s, FILE* fileIn );
int AsmLabelHandler  ( ASM* asm_s, const char* strForRead, int  ip );
int AsmArgJumpHandler( ASM* asm_s, const char* strForRead, int* ip );
int AsmArgHandler    ( ASM* asm_s, const char* strForRead, int* ip );
int AsmMakeArrCmds   ( ASM* asm_s) ;
int AsmMakeBinFile   ( ASM* asm_s, FILE* fileOut );

int GetLabelIndex     ( Label labels[], int numLabels, const char* str, int len );
int GetEmptyLabelIndex( Label labels[], int numLabels );

int SetLabel( Label* label, const char* name, int len, Elem_t val );

int GetRegIndex( const char* reg );

//-----------------------------------------------------------------------------

#endif