#ifndef DSL_H
#define DSL_H

// Right/Left exists
#define IS_R_EXISTS ( node->right == NULL ? 0 : 1 )
#define IS_L_EXISTS ( node->left  == NULL ? 0 : 1 )

// Is right/left node
#define IS_R ( node->parent && node->parent->right == node )
#define IS_L ( node->parent && node->parent->left  == node )

#define IS_OP  ( node->value != NULL && node->value->type == Types::OP_TYPE  ? 1 : 0 )
#define IS_VAL ( node->value != NULL && node->value->type == Types::VAL_TYPE ? 1 : 0 )
#define IS_VAR ( node->value != NULL && node->value->type == Types::VAR_TYPE ? 1 : 0 )

// Right/Left node type
#define IS_R_VAL ( IS_R_EXISTS && node->right->value->type == Types::VAL_TYPE ? 1 : 0 )
#define IS_L_VAL ( IS_L_EXISTS && node->left ->value->type == Types::VAL_TYPE ? 1 : 0 )

#define IS_R_VAR ( IS_R_EXISTS && node->right->value->type == Types::VAR_TYPE ? 1 : 0 )
#define IS_L_VAR ( IS_L_EXISTS && node->left ->value->type == Types::VAR_TYPE ? 1 : 0 )

#define IS_R_OP ( IS_R_EXISTS && node->right->value->type == Types::OP_TYPE ? 1 : 0 )
#define IS_L_OP ( IS_L_EXISTS && node->left ->value->type == Types::OP_TYPE ? 1 : 0 )

// Right/left node dblValue
#define R_VAL node->right->value->dblValue
#define L_VAL node->left ->value->dblValue 

// Right/left node varValue
#define R_VAR node->right->value->varValue
#define L_VAR node->left ->value->varValue 

// Right/left node type
#define R_TYPE node->right->value->type
#define L_TYPE node->left ->value->type 

#define CREATE_VAL_NODE( NUM ) CreateLngNode( VAL_TYPE, NUM, -1 ) 
#define CREATE_VAR_NODE( VAR ) CreateLngNode( VAR_TYPE, 0,   -1, (char*)VAR )

#define CREATE_VAR_NODE_LR( VAR, L, R ) CreateLngNode( VAR_TYPE, 0,   -1, (char*)VAR, L, R )

#define CREATE_OP_NODE( OP, L, R ) CreateLngNode( OP_TYPE, 0, OP, NULL, L, R )

#define CREATE_TYPE_NODE( TYPE ) CreateLngNode( TYPE, 0, -1, NULL )

#define CREATE_TYPE_NODE_LR( TYPE, L, R ) CreateLngNode( TYPE, 0, -1, NULL, L, R )


#define CUR_NODE nodes[*curPos]
#define CUR_NODE_TYPE CUR_NODE->value->type
#define CUR_NODE_OP   CUR_NODE->value->opValue
#define CUR_NODE_VAR  CUR_NODE->value->varValue

#define NODE_TYPE node->value->type
#define NODE_OP   node->value->opValue
#define NODE_VAL  node->value->dblValue
#define NODE_VAR  node->value->varValue

#define PREV_TOKEN (*curPos)--;
#define NEXT_TOKEN (*curPos)++;

#define ASSERT_L_BRACE assert( CUR_NODE_TYPE == L_BRACE_TYPE ); NEXT_TOKEN; // !: 
#define ASSERT_R_BRACE assert( CUR_NODE_TYPE == R_BRACE_TYPE ); NEXT_TOKEN; // :! 

#define ASSERT_L_BRACKET assert( CUR_NODE_TYPE == L_BRACKET_TYPE ); NEXT_TOKEN; // ( 
#define ASSERT_R_BRACKET assert( CUR_NODE_TYPE == R_BRACKET_TYPE ); NEXT_TOKEN; // )

// +
#define ADD(  L, R ) CREATE_OP_NODE( OP_ADD,  L, R )
// -
#define SUB(  L, R ) CREATE_OP_NODE( OP_SUB,  L, R )
// *
#define MUL(  L, R ) CREATE_OP_NODE( OP_MUL,  L, R )
// /
#define DIV(  L, R ) CREATE_OP_NODE( OP_DIV,  L, R )
// sin
#define SIN(  L, R ) CREATE_OP_NODE( OP_SIN,  L, R )
// cos 
#define COS(  L, R ) CREATE_OP_NODE( OP_COS,  L, R )
// ^
#define POW(  L, R ) CREATE_OP_NODE( OP_DEG,  L, R )
// ln
#define LN(   L, R ) CREATE_OP_NODE( OP_LN,   L, R ) 
// sqrt
#define SQRT( L, R ) CREATE_OP_NODE( OP_SQRT, L, R ) 

#endif