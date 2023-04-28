
#include <stdio.h>

//-----------------------------------------------------------------------------

struct CMD
{
    unsigned char code   : 5;
    unsigned char imm    : 1; 
    unsigned char reg    : 1; 
    unsigned char memory : 1;
};

struct Command
{
    CMD cmd;
};

struct BinTrtor
{
    size_t max_num_cmds;
    size_t num_cmds;

    Command* commands;
};

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------