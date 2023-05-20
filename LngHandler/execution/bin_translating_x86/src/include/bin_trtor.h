
#ifndef BIN_TRTOR_H
#define BIN_TRTOR_H

#include "config.h"

#include <stdio.h>
#include <elf.h>

const int StackSize = 1000;

const int PAGE_SIZE = 0x1000;
const int CODE_SIZE = 0x2000;
const int DATA_SIZE = 0x2000;

const int LOAD_ADDR = 0x400000;
const int TEXT_ADDR = 0x401000;

const int RAM_ADDR  = 0x403000;
const int STK_ADDR  = 0x404000;
const int LIB_ADDR  = 0x405000;


const int STR_ADDR    = 0x405000;  
const int PRINTF_ADDR = 0x405804;  

//-----------------------------------------------------------------------------

enum BinType { JIT, ELF };

struct Command
{
    CMD cmd;

    int    reg_num;
    double val;

    int   bin_code_pos;
    char* bin_code_x86_ptr;
};

struct BinTrtor
{
    char*  bin_code_x86;
    size_t bin_code_x86_size;
    
    const char* bin_code;
    size_t      bin_code_size;

    size_t num_cmds;

    Command*   commands;
    double*    RAM;
    u_int64_t* stack_ret;
};

//-----------------------------------------------------------------------------

int BinTrtorCtor( BinTrtor* bin_trtor, const char* bin_code );
int BinTrtorDtor( BinTrtor* bin_trtor );

int BinTrtorParseBinCode( BinTrtor* bin_trtor );

int BinTrtorToX86( BinTrtor* bin_trtor, int bin_type );

int BinTrtorRun  ( BinTrtor* bin_trtor );
int BinTrtorToELF( BinTrtor* bin_trtor, const char* lib_file_name, const char* file_name );

//-----------------------------------------------------------------------------

int CheckBinCodeSignature( const char* bin_code );

int BinEmit( char* bin_code, int size, ... );

Elf64_Phdr HeaderInit( Elf64_Word p_flags, Elf64_Addr addr );

char* GetFilePath( const char* full_file_path );

long int GetFileSizeFromStat( FILE* file );
long int ReadAllFile        ( FILE* file, char** str );

extern "C" int _printf( const char* str, ... ); 

//-----------------------------------------------------------------------------

#endif