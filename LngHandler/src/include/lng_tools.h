#ifndef LNG_TOOLS_H
#define LNG_TOOLS_H

#include "config.h"

//-----------------------------------------------------------------------------

enum // Type of str 
{
    LNG,
    STANDART
};

//-----------------------------------------------------------------------------

int GetOperationType( const char* str, int strType = LNG );

int GetIndexOperation( int numOp );

int GetType( const char* str, int* opType, int strType = LNG );

int GetIndexType( int numType );

//-----------------------------------------------------------------------------

#endif