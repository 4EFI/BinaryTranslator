#ifndef TREE_LNG_DUMP
#define TREE_LNG_DUMP

#include "config.h"

#include <stdio.h>
#include "tree_lng.h"

//-----------------------------------------------------------------------------

enum DumpSide
{
    LR, // Left -> Right
    TD  // Top  -> Down
};

enum TypePrinting
{
    DUMP,
    TREE
};

//-----------------------------------------------------------------------------

struct Stack;

static const char* FileLngDumpName = "lng_dump.html";

int PrintOperation( char* str, int   numOp );
int PrintLngNode  ( char* str, Node* node, int typePrinting = DUMP );

int PrintDotNode( Node* node, int nodeNum, FILE* dotFile, int side = TD );

int GraphVizTree ( Node*  node,  FILE* dotFile, int* nodeNum );
int GraphVizNodes( Node** nodes, FILE* dotFile );

FILE* LngCreateDotTreeDumpFile( Node* node, const char* fileName );
int   LngGraphDumpTree        ( Node* node, const char* str = "", ... );

FILE* LngCreateDotNodesDumpFile( Node** nodes, const char* fileName );
int   LngGraphDumpNodes        ( Node** nodes, const char* str = "", ... );

#endif