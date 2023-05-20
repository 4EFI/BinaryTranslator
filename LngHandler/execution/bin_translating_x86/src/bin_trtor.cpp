
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

    bin_trtor->commands  = ( Command*   )calloc( size,      sizeof( Command   ) ); 
    bin_trtor->RAM       = ( double*    )calloc( RamSize,   sizeof( double    ) );
    bin_trtor->stack_ret = ( u_int64_t* )calloc( StackSize, sizeof( u_int64_t ) );
    
    return 1;
}

//-----------------------------------------------------------------------------

int BinTrtorDtor( BinTrtor* bin_trtor )
{ 
    bin_trtor->bin_code = NULL;

    FREE( bin_trtor->bin_code_x86 );
    FREE( bin_trtor->commands );
    FREE( bin_trtor->RAM );
    FREE( bin_trtor->stack_ret );

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
        bin_trtor->commands[i].cmd = *( CMD * )curr_str_ptr++;

        CMD* cmd = &bin_trtor->commands[i].cmd;

        if( cmd->immed )
        {
            double val = *( double * )curr_str_ptr;

            bin_trtor->commands[i].val = val; 
            curr_str_ptr += sizeof( double );
        }

        if( cmd->reg )
        {
            char reg_num = *curr_str_ptr++;

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

struct Jmp
{
    char*      jmp_val_ptr;
    u_int32_t* jmp_val; 
};

int FillJumpsVal( Jmp* jmps_arr, int num )
{
    for( int i = 0; i < num; i++ )
    {
        u_int32_t jmp_val = ( u_int32_t )( *jmps_arr[i].jmp_val - u_int64_t( jmps_arr[i].jmp_val_ptr ) - sizeof( u_int32_t ) );

        memcpy( jmps_arr[i].jmp_val_ptr, &jmp_val, sizeof( u_int32_t ) );
    }
    
    return 0;
}

#define XOR( reg ) BIN_PRINT( 3, 0x48, 0x31, reg );

#define NULL_REGS()             \
{                               \
    XOR( 0xc0 ); /* rax */      \
    XOR( 0xc9 ); /* rcx */      \
    XOR( 0xd2 ); /* rdx */      \
    XOR( 0xdb ); /* rbx */      \
}                               \

int BinTrtorToX86( BinTrtor* bin_trtor )
{
    char* bin_code_x86_ptr = bin_trtor->bin_code_x86; 

    Jmp* jmps_arr = ( Jmp* )calloc( NumLabels, sizeof( Jmp ) );
    int  curr_jmp = 0;

    // mov r10, stack_ret_addr 
    MOV_R10_PTR( bin_trtor->stack_ret );
    
    // mov rbp, r10
    BIN_PRINT( 3, 0x4c, 0x89, 0xd5 ); 

    NULL_REGS();

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
            #include "commands_x86.h"
            default:
                printf( "SIGILL\n" );
                return -1;
                break;
        }

        #undef DEF_CMD
        #undef BT
    }

    FillJumpsVal( jmps_arr, curr_jmp );

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