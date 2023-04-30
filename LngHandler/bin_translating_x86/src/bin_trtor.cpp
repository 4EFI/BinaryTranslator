
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bin_trtor.h"

//-----------------------------------------------------------------------------

int BinTrtorCtor( BinTrtor* bin_trtor, const char* bin_code )
{
    size_t size = CheckBinCodeSignature( bin_code );
    if(   !size   ) return 0;

    bin_trtor->bin_code = ( const char* )( bin_code + SignatureBlockSize );
    bin_trtor->num_cmds = 0;

    bin_trtor->bin_code_size = size;
    bin_trtor->commands      = ( Command* )calloc( bin_trtor->bin_code_size, sizeof( Command ) ); 
    
    return 1;
}

//-----------------------------------------------------------------------------

int BinTrtorDtor( BinTrtor* bin_trtor )
{
    bin_trtor->bin_code = NULL;

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
        int str_len =  curr_str_ptr - bin_trtor->bin_code; 
        if( str_len == bin_trtor->bin_code_size ) break;
        
        // get cmd 
        memcpy( &bin_trtor->commands[i].cmd, curr_str_ptr++, sizeof( char ) );
        
        CMD* cmd = &bin_trtor->commands[i].cmd;

        printf( "%x\n", *cmd );

        if( cmd->imm )
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

//-----------------------------------------------------------------------------

int CheckBinCodeSignature( const char* bin_code )
{
    int  version      = 0;
    char signature[3] = "";
    int  code_size    = 0;

    sscanf( bin_code, "%s %d %d", signature, &version, &code_size ); 

    // check 
    if( !strcmp( signature, Signature ) && version == Version ) return code_size;

    return 0;
}

//-----------------------------------------------------------------------------