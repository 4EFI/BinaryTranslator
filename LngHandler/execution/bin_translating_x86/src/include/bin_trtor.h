
#ifndef BIN_TRTOR_H
#define BIN_TRTOR_H

#include "config.h"

#include <stdio.h>

const int StackSize = 1000;

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
int BinTrtorToELF( BinTrtor* bin_trtor, const char file_name );

//-----------------------------------------------------------------------------

int CheckBinCodeSignature( const char* bin_code );

int BinEmit( char* bin_code, int size, ... );

extern "C" int _printf( const char* str, ... ); 

//-----------------------------------------------------------------------------

#endif