
#include "lng_tools.h"
#include "my_assert.h"

#include <stdio.h>
#include <string.h>

//-----------------------------------------------------------------------------

int GetOperationType( const char* str, int strType )
{
	ASSERT( str != NULL, -1 );

	for( int i = 0; i < NumOpStrings; i++ )
    {
        if/* */( strType == LNG )
        {
            if( strcmp( str, OpStrings[i].str ) == 0 )
            {
                return OpStrings[i].opNum;
            }
        }   
        else if( strType == STANDART )
        {
            if( strcmp( str, OpStrings[i].strStandart ) == 0 )
            {
                return OpStrings[i].opNum;
            }
        }
    }	

	return -1;
}

//-----------------------------------------------------------------------------

int GetIndexOperation( int numOp )
{
    for( int i = 0; i < NumOpStrings; i++ )
    {
        if( OpStrings[i].opNum == numOp ) return i;
    }

    return -1;
}

//-----------------------------------------------------------------------------

int GetType( const char* str, int* opType, int strType )
{
    ASSERT( str    != NULL, -1 );
    ASSERT( opType != NULL, -1 );

    int type = GetOperationType( str, strType );
    if( type != -1 )
    {
        if( opType ) *opType = type;

        return OP_TYPE;
    }

    for( int i = 0; i < NumTypeStrings; i++ )
    {
        if( strType == LNG )
        {
            if( strcmp( str, TypeStrings[i].str ) == 0 )
            {
                return TypeStrings[i].typeNum;
            }
        }
        else
        {
            if( strcmp( str, TypeStrings[i].strStandart ) == 0 )
            {
                return TypeStrings[i].typeNum;
            }
        }
    }	

    return -1;
}

//-----------------------------------------------------------------------------

int GetIndexType( int numType )
{
    for( int i = 0; i < NumTypeStrings; i++ )
    {
        if( TypeStrings[i].typeNum == numType ) return i;
    }

    return -1;    
}

//-----------------------------------------------------------------------------