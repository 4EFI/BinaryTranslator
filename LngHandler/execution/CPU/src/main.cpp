
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

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
    
    auto start_msr  = std::chrono::steady_clock::now();

    CpuCmdsHandler( &cpu );    

    auto end_msr    = std::chrono::steady_clock::now();
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>( end_msr - start_msr );

    printf( "My CPU exec time in nanoseconds = %lld\n", elapsed_ns.count() );
}

//-----------------------------------------------------------------------------