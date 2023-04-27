

#include "config.h"

#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "tree_lng_parsing.h"
#include "lng_tools.h"
#include "dsl.h"
#include "my_assert.h"
#include "LOG.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

int SetLngNode( Node* node, const char* diffData )
{
    ASSERT( node     != NULL, 0 );
    ASSERT( diffData != NULL, 0 );
    
    int numReadSyms = 0;

    double num = 0;
    int isNum = sscanf( diffData, " %lf%n ", &num, &numReadSyms );

    if( isNum )
    {        
        node->value->type     = Types::VAL_TYPE;
        node->value->dblValue = num;
    }
    else
    {
        char* tempStr = ( char* )calloc( MaxStrLen, sizeof( char ) );

        sscanf( diffData, " \"%[^\"]%n ", tempStr, &numReadSyms );

        if( numReadSyms ) // If variable
        {
            node->value->type     = Types::VAR_TYPE;
            node->value->varValue = tempStr;  
            
            return numReadSyms + 1; // +1 for end \" symbol
        }
    
        sscanf( diffData, " %s%n ", tempStr, &numReadSyms ); 

        int opType = -1;
        int type   = GetType( tempStr, &opType, STANDART ); 

        if( opType != -1 )
        {
            node->value->type    = Types::OP_TYPE;
            node->value->opValue = opType;   
        }
        else
        {  
            if( !strcmp( tempStr, "NIL" ) ) 
            {
                if( IS_L ) node->parent->left  = NULL;
                if( IS_R ) node->parent->right = NULL;
            }
            else
            {
                node->value->type = type;
            }       
        }   
    }

    return numReadSyms; 
}

Node* LoadLngTree( const char* lngData ) 
{
    ASSERT( lngData != NULL, NULL );

    Node* currNode = CREATE_VAL_NODE( 0 );
    Node* node     = currNode;

    int len = strlen( lngData );
    for( int i = 0; i < len; i++ )
    {
        if( lngData[i] == ' ' ) continue;

        if ( lngData[i] == '{' )
        {
            if( !currNode->left )
            {
                currNode = TreeAddChild( currNode, NULL, LEFT_SIDE );
                continue;
            }
            else
            {
                currNode = TreeAddChild( currNode, NULL, RIGHT_SIDE );
                continue;
            }
        }

        if( lngData[i] == '}' )
        {
            currNode = currNode->parent;
            continue;
        }

        i += SetLngNode( currNode, lngData + i );
        i --; 
    }

    LinkNodeParents( node, NULL );

    return currNode->left;
}

//-----------------------------------------------------------------------------