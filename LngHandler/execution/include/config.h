#ifndef CONFIG_H
#define CONFIG_H

//-----------------------------------------------------------------------------

static int PageSize = 4096;

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
    RAX = 0,
    RCX = 1, 
    RDX = 2,
    RBX = 3
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

static const char Signature[] = "SP"; // Super paper
static const char Version     = 1;

static int SignatureBlockSize = 9;

#define DEF_CMD( NAME, NUM, ... ) \
    CMD_##NAME = NUM, 

enum CmdNames
{
    #include "commands.h"
};

#undef DEF_CMD

//-----------------------------------------------------------------------------

#endif