
#include <stdio.h>

//-----------------------------------------------------------------------------

enum RegNum
{
    R0X = 0,
    RAX = 1,
    RBX = 2,
    RCX = 3, 
    RDX = 4
};

struct CMD
{
    unsigned char code : 5;
    unsigned char imm  : 1; 
    unsigned char reg  : 1; 
    unsigned char mem  : 1;
};

struct Command
{
    CMD cmd;

    int    reg_num;
    double val;
};

struct BinTrtor
{
    char* bin_code;
    
    size_t max_num_cmds;
    size_t num_cmds;

    Command* commands;
};

//-----------------------------------------------------------------------------

int BinTrtorCtor( BinTrtor* bin_trtor, const char* bin_code );
int BinTrtorDtor( BinTrtor* bin_trtor );

//-----------------------------------------------------------------------------