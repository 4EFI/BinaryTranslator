
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <ctime>
#include <chrono>

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

#define XOR( reg ) BIN_EMIT( 3, 0x48, 0x31, reg )

#define NULL_REGS()             \
{                               \
    XOR( 0xc0 ); /* rax */      \
    XOR( 0xc9 ); /* rcx */      \
    XOR( 0xd2 ); /* rdx */      \
    XOR( 0xdb ); /* rbx */      \
}                               \

int BinTrtorToX86( BinTrtor* bin_trtor, int bin_type )
{
    char* bin_code_x86_ptr = bin_trtor->bin_code_x86; 

    Jmp* jmps_arr = ( Jmp* )calloc( NumLabels, sizeof( Jmp ) );
    int  curr_jmp = 0;


    // mov r10, stack_ret_addr 
    if( bin_type == BinType::JIT ) { MOV_R10_PTR( bin_trtor->stack_ret ); } // JIT
    else                           { MOV_R10_PTR( STK_ADDR );             } // ELF
    
    // mov rbp, r10
    BIN_EMIT( 3, 0x4c, 0x89, 0xd5 ); 

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

    bin_trtor->bin_code_x86_size = bin_code_x86_ptr - bin_trtor->bin_code_x86;
    
    return 1;
}

//-----------------------------------------------------------------------------

#define PUSH_REGS asm( "push %rax\n push %rbx\n push %rcx\n push %rdx\n push %rsi\n push %rdi\n push %rbp\n push %r10\n");
#define POP_REGS  asm( "pop %r10\n pop %rbp\n pop %rdi\n pop %rsi\n pop %rdx\n pop %rcx\n pop %rbx\n pop %rax\n" );

int BinTrtorRun( BinTrtor* bin_trtor )
{
    int is_prot = mprotect( bin_trtor->bin_code_x86, 
                            bin_trtor->bin_code_x86_size,
                            PROT_READ | PROT_WRITE | PROT_EXEC );
    if( is_prot ) exit( errno );

    void ( *exec_function )( void ) = ( void ( * )( void ) )( bin_trtor->bin_code_x86 );

    // measure execution time
    auto start_msr  = std::chrono::steady_clock::now();

    PUSH_REGS
    exec_function();
    POP_REGS

    auto end_msr    = std::chrono::steady_clock::now();
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>( end_msr - start_msr );

    printf( "X86-64 exec time in nanoseconds = %lld\n", elapsed_ns.count() );

    mprotect( bin_trtor->bin_code_x86, 
              bin_trtor->bin_code_x86_size,
              PROT_READ | PROT_WRITE );

    return 1;
}

//-----------------------------------------------------------------------------

int LoadLib( const char* file_name, FILE* exec )
{
    FILE*  lib_file = fopen( file_name, "r" );
    if(   !lib_file   ) { printf( "Can not open lib file...\n" ); return 0; }

    char* lib_buff = NULL;
    long long lib_buff_size = ReadAllFile( lib_file, &lib_buff );

    fwrite( lib_buff, 1, lib_buff_size, exec );

    return 1;
}

int BinTrtorToELF( BinTrtor* bin_trtor, const char* lib_file_name, const char* file_name )
{
    char zero_fill[ELF_SIZE] = {0};

    FILE* exec = fopen( file_name, "w" );

    fwrite( zero_fill, ELF_SIZE, 1, exec );
    fseek ( exec, 0x0, SEEK_SET );

    Elf64_Ehdr elf_header = 
    {
        .e_ident = 
        { 
            /* (EI_NIDENT bytes)     */ 
            /* [0] EI_MAG:           */ ELFMAG0      , /* 0x7F */
                                        ELFMAG1      , /* 'E'  */
                                        ELFMAG2      , /* 'L'  */
                                        ELFMAG3      , /* 'F'  */
            /* [4] EI_CLASS:         */ ELFCLASS64   , /*  2   */
            /* [5] EI_DATA:          */ ELFDATA2LSB  , /*  1   */
            /* [6] EI_VERSION:       */ EV_CURRENT   , /*  1   */
            /* [7] EI_OSABI:         */ ELFOSABI_NONE, /*  0   */
            /* [8] EI_ABIVERSION:    */ 0x0,
            /* [9-15] EI_PAD:        */ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
        },
        .e_type      = ET_EXEC    , /* 2  */
        .e_machine   = EM_X86_64  , /* 62 */
        .e_version   = EV_CURRENT , /* 1  */
        .e_entry     = TEXT_ADDR  , /* (start address at runtime) */
        .e_phoff     = 64         , /* (bytes into file) */
        .e_shoff     = ELF_SIZE   , /* (bytes into file) */
        .e_flags     = 0x0        ,
        .e_ehsize    = 64         , /* (bytes) */
        .e_phentsize = 56         , /* (bytes) */
        .e_phnum     = 5          , /* (program headers) */
        .e_shentsize = 64         , /* (bytes) */
        .e_shnum     = 0          , /* (section headers) */
        .e_shstrndx  = 0         
    };

    Elf64_Phdr first_pg_header  = HeaderInit( PF_R,        LOAD_ADDR );
    Elf64_Phdr second_pg_header = HeaderInit( PF_R | PF_X, TEXT_ADDR );
    Elf64_Phdr ram_header       = HeaderInit( PF_R | PF_W, RAM_ADDR  );
    Elf64_Phdr stack_header     = HeaderInit( PF_R | PF_W, STK_ADDR  );
    Elf64_Phdr library_header   = HeaderInit( PF_R | PF_X, LIB_ADDR  );

    fwrite( &elf_header,       sizeof( elf_header       ), 1, exec );
    fwrite( &first_pg_header,  sizeof( first_pg_header  ), 1, exec );
    fwrite( &second_pg_header, sizeof( second_pg_header ), 1, exec );
    fwrite( &ram_header,       sizeof( ram_header       ), 1, exec );
    fwrite( &stack_header,     sizeof( stack_header     ), 1, exec );
    fwrite( &library_header,   sizeof( library_header   ), 1, exec );
    
    fseek ( exec, TEXT_ADDR - LOAD_ADDR, SEEK_SET );
    fwrite( bin_trtor->bin_code_x86, 1, bin_trtor->bin_code_x86_size, exec );

    fseek  ( exec, LIB_ADDR - LOAD_ADDR, SEEK_SET );
    LoadLib( lib_file_name, exec ); 

    fclose( exec );   
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

int BinEmit( char* bin_code, int size, ... )
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

Elf64_Phdr HeaderInit( Elf64_Word p_flags, Elf64_Addr addr )
{
    Elf64_Phdr self = 
    {
        .p_type   = 1               , /* [PT_LOAD] */
        .p_flags  = p_flags         , /* PF_R */
        .p_offset = addr - LOAD_ADDR, /* (bytes into file) */
        .p_vaddr  = addr            , /* (virtual addr at runtime) */
        .p_paddr  = addr            , /* (physical addr at runtime) */
        .p_filesz = CODE_SIZE       , /* (bytes in file) */
        .p_memsz  = DATA_SIZE       , /* (bytes in mem at runtime) */
        .p_align  = PAGE_SIZE       ,/* (min mem alignment in bytes) */
    };

    return self;
}

//-----------------------------------------------------------------------------

// Path without last '/'

char* GetFilePath( const char* full_file_path )
{
	int len_full_path = strlen( full_file_path );
	int len_file_name = 0;

	for( int i = len_full_path - 1; i >= 0; i-- )
	{
		if( full_file_path[i] == '/' || 
			full_file_path[i] == '\\' )
		{
			break;
		}

		len_file_name++;
	}
	
	int len_path = len_full_path - len_file_name;
	
	char* path = ( char* )calloc( len_path + 1, sizeof( char ) ); // +1 for '\0'

	if( len_path > 0 ) strncpy( path, full_file_path, len_path - 1 );
	
	return path;
}

//-----------------------------------------------------------------------------

long int GetFileSizeFromStat( FILE* file ) 
{
    struct stat fileInfo = {};

    fstat( fileno( file ), &fileInfo );

    long int fileSize = fileInfo.st_size;

    return fileSize;
} 

//-----------------------------------------------------------------------------

long int ReadAllFile( FILE* file, char** str )
{
    long int fileSize = GetFileSizeFromStat( file );
    
    *str = ( char* )calloc( sizeof( char ), fileSize + 1 );

    long int rightRead = fread( *str, sizeof( char ), fileSize, file ); 

    if( rightRead < fileSize )
        realloc( str, sizeof( char ) * ( rightRead + 1 ) ); // Windows specific, \r remove

    (*str)[rightRead] = '\0';

    return rightRead;
}

//-----------------------------------------------------------------------------