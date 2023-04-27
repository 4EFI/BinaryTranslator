
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree_lng.h"
#include "my_assert.h"
#include "LOG.h"

#define FREE( ptr ) \
    free( ptr );    \
    ptr = NULL;

//  Node functions
//-----------------------------------------------------------------------------

int NodeCtor( Node* node )
{
    ASSERT( node != NULL, 0 );

    node->value = ( LngNode* )calloc( 1, sizeof( LngNode ) );

    node->parent = NULL;

    node->left  = NULL;
    node->right = NULL;

    return 1;
}  

//-----------------------------------------------------------------------------

Node* CreateLngNode( int type, double dbl, int op, char* var, Node* left, Node* right, Node* parent )
{    
    Node* newNode = ( Node* )calloc( 1, sizeof( Node ) );

    newNode->value           = ( LngNode* )calloc( 1, sizeof( LngNode ) );    
    newNode->value->type     = type;
    newNode->value->dblValue = dbl;
    newNode->value->opValue  = op;
    newNode->value->varValue = var;

    newNode->parent = parent;
    newNode->left   = left;
    newNode->right  = right;

    return newNode;
}

Node* CopyLngNode( const Node* node )
{
    ASSERT( node != NULL, 0 );

    Node* newNode = ( Node* )calloc( 1, sizeof( Node ) );

    newNode->value = ( LngNode* )calloc( 1, sizeof( LngNode ) );

    memmove( newNode,        node,        sizeof(    Node ) );
    memmove( newNode->value, node->value, sizeof( LngNode ) );

    if( node->left )
    {
        newNode->left = CopyLngNode( node->left );
        newNode->left->parent = newNode; 
    }

    if( node->right )
    {
        newNode->right = CopyLngNode( node->right );
        newNode->right->parent = newNode; 
    }

    return newNode;
}

//-----------------------------------------------------------------------------

int LinkNodeParents( Node* node, Node* parent )
{
    ASSERT( node != NULL, 0 );

    node->parent = parent;

    if( node->left )
    {
        LinkNodeParents( node->left, node );
    }
    
    if( node->right )
    {
        LinkNodeParents( node->right, node );
    }

    return 1; 
}

//-----------------------------------------------------------------------------

int NodeDtor( Node* node )
{
    ASSERT( node != NULL, 0 );
    
    node->value = NODE_POISON;
    
    FREE( node->left  );
    FREE( node->right );

    return 1;
}

//-----------------------------------------------------------------------------


//  Tree functions
//-----------------------------------------------------------------------------

Node* TreeSetNodeValue( Node* node, TreeElem_t val )
{
    ASSERT( node != NULL, NULL );

    node->value = val;

    return node;
}

//-----------------------------------------------------------------------------

Node* TreeAddChild( Node* node, TreeElem_t val, int side )
{
    ASSERT( node != NULL, NULL );

    Node*     newNode = ( Node* )calloc( 1, sizeof( Node ) );
    NodeCtor( newNode );

    if( !val ) 
    {
        newNode->value = ( LngNode* )calloc( 1, sizeof( LngNode ) );
        newNode->value->type = -1;
    }
    else
    {
        newNode->value = val;
    }

    newNode->parent = node;
    newNode->side   = side;

    if/* */( side == NodeSides::LEFT_SIDE )
    {
        node->left = newNode;
    }
    else if( side == NodeSides::RIGHT_SIDE )
    {
        node->right = newNode;
    }

    return newNode;
}

//-----------------------------------------------------------------------------

Node* TreeSearch( Node* nodeBegin, TreeElem_t val )
{
    ASSERT( nodeBegin != NULL, 0 );

    Node* leftNode  = NULL;
    Node* rightNode = NULL;

    if( nodeBegin->left )
    {
        leftNode  = TreeSearch( nodeBegin->left,  val );
    }
    if( nodeBegin->right )
    {
        rightNode = TreeSearch( nodeBegin->right, val );
    }

    if( nodeBegin->value, val ) 
    {
        return nodeBegin;
    }
    else if( leftNode  ) return leftNode;
    else if( rightNode ) return rightNode;

    return NULL;
}

//-----------------------------------------------------------------------------


//  Other functions
//-----------------------------------------------------------------------------

int CreateGraphVizImg( const char* dotFileName, const char* fileName, const char* fileType )
{
    if(dotFileName == NULL || fileName == NULL || fileType == NULL) return 0; 

    char cmd[MaxStrLen] = "";

    sprintf( cmd, "dot %s -T %s -o %s", dotFileName, fileType, fileName );
    system(  cmd  );

    return 1;
}

//-----------------------------------------------------------------------------
