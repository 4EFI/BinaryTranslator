#ifndef LNG_READER_H
#define LNG_READER_H

#include "tree_lng.h"

//-----------------------------------------------------------------------------

enum { IN, OUT };

Node* GetLngTree( const char* str );

Node* GetGrammar     ( Node** nodes, int* curPos );
Node* GetStatement   ( Node** nodes, int* curPos );
Node* GetReturn      ( Node** nodes, int* curPos );
Node* GetCallParams  ( Node** nodes, int* curPos );
Node* GetCallFunction( Node** nodes, int* curPos ); 
Node* GetFunction    ( Node** nodes, int* curPos );
Node* GetParams      ( Node** nodes, int* curPos );
Node* GetInput       ( Node** nodes, int* curPos );
Node* GetOutput      ( Node** nodes, int* curPos );
Node* GetInOutParams ( Node** nodes, int* curPos, int typeParams );
Node* GetCallParam   ( Node** nodes, int* curPos );
Node* GetInitVar     ( Node** nodes, int* curPos );
Node* GetEqual       ( Node** nodes, int* curPos );
Node* GetIf          ( Node** nodes, int* curPos );
Node* GetElse        ( Node** nodes, int* curPos );
Node* GetWhile       ( Node** nodes, int* curPos );
Node* GetCompare     ( Node** nodes, int* curPos );
Node* GetAddSub      ( Node** nodes, int* curPos );
Node* GetMulDiv      ( Node** nodes, int* curPos );
Node* GetPower       ( Node** nodes, int* curPos );
Node* GetBracket     ( Node** nodes, int* curPos );
Node* GetStrMathsFunc( Node** nodes, int* curPos ); // ln sin cos
Node* GetVar         ( Node** nodes, int* curPos );
Node* GetNumber      ( Node** nodes, int* curPos );

//-----------------------------------------------------------------------------

#endif