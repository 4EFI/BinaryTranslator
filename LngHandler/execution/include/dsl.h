#ifndef DSL_H
#define DSL_h

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
    
#endif 