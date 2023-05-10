
#include "config.h"
#include "var_handler.h"

#include "stack.h"
#include "my_assert.h"
#include "LOG.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//-----------------------------------------------------------------------------

Stack StkVarTables = { 0 };

//-----------------------------------------------------------------------------

int AddLocalVarsBlock( FILE* file, int isNewFunc )
{
    ASSERT( file != NULL, 0 );
    
    static int isStkEmpty = true;
    if(        isStkEmpty        ) { isStkEmpty = false; StackCtor( &StkVarTables, 1 ); }

    VarTable* varTable  = ( VarTable* )calloc( 1, sizeof( VarTable ) );
    varTable->pos       = 0; 
    varTable->numVars   = 0;
    varTable->isNewFunc = isNewFunc;
    
    StackPush( &StkVarTables, varTable );

    return 1;
}

//-----------------------------------------------------------------------------

int ShiftRegTop( FILE* file )
{
    ASSERT( file != NULL, NULL );

    int curStkPos  = StkVarTables.size - 1;
    if( curStkPos >= 0 ) // Shift rax
    {
        fprintf( file, "push %d + rax\n", StkVarTables.data[ curStkPos ]->numVars * sizeof( double ) );
        fprintf( file, "pop rax ; Shifting top the var register\n" );

        LOG( "New block" );
    }

    return 1;
}

//-----------------------------------------------------------------------------

int RemoveLocalVarsBlock( FILE* file )
{
    ASSERT( file != NULL, 0 );

    StackPop( &StkVarTables );

    return 1;
}

//-----------------------------------------------------------------------------

int ShiftRegDown( FILE* file )
{
    ASSERT( file != NULL, 0 );
    
    int curStkPos  = StkVarTables.size - 1;
    if( curStkPos >= 0 )
    {
        fprintf( file, "push %d + rax\n", -StkVarTables.data[ curStkPos ]->numVars * sizeof( double ) );
        fprintf( file, "pop rax ; Shifting down the var register\n" );
        
        LOG( "Remove block" );
    }

    return 1;
}

//-----------------------------------------------------------------------------

int GetTableVarPos( const char* varName )
{
    ASSERT( varName != NULL, 0 );

    int numBack        = 0;
    int indexLastBlock = StkVarTables.size - 1;

    int isNewFunc = false;

    for( int curTable = indexLastBlock; curTable >= 0; curTable-- )
    {        
        int curTableSize = StkVarTables.data[ curTable ]->numVars;
        
        if( !isNewFunc || curTable == 0 )
        {
            for( int i = 0; i < curTableSize; i++ )
            {
                char* curVarName = StkVarTables.data[ curTable ]->varNames[i];
                
                if( !strcmp( varName, curVarName ) )
                {                
                    if( curTable == indexLastBlock ) { return i; }
                    else                             { return -( numBack + curTableSize - i ); } 
                }
            }

            isNewFunc = StkVarTables.data[ curTable ]->isNewFunc;
        }

        if( curTable != indexLastBlock ) numBack += curTableSize; // If not the last local block
    }

    printf( "Variable \"%s\" does not exist...\n", varName );

    assert( 0 );
    return  0;
}

//-----------------------------------------------------------------------------

int AddVarToTable( const char* varName )
{
    ASSERT( varName != NULL, 0 );

    int curTable  = StkVarTables.size - 1;

    LOG( "CurTable = %d", curTable );

    int curVarPos = StkVarTables.data[ curTable ]->numVars;

    StkVarTables.data[ curTable ]->varNames[ curVarPos ] = ( char* )varName; // set varName
    StkVarTables.data[ curTable ]->numVars++; 

    return curVarPos;
}

//-----------------------------------------------------------------------------