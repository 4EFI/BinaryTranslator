
#include "dsl.h"

//-----------------------------------------------------------------------------

DEF_CMD( HLT, 0, 
{
    return 0;
})

//-----------------------------------------------------------------------------

DEF_CMD( PUSH, 1, 
{
    S_PUSH( arg_val );
})

//-----------------------------------------------------------------------------

DEF_CMD( ADD, 2, 
{
    S_POP_VALUES
    S_PUSH( val_1 + val_2 );
})

//-----------------------------------------------------------------------------

DEF_CMD( SUB, 3, 
{ 
    S_POP_VALUES
    S_PUSH( val_1 - val_2 );
})

//-----------------------------------------------------------------------------

DEF_CMD( MUL, 4, 
{
    S_POP_VALUES
    S_PUSH( val_1 * val_2 );
})

//-----------------------------------------------------------------------------

DEF_CMD( DIV, 5, 
{
    S_POP_VALUES          
    S_PUSH( val_1 / val_2 );
})

//-----------------------------------------------------------------------------

DEF_CMD( OUT, 6, 
{
    if( arg_ptr )
    {
        if/* */( cmd.memory ) printf( "RAM[%d] = %g\n",  arg_ptr - cpu->RAM,  (double)(*arg_ptr) );
        else if( cmd.reg    ) printf( "Regs[%d] = %g\n", arg_ptr - cpu->regs, (double)(*arg_ptr) );
    }
    else
    {
        S_POP( 1 )
        if( CompareDoubles( val_1, 0 ) ) 
        {
            val_1 = 0;
        }

        printf( "%lf\n", double( val_1 ) );
    }
})

//-----------------------------------------------------------------------------

DEF_CMD( POP, 7, 
{ 
    S_POP( 1 )
    *arg_ptr = val_1;
})

//-----------------------------------------------------------------------------

// Jumps
DEF_CMD( JMP, 8, 
{
    ip = arg_val;
})

//-----------------------------------------------------------------------------

#define DEF_JMP( NAME, NUM, UNUSED_0, UNUSED_1, COND, UNUSED_2 )    \
    DEF_CMD( NAME, NUM,                                             \
    {                                                               \
        S_POP_VALUES                                                \
        if( val_1 COND val_2 ) ip = int(arg_val);                   \
    })

#include "jumps.h"

#undef DEF_JMP

//-----------------------------------------------------------------------------

DEF_CMD( IN, 15, 
{
    Elem_t val = 0;
    scanf( "%lf", &val );
    
    S_PUSH( val );
})

//-----------------------------------------------------------------------------

DEF_CMD( SQRT, 16, 
{
    S_POP( 1 )
    S_PUSH( sqrt( val_1 ) );
})

//-----------------------------------------------------------------------------

DEF_CMD( CALL, 17, 
{
    S_RET_PUSH( ip );
    ip = arg_val;
})

//-----------------------------------------------------------------------------

DEF_CMD( RET, 18, 
{
    ip = S_RET_POP;
})

//-----------------------------------------------------------------------------

DEF_CMD( SIN, 19, 
{
    S_POP( 1 );
    S_PUSH( sin( val_1 ) );
})

//-----------------------------------------------------------------------------

DEF_CMD( COS, 20, 
{
    S_POP( 1 );
    S_PUSH( cos( val_1 ) );
})

//-----------------------------------------------------------------------------

DEF_CMD( POW, 21, 
{
    S_POP_VALUES          
    S_PUSH( pow( val_1, val_2 ) );   
})

//-----------------------------------------------------------------------------

// CMP: is_ee, is_ne, etc...

#define DEF_JMP( UNUSED_0, UNUSED_1, NAME, NUM, COND, UNUSED_2 )    \
    DEF_CMD( NAME, NUM,                                             \
    {                                                               \
        S_POP_VALUES                                                \
        S_PUSH( val_1 COND val_2 );                                 \
    })

#include "jumps.h"

#undef DEF_JMP

//-----------------------------------------------------------------------------

DEF_CMD( DUMP, 31, 
{
    if( LogFile ) StackDump( &cpu->stack );

    CpuCmdDump( cpu, ip, LogFile );
    CpuRegDump( cpu,     LogFile );

    fprintf( LogFile, "\n" );

    CpuRamDump( cpu, LogFile );

    PrintSyms( '-', NumDumpDividers, LogFile );
    fprintf  ( LogFile, "\n" );
})

//-----------------------------------------------------------------------------

#undef S_POP
#undef S_PUSH
