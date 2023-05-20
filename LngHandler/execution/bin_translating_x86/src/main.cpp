
#include <stdio.h>
#include <stdlib.h>

#include "bin_trtor.h"
#include "file_algs.h"

//-----------------------------------------------------------------------------

int main( int argc, char* argv[] )
{    
    printf( "asdfsf\n" );
    
    const char*    bin_file_name = NULL;
    if( argc > 1 ) bin_file_name = argv[1]; else printf( "Input file have not given...\n" ); 

    FILE* bin_file = fopen( bin_file_name, "r" );
    if(  !bin_file  ) { printf( "\"%s\" does not exist...\n", bin_file_name ); return 0; };

    char* bin_code = NULL;
    ReadAllFile( bin_file, &bin_code );
    
    BinTrtor       bin_trtor = {};
    BinTrtorCtor( &bin_trtor, bin_code );

    BinTrtorParseBinCode( &bin_trtor );
    BinTrtorToX86       ( &bin_trtor, BinType::ELF );
    BinTrtorToELF       ( &bin_trtor, "out" );

    BinTrtorDtor( &bin_trtor );
}

//-----------------------------------------------------------------------------