#ifndef CONFIG_H
#define CONFIG_H

//-----------------------------------------------------------------------------

static const int NumRegs       = 5;
static const int NumLabels     = 1000;

static const int RamWidthSize  = 100;
static const int RamHeightSize = 100; 
static const int RamSize       = RamWidthSize * RamHeightSize;

#define MaxStrLen 255 

static int NumDumpDividers = 65;

//-----------------------------------------------------------------------------

enum Reg_i
{
    R0X = 0,
    RAX = 1,
    RBX = 2,
    RCX = 3, 
    RDX = 4
};

struct CMD
{
    unsigned char code   : 5;
    unsigned char immed  : 1; // imm
    unsigned char reg    : 1; 
    unsigned char memory : 1;
};

// #define ON_LOG_FILE
// #define NHASH
// #define NCANARY

typedef double Elem_t;

static char Signature[] = "SP"; // Super paper
static int  Version     = 1;

static int  SignatureBlockSize = 10;

#define DEF_CMD( NAME, NUM, ... ) \
    CMD_##NAME = NUM, 

enum CmdNames
{
    #include "commands.h"
};

#undef DEF_CMD

//-----------------------------------------------------------------------------

#endif