
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

#define REG_PLUS_VALUE_TO_XMM0()                            \
{                                                           \
    if( cmd->reg )                                          \
    {                                                       \
        /* movq xmm1, r10 */                                \
        BIN_PRINT( 5, 0x66, 0x49, 0x0f, 0x6e, 0xca );       \
                                                            \
        LOAD_XMM0_FROM_RX( BIN_TRTOR_CMD( i ).reg_num );    \
                                                            \
        /* addsd xmm0, xmm1 */                              \
        BIN_PRINT( 4, 0xf2, 0x0f, 0x58, 0xc1 );             \
    }                                                       \
    else if( cmd->memory )                                  \
    {                                                       \
        /* movq xmm0, r10 */                                \
        BIN_PRINT( 5, 0x66, 0x49, 0x0f, 0x6e, 0xc2 );       \
    }                                                       \
}

#define PUSH()                                              \
{                                                           \
    if( cmd->memory )                                       \
    {                                                       \
        CVT_XMM0_TO_INT();                                  \
        PUSH_R10_M();                                       \
    }                                                       \
    else                                                    \
    {                                                       \
        if( cmd->reg && cmd->immed )                        \
        {                                                   \
            /* movq r10, xmm0 */                            \
            BIN_PRINT( 5, 0x66, 0x49, 0x0f, 0x7e, 0xc2 );   \
        }                                                   \
                                                            \
        PUSH_R10();                                         \
    }                                                       \
}

DEF_CMD( PUSH, 1, 
{
#ifndef BT
{
    S_PUSH( arg_val );
}
#else
{    
    NOP 

    if( cmd->immed || cmd->memory )
    {
        double val = 0;
        if( cmd->immed  ) val += BIN_TRTOR_CMD( i ).val;
        if( cmd->memory ) val += double( ( u_int64_t )( bin_trtor->RAM ) );
        
        MOV_R10_VAL( &val, sizeof( double ) );
    
        REG_PLUS_VALUE_TO_XMM0();
        
        PUSH();
    }
    else
    {        
        PUSH_RX( BIN_TRTOR_CMD( i ).reg_num );
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
    NOP 

    if( cmd->immed || cmd->memory )
    {
        double           val  = double( ( u_int64_t )( bin_trtor->RAM ) );
        if( cmd->immed ) val += BIN_TRTOR_CMD( i ).val;
        
        MOV_R10_VAL( &val, sizeof( double ) );
    
        REG_PLUS_VALUE_TO_XMM0();
        
        CVT_XMM0_TO_INT();

        // pop qword [r10]
        BIN_PRINT( 3, 0x41, 0x8f, 0x02 );
    }
    else
    {        
        POP_RX( BIN_TRTOR_CMD( i ).reg_num );  
    }

    NOP
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

    size_t cmd_num = FindLabelCommand( bin_trtor, int( BIN_TRTOR_CMD( i ).val ) ); 

    // jmp ... 
    BIN_PRINT( 1, 0xe9 );
    
    ADD_JMP(); PP( 4 );

    NOP
}
#endif 
})

//-----------------------------------------------------------------------------

#ifndef BT

#define DEF_JMP( NAME, NUM, UNUSED_0, UNUSED_1, COND, UNUSED_2 )    \
    DEF_CMD( NAME, NUM,                                             \
    {                                                               \
        S_POP_VALUES                                                \
        if( val_1 COND val_2 ) ip = int(arg_val);                   \
    })

#else

#define DEF_JMP( NAME, NUM, UNUSED_0, UNUSED_1, COND, CMP_TYPE )                        \
    DEF_CMD( NAME, NUM,                                                                 \
    {                                                                                   \
        NOP                                                                             \
                                                                                        \
        LOAD_XMM1_FROM_S(); PP_RSP( 8 );                                                \
        LOAD_XMM0_FROM_S(); PP_RSP( 8 );                                                \
                                                                                        \
        CMPSD_XMM0_XMM1( CMP_TYPE );                                                    \
                                                                                        \
        /* movq r10, xmm0 */                                                            \
        BIN_PRINT( 5, 0x66, 0x49, 0x0f, 0x7e, 0xc2 );                                   \
                                                                                        \
        size_t cmd_num = FindLabelCommand( bin_trtor, int( BIN_TRTOR_CMD( i ).val ) );  \
                                                                                        \
        /* cmp r10, 0 */                                                                \
        BIN_PRINT( 4, 0x49, 0x83, 0xfa, 0x00 );                                         \
        /* jne ... */                                                                   \
        BIN_PRINT( 2, 0x0f, 0x85 /* ... */ );                                           \
                                                                                        \
        ADD_JMP(); PP( 4 );                                                             \
                                                                                        \
        NOP                                                                             \
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
    NOP
    
    LOAD_XMM1_FROM_S();

    // sqrtsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x51, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
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
    NOP

    size_t cmd_num = FindLabelCommand( bin_trtor, int( BIN_TRTOR_CMD( i ).val ) );

    PP_RBP( 8 );

    // mov r10, ret_ptr
    MOV_R10_PTR( bin_code_x86_ptr + 12 /*movs code size*/ + 5 /*jmp code size*/ );

    // mov qword [rbp], r10
    BIN_PRINT( 4, 0x4c, 0x89, 0x55, 0x00 ); 

    // jmp ... 
    BIN_PRINT( 1, 0xe9 );
    
    ADD_JMP(); PP( 4 );

    NOP
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
    
    // push qword [rbp] ; push ret_addr
    BIN_PRINT( 3, 0xff, 0x75, 0x00 ); MM_RBP( 8 );
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

// CMP: is_ee, is_ne, etc...
#ifndef BT

#define DEF_JMP( UNUSED_0, UNUSED_1, NAME, NUM, COND, UNUSED_2 )    \
    DEF_CMD( NAME, NUM,                                             \
    {                                                               \
        S_POP_VALUES                                                \
        S_PUSH( val_1 COND val_2 );                                 \
    })

#else
    
#define DEF_JMP( UNUSED_0, UNUSED_1, NAME, NUM, COND, CMP_TYPE )    \
    DEF_CMD( NAME, NUM,                                             \
    {                                                               \
        NOP                                                         \
                                                                    \
        LOAD_XMM1_FROM_S(); PP_RSP( 8 );                            \
        LOAD_XMM0_FROM_S();                                         \
                                                                    \
        CMPSD_XMM0_XMM1( CMP_TYPE );                                \
                                                                    \
        LOAD_S_FROM_XMM0();                                         \
                                                                    \
        NOP                                                         \
    })  

#endif

#include "jumps.h"

#undef DEF_JMP

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
