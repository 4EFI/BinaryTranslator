
// ASM & CPU DSL
//-----------------------------------------------------------------------------

#define __S_POP         StackPop ( &cpu->stack )
#define   S_PUSH( VAL ) StackPush( &cpu->stack, VAL )

#define S_RET_POP         StackPop ( &cpu->stkRetIP )
#define S_RET_PUSH( VAL ) StackPush( &cpu->stkRetIP, VAL )

#define S_POP( NUM )                            \
    Elem_t val_##NUM = __S_POP;                 \
                                                \
    if( val_##NUM == StackDataPoisonValue )     \
    {                                           \
        printf( "Poisonous stack value...\n" ); \
        return -1;                              \
    }

#define S_POP_VALUES \
    S_POP( 2 )       \
    S_POP( 1 )

//-----------------------------------------------------------------------------

#define ASM_PUT_CODE( VAL )                                     \
{                                                               \
    Elem_t __val_temp = VAL;                                    \
    memcpy( asm_s->code + *ip, &__val_temp, sizeof( Elem_t ) ); \
    (*ip) += sizeof( Elem_t );                                  \
} 

//-----------------------------------------------------------------------------


//  BinTrtor DSL
//-----------------------------------------------------------------------------

#define PP( NUM ) bin_code_x86_ptr += ( NUM );

#define BIN_PRINT( SIZE, ... )                                      \
    BinPrint( bin_code_x86_ptr, SIZE, __VA_ARGS__ ); PP( SIZE );

#define BIN_TRTOR_CMD( i ) bin_trtor->commands[ i ]

#ifndef NDEBUG 
    #define NOP BIN_PRINT( 1, 0x90 );
#else
    #define NOP ;
#endif

#define VAL_TO_BIN_CODE_X86( VAL_PTR, SIZE )                \
    memcpy( bin_code_x86_ptr, VAL_PTR, SIZE ); PP( SIZE );

#define PTR_TO_BIN_CODE_X86( PTR )                          \
{                                                           \
    u_int64_t __val_ptr = ( u_int64_t )( PTR );             \
    memcpy( bin_code_x86_ptr, &__val_ptr, 8 ); PP( 8 );     \
}


// add rsp, NUM
#define PP_RSP( NUM ) BIN_PRINT( 4, 0x48, 0x83, 0xc4, NUM );        

// sub rsp, NUM
#define MM_RSP( NUM ) BIN_PRINT( 4, 0x48, 0x83, 0xec, NUM );                         

// movlps xmm0, [rsp] 
#define LOAD_XMM0_FROM_S()                  \
    BIN_PRINT( 4, 0x0f, 0x12, 0x04, 0x24 );

// movlps xmm1, [rsp] 
#define LOAD_XMM1_FROM_S()                  \
    BIN_PRINT( 4, 0x0f, 0x12, 0x0c, 0x24 );

// movlps [rsp], xmm0 
#define LOAD_S_FROM_XMM0()                  \
    BIN_PRINT( 4, 0x0f, 0x13, 0x04, 0x24 );      

// movq xmm0, r_x
#define LOAD_XMM0_FROM_RX( REG_NUM )                        \
    BIN_PRINT( 5, 0x66, 0x48, 0x0f, 0x6e, 0xc0 + REG_NUM );    

// cvttsd2si r10, xmm0
#define CVT_XMM0_TO_INT()                           \
    BIN_PRINT( 5, 0xf2, 0x4c, 0x0f, 0x2c, 0xd0 );                     

// push r10
#define PUSH_R10()   BIN_PRINT( 2, 0x41, 0x52 );

// push qword [r10]
#define PUSH_R10_M() BIN_PRINT( 3, 0x41, 0xff, 0x32 );

// push r_x
#define PUSH_RX( REG_NUM )   BIN_PRINT( 1, 0x50 + REG_NUM ); 

// push qword [r_x]
#define PUSH_RX_M( REG_NUM ) BIN_PRINT( 2, 0xff, 0x30 + REG_NUM ); 

// pop r_x
#define POP_RX( REG_NUM )    BIN_PRINT( 1, 0x58 + REG_NUM ); 

// mov r10, PTR
#define MOV_R10_PTR( PTR )      \
    BIN_PRINT( 2, 0x49, 0xba ); \
    PTR_TO_BIN_CODE_X86( PTR );

// mov r10, VAL
#define MOV_R10_VAL( VAL_PTR, SIZEOF )      \
    BIN_PRINT( 2, 0x49, 0xba );             \
    VAL_TO_BIN_CODE_X86( VAL_PTR, SIZEOF );

// cmpsd xmm0, xmm1, TYPE
#define CMPSD_XMM0_XMM1( TYPE )                     \
    BIN_PRINT( 5, 0xf2, 0x0f, 0xc2, 0xc1, TYPE );

#define ADD_JMP()                                                                                           \
    jmps_arr[ curr_jmp ].jmp_val_ptr =  bin_code_x86_ptr;                                                   \
    jmps_arr[ curr_jmp ].jmp_val     = ( u_int32_t* )( &bin_trtor->commands[ cmd_num ].bin_code_x86_ptr );  \
    curr_jmp++; 

//-----------------------------------------------------------------------------
