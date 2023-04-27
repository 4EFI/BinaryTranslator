#ifndef VAR_HANDLER_H
#define VAR_HANDLER_H

#include "stdio.h"

//-----------------------------------------------------------------------------

const int MaxNumVars = 100;

struct VarTable
{
    char* varNames[ MaxNumVars + 1 ];
    int   pos;
    int   numVars;
    int   isNewFunc;
};

//-----------------------------------------------------------------------------

int AddLocalVarsBlock   ( FILE* file, int isNewFunc = false );
int RemoveLocalVarsBlock( FILE* file );

int ShiftRegTop ( FILE* file );
int ShiftRegDown( FILE* file );

int ShigtRegDownFunc( FILE* file );

int GetTableVarPos( const char* varName );
int AddVarToTable ( const char* varName );

//-----------------------------------------------------------------------------

#endif