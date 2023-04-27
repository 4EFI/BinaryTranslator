#ifndef CPU_H
#define CPU_H

#include "config.h"
#include "stack.h"

//-----------------------------------------------------------------------------

struct CPU
{
    Elem_t RAM [RamSize];
    Elem_t regs[NumRegs];
    
    Stack stack; 
    Stack stkRetIP;
    
    int   codeSize;
    char* code;
};

int     CpuCtor       ( CPU* cpu );
int     CpuGetCmdsArr ( CPU* cpu, FILE* file );
int     CpuCmdsHandler( CPU* cpu );
Elem_t* CpuGetArg     ( CPU* cpu, int* ip, Elem_t* val );

int     CpuCmdDump    ( CPU* cpu, int ip, FILE* file );
int     CpuRamDump    ( CPU* cpu,         FILE* file );
int     CpuRegDump    ( CPU* cpu,         FILE* file );

int OutRam( CPU* cpu, int numSymsPerLine );

#ifdef NDUMP
    СpuDump( cpu ) ;
#endif

//-----------------------------------------------------------------------------

int CheckCompatibility( FILE* file );

int PrintArr( Elem_t* arr, int arrSize, FILE* file );

bool ClearConsole();

//-----------------------------------------------------------------------------

#endif