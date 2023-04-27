
#include <stdio.h>

#include "cpu.h"

//-----------------------------------------------------------------------------

int main( int argc, const char* argv[] )
{    
    FILE* file = NULL;

    if( argc > 1 ) file = fopen( argv[1], "rb" );
    
    if( !file ) 
    {
        printf( "Cpu file opening error...\n" );
        return -1;
    }
    
    if( !CheckCompatibility( file ) ) return -1;

    CPU cpu = {0};
    CpuCtor( &cpu );

    CpuGetCmdsArr ( &cpu, file );
    CpuCmdsHandler( &cpu );    
}

//-----------------------------------------------------------------------------