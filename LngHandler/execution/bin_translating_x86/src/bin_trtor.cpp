
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "bin_trtor.h"

//-----------------------------------------------------------------------------

int BinTrtorCtor( BinTrtor* bin_trtor, const char* bin_code )
{
    size_t size = CheckBinCodeSignature( bin_code );
    if(   !size   ) return 0;

    bin_trtor->bin_code      = ( const char* )( bin_code + SignatureBlockSize );
    bin_trtor->bin_code_size = size;
    bin_trtor->num_cmds      = 0;

    bin_trtor->bin_code_x86 = ( char*    )calloc( size * 10, sizeof( char    ) );
    bin_trtor->commands     = ( Command* )calloc( size,      sizeof( Command ) ); 
    
    return 1;
}

//-----------------------------------------------------------------------------

int BinTrtorDtor( BinTrtor* bin_trtor )
{
    bin_trtor->bin_code = NULL;

    free( bin_trtor->bin_code_x86 );
    bin_trtor->bin_code_x86 = NULL;

    free( bin_trtor->commands );
    bin_trtor->commands = NULL;

    bin_trtor->num_cmds      = 0;
    bin_trtor->bin_code_size = 0;

    return 1;
}

//-----------------------------------------------------------------------------

int BinTrtorParseBinCode( BinTrtor* bin_trtor )
{
    const char* curr_str_ptr = bin_trtor->bin_code;
    
    for( size_t i = 0; i < bin_trtor->bin_code_size; i++ )
    {        
        size_t str_len =  curr_str_ptr - bin_trtor->bin_code; 
        if(    str_len == bin_trtor->bin_code_size    ) break;
        
        // get cmd 
        memcpy( &bin_trtor->commands[i].cmd, curr_str_ptr++, sizeof( char ) );

        CMD* cmd = &bin_trtor->commands[i].cmd;

        printf( "cmd = %d\n", *cmd );

        if( cmd->immed )
        {
            double   val = 0;
            memcpy( &val, curr_str_ptr, sizeof( double ) );

            bin_trtor->commands[i].val = val;
            curr_str_ptr += sizeof( double );
        }

        if( cmd->reg )
        {
            char     reg_num = 0;
            memcpy( &reg_num, curr_str_ptr++, sizeof( char ) );

            bin_trtor->commands[i].reg_num = reg_num;
        }

        bin_trtor->num_cmds++;
    }
    
    return 1;
}

int BinTrtorToX86( BinTrtor* bin_trtor )
{
    char* bin_code_x86_ptr = bin_trtor->bin_code_x86;

    for( size_t i = 0; i < bin_trtor->num_cmds; i++ )
    {
        CMD* cmd = &bin_trtor->commands[i].cmd;   

        printf( "%d\n", cmd->code );

        #define BT
        #define DEF_CMD( NAME, NUM, ... ) \
            case CMD_##NAME:              \
                __VA_ARGS__               \
                break;

        switch( cmd->code )
        {
            #include "commands.h"
            default:
                printf( "SIGILL\n" );
                return -1;
                break;
        }

        #undef DEF_CMD
        #undef BT
    }
    
    return 1;
}

int BinTrtorRun( BinTrtor* bin_trtor )
{
    mprotect( bin_trtor->bin_code_x86, 
              bin_trtor->bin_code_x86_size,
              PROT_EXEC );

    void (*function)() = ( void(*)() )( bin_trtor->bin_code_x86 );
    function();

    return 1;
}

//-----------------------------------------------------------------------------

int CheckBinCodeSignature( const char* bin_code )
{
    char version      = 0;
    char signature[3] = "";

    int num_read_syms = 0;
    sscanf( bin_code, "%s %c%n", signature, &version, &num_read_syms );

    bin_code += ( num_read_syms + 1 ); 

    // check 
    if( !( !strcmp( signature, Signature ) && version == Version ) ) return 0;

    int code_size = 0;
    memcpy( &code_size, bin_code, sizeof( int ) );

    return code_size;
}

//-----------------------------------------------------------------------------