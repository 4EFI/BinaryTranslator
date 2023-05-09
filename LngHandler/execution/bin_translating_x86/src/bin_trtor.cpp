
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdarg.h>

#include "bin_trtor.h"

#define FREE( PTR )     \
    free( PTR );        \
    PTR = NULL;

//-----------------------------------------------------------------------------

int BinTrtorCtor( BinTrtor* bin_trtor, const char* bin_code )
{
    size_t size = CheckBinCodeSignature( bin_code );
    if(   !size   ) return 0;

    bin_trtor->bin_code      = ( const char* )( bin_code + SignatureBlockSize );
    bin_trtor->bin_code_size = size;
    bin_trtor->num_cmds      = 0;

    bin_trtor->bin_code_x86_size = size * 10;
    bin_trtor->bin_code_x86 = ( char* )aligned_alloc( PageSize, bin_trtor->bin_code_x86_size );

    bin_trtor->commands = ( Command* )calloc( size,    sizeof( Command ) ); 
    bin_trtor->RAM      = ( double*  )calloc( RamSize, sizeof( double  ) );
    
    return 1;
}

//-----------------------------------------------------------------------------

int BinTrtorDtor( BinTrtor* bin_trtor )
{
    bin_trtor->bin_code = NULL;

    FREE( bin_trtor->bin_code_x86 );
    FREE( bin_trtor->commands );
    FREE( bin_trtor->RAM );

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

        bin_trtor->commands[i].bin_code_pos = int( curr_str_ptr - bin_trtor->bin_code );
        
        // get cmd 
        memcpy( &bin_trtor->commands[i].cmd, curr_str_ptr++, sizeof( char ) );

        CMD* cmd = &bin_trtor->commands[i].cmd;

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

//-----------------------------------------------------------------------------

size_t FindLabelCommand( BinTrtor* bin_trtor, int label_pos )
{
    for( size_t i = 0; i < bin_trtor->num_cmds; i++ )
    {   
        if( bin_trtor->commands[i].bin_code_pos == label_pos )
        {
            return i;
        }
    }

    return -1;
}

int FillJumpsVal( BinTrtor* bin_trtor )
{
    for( size_t i = 0; i < bin_trtor->num_cmds; i++ )
    {
        char* jmp_val_ptr  = bin_trtor->commands[i].jmp_x86_val_ptr;
        if(   jmp_val_ptr != NULL   )
        {
            u_int32_t jmp_val = ( u_int32_t )( bin_trtor->commands[i].bin_code_x86_ptr - jmp_val_ptr - sizeof( u_int32_t ) );

            memcpy( jmp_val_ptr, &jmp_val, sizeof( u_int32_t ) );
        }
    }
    
    return 0;
}

int BinTrtorToX86( BinTrtor* bin_trtor )
{
    char* bin_code_x86_ptr = bin_trtor->bin_code_x86;
    
    for( size_t i = 0; i < bin_trtor->num_cmds; i++ )
    {
        bin_trtor->commands[i].bin_code_x86_ptr = bin_code_x86_ptr; 
        
        CMD* cmd = &bin_trtor->commands[i].cmd;   

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

    FillJumpsVal( bin_trtor );

    *(bin_code_x86_ptr++) = char( 0xC3 ); // add 'ret' to end of buffer
    
    return 1;
}

int BinTrtorRun( BinTrtor* bin_trtor )
{
    int is_prot = mprotect( bin_trtor->bin_code_x86, 
                            bin_trtor->bin_code_x86_size,
                            PROT_READ | PROT_WRITE | PROT_EXEC );
    if( is_prot ) exit( errno );

    void ( *exec_function )( void ) = ( void ( * )( void ) )( bin_trtor->bin_code_x86 );
    exec_function();

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

int BinPrint( char* bin_code, int size, ... )
{
    va_list   args = {};
    va_start( args, size );

    for( int i = 0; i < size; i++ )
    {
        int c = va_arg( args, int );
        bin_code[i] = char( c );
    }

    va_end( args );

    return 1;
}

//-----------------------------------------------------------------------------