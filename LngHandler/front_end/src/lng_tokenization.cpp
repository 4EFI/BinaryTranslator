
#include "config.h"

#include "lng_tokenization.h"
#include "dsl.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "my_assert.h"
#include "LOG.h"
#include "lng_tools.h"
#include "ru_translitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

Node** LngTokenization( const char* str )
{
	ASSERT( str != NULL, NULL );

	Node** nodes = ( Node** )calloc( MaxNumNodes, sizeof( Node* ) ); 

	int curNodePos = 0;

	int isStartComment = false;

	char*   str_ptr  = ( char* )str;
	while( *str_ptr != '\0' )
	{	
		int numReadSyms = 0;

		double num = 0;
		int isNum = sscanf( str_ptr, "%lf%n", &num, &numReadSyms ); 
		if( isNum == -1 ) break;

		if( isNum )
		{
			// Is is comment
			if( isStartComment ) { str_ptr += numReadSyms; continue; }
			
			nodes[ curNodePos ] = CREATE_VAL_NODE( num );
		}
		else
		{
			char* strType = ( char* )calloc( MaxStrLen, sizeof( char ) );

			int  isStr = sscanf( str_ptr, "%s%n", strType, &numReadSyms );
			if( !isStr ) break;

			int opType = -1;
			int type   = GetType( strType, &opType );


			// Remove comments
			if( type == R_COMMENT_TYPE &&  isStartComment ) { str_ptr += numReadSyms; isStartComment = false; continue; }
			if( type == L_COMMENT_TYPE && !isStartComment ) { str_ptr += numReadSyms; isStartComment = true;  continue; }

			if( isStartComment ) { str_ptr += numReadSyms; continue; }


			if/* */( type == OP_TYPE )
			{
				nodes[ curNodePos ] = CREATE_OP_NODE( opType, NULL, NULL );	
			}
			else if( type == -1 )
			{					
				nodes[ curNodePos ] = CREATE_VAR_NODE( TranslitString( strType, strlen( strType ) ) );
			}
			else
			{
				nodes[ curNodePos ] = CREATE_TYPE_NODE( type );
			}
		}

		str_ptr += numReadSyms; 
		curNodePos++;
	}

	nodes[ curNodePos ] = CREATE_TYPE_NODE( END_PROG_TYPE );

	return nodes;
}

//-----------------------------------------------------------------------------