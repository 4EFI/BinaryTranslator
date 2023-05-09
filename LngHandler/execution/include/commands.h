
#include "dsl.h"

//-----------------------------------------------------------------------------

DEF_CMD( HLT, 0,
{
#ifndef BT
{
    return 0;
}
#else
{
    NOP
    
    // mov eax, 0x3c
    BIN_PRINT( 5, 0xb8, 0x3c, 0x00, 0x00, 0x00 );
    // xor rdi, rdi
    BIN_PRINT( 3, 0x48, 0x31, 0xff );
    // syscall 
    BIN_PRINT( 2, 0x0f, 0x05 );

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( PUSH, 1, 
{
#ifndef BT
{
    S_PUSH( arg_val );
}
#else
{   
    NOP

    if( cmd->immed )
    {
        MOV_R10_VAL( &BIN_TRTOR_CMD( i ).val, sizeof( double ) );
        PUSH_R10();
    }

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( ADD, 2, 
{
#ifndef BT
{
    S_POP_VALUES
    S_PUSH( val_1 + val_2 );
}
#else
{
    NOP

    LOAD_XMM1_FROM_S(); PP_RSP( 8 );
    LOAD_XMM0_FROM_S();

    // addsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x58, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( SUB, 3, 
{ 
#ifndef BT
{
    S_POP_VALUES
    S_PUSH( val_1 - val_2 );
}
#else
{
    NOP
    
    LOAD_XMM1_FROM_S(); PP_RSP( 8 );
    LOAD_XMM0_FROM_S();

    // subsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x5c, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( MUL, 4, 
{
#ifndef BT
{
    S_POP_VALUES
    S_PUSH( val_1 * val_2 );
}
#else
{
    NOP
    
    LOAD_XMM1_FROM_S(); PP_RSP( 8 );
    LOAD_XMM0_FROM_S();

    // mulsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x59, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( DIV, 5, 
{
#ifndef BT
{
    S_POP_VALUES          
    S_PUSH( val_1 / val_2 );
}
#else
{
    NOP
    
    LOAD_XMM1_FROM_S(); PP_RSP( 8 );
    LOAD_XMM0_FROM_S();

    // divsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x5e, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( OUT, 6, 
{
#ifndef BT
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
}
#else
{
    NOP
    
    const char* str = "%d\n";

    LOAD_XMM0_FROM_S(); PP_RSP( 8 );
    CVT_XMM0_TO_INT();  
    PUSH_R10();

    MOV_R10_PTR( str );
    PUSH_R10();

    MOV_R10_PTR( _printf );

    // call r10
    BIN_PRINT( 3, 0x41, 0xff, 0xd2 ); PP_RSP( 16 );

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( POP, 7, 
{ 
#ifndef BT
{
    S_POP( 1 )
    *arg_ptr = val_1;
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

// Jumps
DEF_CMD( JMP, 8, 
{
#ifndef BT
{
    ip = arg_val;
}
#else
{
    NOP

    double val = 0;
    MOV_R10_VAL( &val, sizeof( double ) );
    
    size_t cmd_num = FindLabelCommand( bin_trtor, BIN_TRTOR_CMD( i ).val ); 

    bin_trtor->commands[ cmd_num ].jmp_x86_val_ptr = ( bin_code_x86_ptr - sizeof( double ) );

    // jmp r10
    BIN_PRINT( 3, 0x41, 0xff, 0xe2 );

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

#ifndef BT

#define DEF_JMP( NAME, NUM, COND )                  \
    DEF_CMD( NAME, NUM,                             \
    {                                               \
        S_POP_VALUES                                \
        if( val_1 COND val_2 ) ip = int(arg_val);   \
    })

#else

#define DEF_JMP( NAME, NUM, COND )                                              \
    DEF_CMD( NAME, NUM,                                                         \
    {                                                                           \
        NOP                                                                     \
                                                                                \
        LOAD_XMM1_FROM_S(); PP_RSP( 8 );                                        \
        LOAD_XMM0_FROM_S(); PP_RSP( 8 );                                        \
                                                                                \
        CMPSD_XMM0_XMM1( 0 );                                                   \
                                                                                \
        /* movq r10, xmm0 */                                                    \
        BIN_PRINT( 5, 0x66, 0x49, 0x0f, 0x7e, 0xc2 );                           \
                                                                                \
        size_t cmd_num = FindLabelCommand( bin_trtor, BIN_TRTOR_CMD( i ).val ); \
                                                                                \
        /* cmp r10, 0 */                                                        \
        BIN_PRINT( 4, 0x49, 0x83, 0xfa, 0x00 );                                 \
        /* jne ... */                                                           \
        BIN_PRINT( 2, 0x0f, 0x85 /* ... */ );                                   \
                                                                                \
        bin_trtor->commands[ cmd_num ].jmp_x86_val_ptr = ( bin_code_x86_ptr );  \
        PP( 4 );                                                                \
                                                                                \
        NOP                                                                     \
    })                                  

#endif

#include "jumps.h"

#undef DEF_JMP

//-----------------------------------------------------------------------------

DEF_CMD( IN, 15, 
{
#ifndef BT
{
    Elem_t val = 0;
    scanf( "%lf", &val );
    
    S_PUSH( val );
}
#else
{
    //
} 
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( SQRT, 16, 
{
#ifndef BT
{
    S_POP( 1 )
    S_PUSH( sqrt( val_1 ) );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( CALL, 17, 
{
#ifndef BT
{
    S_RET_PUSH( ip );
    ip = arg_val;
}
#else
{
    // BIN_PRINT(  );
}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( RET, 18, 
{
#ifndef BT
{
    ip = S_RET_POP;
}
#else
{
    NOP
    
    // ret
    BIN_PRINT( 1, 0xC3 );

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( SIN, 19, 
{
#ifndef BT
{
    S_POP( 1 );
    S_PUSH( sin( val_1 ) );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( COS, 20, 
{
#ifndef BT
{
    S_POP( 1 );
    S_PUSH( cos( val_1 ) );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( POW, 21, 
{
#ifndef BT
{
    S_POP_VALUES          
    S_PUSH( pow( val_1, val_2 ) );
}
#else
{
        
}   
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( IS_EE, 22, 
{
#ifndef BT
{
    S_POP_VALUES          
    S_PUSH( val_1 == val_2 );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( IS_GE, 23, 
{
#ifndef BT
{
    S_POP_VALUES          
    S_PUSH( val_1 >= val_2 );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( IS_BE, 24, 
{
#ifndef BT
{
    S_POP_VALUES          
    S_PUSH( val_1 <= val_2 );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( IS_GT, 25, 
{
#ifndef BT
{ 
    S_POP_VALUES          
    S_PUSH( val_1 > val_2 );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( IS_BT, 26, 
{
#ifndef BT
{
    S_POP_VALUES          
    S_PUSH( val_1 < val_2 );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( IS_NE, 27, 
{
#ifndef BT
{
    S_POP_VALUES          
    S_PUSH( val_1 != val_2 );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

DEF_CMD( DUMP, 31, 
{
#ifndef BT
{ 
     if( LogFile ) StackDump( &cpu->stack );

    CpuCmdDump( cpu, ip, LogFile );
    CpuRegDump( cpu,     LogFile );

    fprintf( LogFile, "\n" );

    CpuRamDump( cpu, LogFile );

    PrintSyms( '-', NumDumpDividers, LogFile );
    fprintf  ( LogFile, "\n" );
}
#else
{

}
#endif 
})

//-----------------------------------------------------------------------------

#undef S_POP
#undef S_PUSH
