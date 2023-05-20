
#include "dsl.h"

//-----------------------------------------------------------------------------

DEF_CMD( HLT, 0, 
{
    NOP
    
    // mov eax, 0x3c
    BIN_PRINT( 5, 0xb8, 0x3c, 0x00, 0x00, 0x00 );
    // xor rdi, rdi
    BIN_PRINT( 3, 0x48, 0x31, 0xff );
    // syscall 
    BIN_PRINT( 2, 0x0f, 0x05 );

    NOP
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
})

//-----------------------------------------------------------------------------

DEF_CMD( ADD, 2, 
{
    NOP

    LOAD_XMM1_FROM_S(); PP_RSP( 8 );
    LOAD_XMM0_FROM_S();

    // addsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x58, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
})

//-----------------------------------------------------------------------------

DEF_CMD( SUB, 3, 
{ 
    NOP
    
    LOAD_XMM1_FROM_S(); PP_RSP( 8 );
    LOAD_XMM0_FROM_S();

    // subsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x5c, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
})

//-----------------------------------------------------------------------------

DEF_CMD( MUL, 4, 
{
    NOP
    
    LOAD_XMM1_FROM_S(); PP_RSP( 8 );
    LOAD_XMM0_FROM_S();

    // mulsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x59, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
})

//-----------------------------------------------------------------------------

DEF_CMD( DIV, 5, 
{
    NOP
    
    LOAD_XMM1_FROM_S(); PP_RSP( 8 );
    LOAD_XMM0_FROM_S();

    // divsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x5e, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
})

//-----------------------------------------------------------------------------

DEF_CMD( OUT, 6, 
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
})

//-----------------------------------------------------------------------------

DEF_CMD( POP, 7, 
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
})

//-----------------------------------------------------------------------------

// Jumps
DEF_CMD( JMP, 8, 
{
    NOP

    size_t cmd_num = FindLabelCommand( bin_trtor, int( BIN_TRTOR_CMD( i ).val ) ); 

    // jmp ... 
    BIN_PRINT( 1, 0xe9 );
    
    ADD_JMP(); PP( 4 );

    NOP
})

//-----------------------------------------------------------------------------

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

#include "jumps.h"

#undef DEF_JMP

//-----------------------------------------------------------------------------

DEF_CMD( IN, 15, 
{
})

//-----------------------------------------------------------------------------

DEF_CMD( SQRT, 16, 
{
    NOP
    
    LOAD_XMM1_FROM_S();

    // sqrtsd xmm0, xmm1
    BIN_PRINT( 4, 0xf2, 0x0f, 0x51, 0xc1 );

    LOAD_S_FROM_XMM0();

    NOP
})

//-----------------------------------------------------------------------------

DEF_CMD( CALL, 17, 
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
})

//-----------------------------------------------------------------------------

DEF_CMD( RET, 18, 
{
    NOP
    
    // push qword [rbp] ; push ret_addr
    BIN_PRINT( 3, 0xff, 0x75, 0x00 ); MM_RBP( 8 );
    // ret
    BIN_PRINT( 1, 0xC3 );

    NOP
})

//-----------------------------------------------------------------------------

DEF_CMD( SIN, 19, 
{
})

//-----------------------------------------------------------------------------

DEF_CMD( COS, 20, 
{
})

//-----------------------------------------------------------------------------

DEF_CMD( POW, 21, 
{
})

//-----------------------------------------------------------------------------

// CMP: is_ee, is_ne, etc...

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

#include "jumps.h"

#undef DEF_JMP

//-----------------------------------------------------------------------------

DEF_CMD( DUMP, 31, 
{
})

//-----------------------------------------------------------------------------

#undef S_POP
#undef S_PUSH
