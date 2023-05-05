
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cpu.h"
#include "LOG.h"

//-----------------------------------------------------------------------------

int CpuCtor( CPU* cpu )
{
    if( cpu == NULL ) return 0;
    
    StackCtor( &cpu->stack,    1 );
    StackCtor( &cpu->stkRetIP, 1 );

    // Fill all with zero
    memset( cpu->RAM,  0, RamSize * sizeof( Elem_t ) );
    memset( cpu->regs, 0, NumRegs * sizeof( Elem_t ) );

    cpu->codeSize = 0;
    cpu->code     = NULL;

    return 1;
}

//-----------------------------------------------------------------------------

int CpuGetCmdsArr( CPU* cpu, FILE* file )
{
    if( file == NULL || file == NULL ) return 0;
    
    fread( &cpu->codeSize, sizeof( int ), 1, file );

    cpu->code = (char*)calloc( cpu->codeSize, sizeof( char ) );

    fread( cpu->code, sizeof( char ), cpu->codeSize, file );

    return 1;
}

//-----------------------------------------------------------------------------

Elem_t* CpuGetArg( CPU* cpu, int* ip, Elem_t* val )
{
    if( cpu == NULL ) return 0;

    CMD cmd = {0};
    *(char*)(&cmd) = cpu->code[(*ip)++];

    Elem_t* arg_ptr = NULL;   

    if( cmd.immed )
    {
        Elem_t temp = 0;
        memcpy( &temp, cpu->code + *ip, sizeof( Elem_t ) );

        (*val) += temp;
        (*ip)  += sizeof( Elem_t );
    }
    
    if( cmd.reg )
    {        
        (*val) +=  cpu->regs[int( cpu->code[*ip] )];
        arg_ptr = &cpu->regs[int( cpu->code[*ip] )];

        (*ip)++;
    }

    if( cmd.memory )
    {
        arg_ptr = &cpu->RAM[int(*val)];
        (*val)  =  cpu->RAM[int(*val)];
    }

    return arg_ptr;  
}

//-----------------------------------------------------------------------------

int CpuCmdsHandler( CPU* cpu )
{
    if( cpu == NULL ) return 0;

    for( int ip = 0; ip < cpu->codeSize; )
    {
        CMD cmd = {0};
        *(char*)(&cmd) = cpu->code[ip];

        Elem_t  arg_val = 0;
        Elem_t* arg_ptr = CpuGetArg( cpu, &ip, &arg_val );

        #define DEF_CMD( NAME, NUM, ... ) \
            case CMD_##NAME:              \
                __VA_ARGS__               \
                break;

        switch( cmd.code )
        {
            #include "commands.h"
            default:
                printf( "SIGILL\n" );
                return -1;
                break;
        }

        #undef DEF_CMD
    }
    
    return 1;
}

//-----------------------------------------------------------------------------

int CpuCmdDump( CPU* cpu, int ip, FILE* file )
{
    if( cpu == NULL || file == NULL ) return 0;
    
    fprintf( file, "\nCode Dump():\n" );
    
    for( int i = 0; i < cpu->codeSize; i++ )
    {
        fprintf( file, "%02X ", cpu->code[i] );
    }

    fprintf( file, "\n" );

    PrintSyms( '~', ip*3 - 3, file );
    
    fprintf( file, "^ip = %d\n\n", ip );

    return 1;
}

//-----------------------------------------------------------------------------

int CpuRegDump( CPU* cpu, FILE* file )
{
    if( cpu == NULL || file == NULL ) return 0;

    fprintf( file, "REGS Dump():\n" );
    PrintArr( cpu->regs, NumRegs, file );
    
    return 1;
}

//-----------------------------------------------------------------------------

int CpuRamDump( CPU* cpu, FILE* file )
{
    if( cpu == NULL || file == NULL ) return 0;

    fprintf ( file, "RAM Dump():\n" );
    PrintArr( cpu->RAM, RamSize, file );

    return 1;
}

//-----------------------------------------------------------------------------

int OutRam( CPU* cpu, int numSymsPerLine )
{
    if( cpu == NULL ) return 0;
    
    for( int i = 0; i < RamSize; i++ )
    {
        if( cpu->RAM[i] == 0 )
        {
            printf( "." );
        }
        else
        {
            printf( "%c", int(cpu->RAM[i]) );
        }

        if( (i + 1) % numSymsPerLine == 0 ) printf( "\n" );
    }

    return 1;
}

//-----------------------------------------------------------------------------

int CheckCompatibility( FILE* file )
{
    if( file == NULL ) return 0;

    char signature[MaxStrLen] = "";
    fscanf( file, "%s ", signature);

    if( strcmp( Signature, signature ) != 0 )
    {
        printf( "Incorrect signature...\n" );
        return 0;
    }

    char version = 0;
    char sym     = 0;
    fscanf( file, "%c%c", &version, &sym );

    if( version != Version ) 
    {
        printf( "Incompatibility versions: ASM version - %d | CPU version - %d...\n", version, Version );
        return 0;
    }

    return 1;
}

//-----------------------------------------------------------------------------

int PrintArr( Elem_t* arr, int arrSize, FILE* file )
{
    if( file == NULL || arr == NULL ) return 0;

    for( int i = 0; i < arrSize; i++ )
    {
        fprintf ( file, "[%d] = %g\n", i, double(arr[i]) ); 
    }

    return 1;
}

//-----------------------------------------------------------------------------