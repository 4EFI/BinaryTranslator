#include <stdio.h>

#include "asm.h"
#include "LOG.h"
#include "OptionsCmdLine.h"
#include "FileAlgorithms.h"

int SetFileOut( FILE** fileOut );

//-----------------------------------------------------------------------------

int main( int argc, const char* argv[] )
{
    FILE* fileIn  = NULL;
    FILE* fileOut = NULL;
 
    // No input file name
    if( argc <= 1 ) 
    {
        printf( "To few arguments...\n" );
        return -1;
    }

    // Set input file
    if( !OpenFile( &fileIn, argv[1], "r" ) ) return -1;

    START_OPT_HANDLER( argc, argv )
    {
        OPT_HANDLER( "-o", SetFileOut( &fileOut ); )    
    }

    if( !fileOut ) fileOut = fopen( "a.code", "wb" );

    ASM asm_s = { 0 };
    AsmCtor( &asm_s );

    AsmGetCmds( &asm_s, fileIn );

    for( int i = 0; i < 2; i++ ) 
    {
        if( AsmMakeArrCmds( &asm_s ) == -1 ) return -1;
    }

    AsmMakeBinFile( &asm_s, fileOut );

    fclose( fileIn );
    fclose( fileOut );
}

//-----------------------------------------------------------------------------

int SetFileOut( FILE** fileOut )
{
    if( __CUR_OPT__ < __NUM_OPTIONS__ - 1 ) 
    {
        *fileOut = fopen( Argv[__CUR_OPT__ + 1], "wb" );
    }

    return 1;
}

//-----------------------------------------------------------------------------