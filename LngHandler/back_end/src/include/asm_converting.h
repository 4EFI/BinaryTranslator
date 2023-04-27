#ifndef ASM_CONVERTING_H
#define ASM_CONVERTING_H

#include <stdio.h>
#include "tree_lng.h"

//-----------------------------------------------------------------------------

enum
{
    IN, 
    OUT, 
    FUNC
};

int TreeToAsmConverting( Node* node, FILE* file );

int TreeToAsm          ( Node* node, FILE* file );
int MathExpressionToAsm( Node* node, FILE* file );
int IfToAsm            ( Node* node, FILE* file );
int WhileToAsm         ( Node* node, FILE* file );
int VarInitToAsm       ( Node* node, FILE* file );
int VarEqualToAsm      ( Node* node, FILE* file );
int InputToAsm         ( Node* node, FILE* file );
int OutputToAsm        ( Node* node, FILE* file );
int FuncParamsToAsm    ( Node* node, FILE* file );
int FuncToAsm          ( Node* node, FILE* file );
int CallFuncToAsm      ( Node* node, FILE* file );
int ReturnToAsm        ( Node* node, FILE* file );
int CallParamsToAsm    ( Node* node, FILE* file, int typeParams );

int VarRAMPosToAsm( const char* varName, FILE* file );

//-----------------------------------------------------------------------------

#endif