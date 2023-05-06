
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

#define PP( NUM )               \
    bin_code_x86_ptr += ( NUM );

#define BIN_TRTOR_CMD( i ) bin_trtor->commands[ i ]


#define VAL_TO_BIN_CODE_X86( VAL_PTR, SIZE )                \
    memcpy( bin_code_x86_ptr, VAL_PTR, SIZE ); PP( SIZE );

#define PTR_TO_BIN_CODE_X86( PTR )                          \
{                                                           \
    u_int64_t __val_ptr = ( u_int64_t )( PTR );             \
    memcpy( bin_code_x86_ptr, &__val_ptr, 8 ); PP( 8 );     \
}


#define PP_SP( NUM )                                                            \
    sprintf( bin_code_x86_ptr, "%c%c%c%c", 0x48, 0x83, 0xc4, NUM ); PP( 4 );

#define MM_SP( NUM )                                                            \
    sprintf( bin_code_x86_ptr, "%c%c%c%c", 0x48, 0x83, 0xec, NUM ); PP( 4 );

#define MOV_LPS_XMM_N( N )                                                      \
    sprintf( bin_code_x86_ptr, "%c%c%c%c", 0x0f, 0x12, 0x04, 0x24 ); PP( 4 );

//-----------------------------------------------------------------------------
