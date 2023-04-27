
#include "tree_lng_dump.h"
#include "tree_lng.h"
#include "my_assert.h"
#include "LOG.h"
#include "lng_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> 

FILE* FileLngDump = fopen( FileLngDumpName, "w" );

// DiffDump
//-----------------------------------------------------------------------------

int PrintOperation( char* str, int numOp )
{
    ASSERT( str != NULL, 0 );

    int indOp = GetIndexOperation( numOp );     
    if( indOp == -1 ) return -1; 
    
    sprintf( str, "%s", OpStrings[ indOp ].strStandart );
            
    return 1;
}

int PrintLngNode( char* str, Node* node, int typePrinting )
{
    ASSERT( str  != NULL, 0 );
    ASSERT( node != NULL, 0 );

    if/* */( node->value->type == VAL_TYPE )
    {
        sprintf( str, "%g", node->value->dblValue );
        return VAL_TYPE;
    }
    else if( node->value->type == OP_TYPE )
    {   
        PrintOperation( str, node->value->opValue );
        return OP_TYPE;
    }
    else if( node->value->type == VAR_TYPE )
    {
        char* varName = node->value->varValue;   
        if(  !varName  ) return -1; 
        
        if( typePrinting == DUMP ) sprintf( str,   "%s",   varName );
        else                       sprintf( str, "\"%s\"", varName );

        return VAR_TYPE;
    }
    else
    {
        int type = node->value->type; 
        int ind  = GetIndexType( type );
        
        if( typePrinting == DUMP ) sprintf( str, "%s", TypeStrings[ ind ].str         ); 
        else                       sprintf( str, "%s", TypeStrings[ ind ].strStandart ); 

        return type;
    }

    return 1;
}

int PrintDotNode( Node* node, int nodeNum, FILE* dotFile, int side )
{
    ASSERT( dotFile != NULL, 0 );

    if( !node ) return 0;

    int   typeNum = VAL_TYPE;
    char* color   = NULL;

    if( node->value != NULL ) typeNum = node->value->type; 

    if/* */( typeNum == Types::OP_TYPE  ) { color = "lightgrey"   ; }
    else if( typeNum == Types::VAL_TYPE ) { color = "lightgreen"  ; }
    else if( typeNum == Types::VAR_TYPE ) { color = "lightblue"   ; }
    else                                  { color = "lightyellow" ; }

    char valueStr[ MaxStrLen ] = "";
    PrintLngNode( valueStr, node );

    if( side == LR )
    {
        fprintf( dotFile, "\tnode%d[ shape = record, style = \"filled\", fillcolor = \"%s\", label = \"{ { p: %p | n: %p | %s } }\" ];\n", 
                            nodeNum, color, node->parent, node, valueStr );   
    }
    else
    {       
        fprintf( dotFile, "\tnode%d[ shape = record, style = \"filled\", fillcolor = \"%s\", label = \"{ p: %p | n: %p | { %s } }\" ];\n", 
                           nodeNum, color, node->parent, node, valueStr );   
    }

    return 1;
}

int GraphVizTree( Node* node, FILE* dotFile, int* nodeNum )
{
    ASSERT( dotFile != NULL, 0 );
    ASSERT( nodeNum != NULL, 0 );

    if( !node ) return 0;

    int leftNum  = 0;
    int rightNum = 0;
    
    if/* */( node->left )
    {
        leftNum  = GraphVizTree( node->left,  dotFile, nodeNum );
    }    
    if( node->right )
    {   
        rightNum = GraphVizTree( node->right, dotFile, nodeNum );
    }     

    PrintDotNode( node, *nodeNum, dotFile );                              

    if( node->left )
    {
        fprintf( dotFile, "\t\"node%d\" -> \"node%d\"\n", *nodeNum, leftNum );
    }
    if( node->right )
    {
        fprintf( dotFile, "\t\"node%d\" -> \"node%d\"\n", *nodeNum, rightNum );
    }

    (*nodeNum)++;
    return *nodeNum - 1;
}

int GraphVizNodes( Node** nodes, FILE* dotFile )
{
    ASSERT( dotFile != NULL, 0 );

    if( !nodes ) return 0;

    int leftNum  = 0;
    int rightNum = 0;
    
    int numNodes = 0;
    
    for( int i = 0; i < MaxNumNodes; i++ )
    {
        if( nodes[i] == NULL ) { numNodes = i; break; }
        
        PrintDotNode( nodes[i], i, dotFile, LR );
    }

    for( int i = 1; i < numNodes; i++ )
    {   
        fprintf( dotFile, "\t\"node%d\" -> \"node%d\" [ style = invis ]; \n", i - 1, i );
    }

    return 1;
}

//-----------------------------------------------------------------------------

FILE* LngCreateDotTreeDumpFile( Node* node, const char* fileName )
{
    ASSERT( node != NULL, NULL );

    FILE* tempDotFile = fopen( fileName, "w" );

    fprintf( tempDotFile, "digraph TreeDump\n" );
    fprintf( tempDotFile, "{\n" );
    {
        int curNodeNum = 1;  
        GraphVizTree( node, tempDotFile, &curNodeNum );
    }
    fprintf( tempDotFile, "}\n" );

    return tempDotFile;
}

FILE* LngCreateDotNodesDumpFile( Node** nodes, const char* fileName )
{
    ASSERT( nodes != NULL, NULL );

    FILE* tempDotFile = fopen( fileName, "w" );

    fprintf( tempDotFile, "digraph NodesDump\n" );
    fprintf( tempDotFile, "{\n" );
    {
        fprintf( tempDotFile, "rankdir = LR;\n" );
        
        GraphVizNodes( nodes, tempDotFile );
    }
    fprintf( tempDotFile, "}\n" );

    return tempDotFile;
}

//-----------------------------------------------------------------------------

int FillHtmlFile( const char* graphName, const char* str, ... )
{
    ASSERT( graphName != NULL, 0 );

    FileLngDump = fopen( FileLngDumpName, "a+" );

    va_list   arg = {};
    va_start( arg, str );

    // Create html file
    fprintf( FileLngDump, "<pre>" );
    fprintf( FileLngDump, "<font size = 4>" );
   vfprintf( FileLngDump, str, arg );
    fprintf( FileLngDump, "</font>" );

    if( str ) 
    {
        fprintf( FileLngDump, "\n\n" );
    }

    fprintf( FileLngDump, "<img src = \"%s\", style = \" max-width: 95vw \">", graphName );
    fprintf( FileLngDump, "<hr>" );

    va_end( arg );
    fclose( FileLngDump );

    return 1;
}

int LngGraphDumpTree( Node* node, const char* str, ... )
{
    if( node == NULL ) return 0;

    const char* tempDotFileName = "temp_graph_viz_tree.dot"; 
    FILE*       tempDotFile = LngCreateDotTreeDumpFile( node, tempDotFileName );
    fclose(     tempDotFile     );

    static int dumpNum = 0;

    char     graphName[MaxStrLen] = "";
    sprintf( graphName, "img/graph_tree%d.png", dumpNum++ );

    CreateGraphVizImg( tempDotFileName, graphName, "png" );

    // Delete temp file
    remove( tempDotFileName );

    // Create html
    va_list   arg = {};
    va_start( arg, str );

    FillHtmlFile( graphName, str, arg );

    va_end( arg );

    return 1;
}

int LngGraphDumpNodes( Node** nodes, const char* str, ... )
{
    ASSERT( nodes != NULL, 0 );

    const char* tempDotFileName = "temp_graph_viz_nodes.dot"; 
    FILE*       tempDotFile = LngCreateDotNodesDumpFile( nodes, tempDotFileName );
    fclose(     tempDotFile     );

    static int dumpNum = 0;

    char     graphName[MaxStrLen] = "";
    sprintf( graphName, "img/graph_nodes%d.png", dumpNum++ );

    CreateGraphVizImg( tempDotFileName, graphName, "png" );

    // Delete temp file
    remove( tempDotFileName );

    // Create html
    va_list   arg = {};
    va_start( arg, str );

    FillHtmlFile( graphName, str, arg );

    va_end( arg );

    return 1;
}

//-----------------------------------------------------------------------------
// End DiffDump
