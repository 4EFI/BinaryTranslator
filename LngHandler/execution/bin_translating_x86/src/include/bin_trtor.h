
#include "config.h"

#include <stdio.h>

//-----------------------------------------------------------------------------

struct Command
{
    CMD cmd;

    int    reg_num;
    double val;
};

struct BinTrtor
{
    char*  bin_code_x86;
    size_t bin_code_x86_size;
    
    const char* bin_code;
    size_t      bin_code_size;

    size_t num_cmds;

    Command* commands;
};

//-----------------------------------------------------------------------------

int BinTrtorCtor( BinTrtor* bin_trtor, const char* bin_code );
int BinTrtorDtor( BinTrtor* bin_trtor );

int BinTrtorParseBinCode( BinTrtor* bin_trtor );

int BinTrtorToX86( BinTrtor* bin_trtor );

int BinTrtorRun( BinTrtor* bin_trtor );

//-----------------------------------------------------------------------------

int CheckBinCodeSignature( const char* bin_code );

extern "C" int _printf( const char* str, ... ); 

//-----------------------------------------------------------------------------