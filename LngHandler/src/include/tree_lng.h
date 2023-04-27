#ifndef TREE_LNG_H
#define TREE_LNG_H

#include "config.h"

#include <stdio.h>

static const TreeElem_t NODE_POISON = ( TreeElem_t )0x5E32DEAD;

enum NodeSides
{
    LEFT_SIDE  = -1,
    RIGHT_SIDE =  1
};

//-----------------------------------------------------------------------------

struct Node
{
    Node* parent;

    int side;
    
    TreeElem_t value;

    Node* left;
    Node* right;
};

int NodeCtor( Node* node );
int NodeDtor( Node* node );

Node* CreateLngNode( int type, double dbl, int op, char* var = NULL, Node* left = NULL, Node* right = NULL, Node* parent = NULL );

Node* CopyLngNode( const Node* node );

int LinkNodeParents( Node* node, Node* parent );

Node* TreeSetNodeValue( Node* node, TreeElem_t val );
Node* TreeAddChild    ( Node* node, TreeElem_t val, int side );

Node* TreeSearch( Node* nodeBegin, TreeElem_t val );

//-----------------------------------------------------------------------------

int CreateGraphVizImg( const char* dotFileName, const char* fileName, const char* fileType );

//-----------------------------------------------------------------------------

#endif