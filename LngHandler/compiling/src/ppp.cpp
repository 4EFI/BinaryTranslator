
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

char* GetFilePath( const char* full_file_path );

//-----------------------------------------------------------------------------

#define ssystem( str, ... )              \
{										 \
	char temp[ MaxStrLen ] = "";         \
	sprintf( temp, str, ##__VA_ARGS__ ); \
	system(  temp  );                    \
}

//-----------------------------------------------------------------------------

int main( int argc, char *argv[] )
{	
	char* file_path = GetFilePath( argv[0] );

	printf( "Start \"%s\" - triple 'p' compiler (ppp)...\n", argv[0] );

	ssystem( "./%s/lng_to_tree", 		   file_path );	
	// remove ( "test.lng.tree" );
	ssystem( "./%s/tree_to_asm",           file_path );	
	ssystem( "./%s/asm test.ppp.tree.asm", file_path );	
}

//-----------------------------------------------------------------------------

// Path without last '/'

char* GetFilePath( const char* full_file_path )
{
	int len_full_path = strlen( full_file_path );
	int len_file_name = 0;

	for( int i = len_full_path - 1; i >= 0; i-- )
	{
		if( full_file_path[i] == '/' || 
			full_file_path[i] == '\\' )
		{
			break;
		}

		len_file_name++;
	}
	
	int len_path = len_full_path - len_file_name;
	
	char* path = ( char* )calloc( len_path + 1, sizeof( char ) ); // +1 for '\0'

	if( len_path > 0 ) strncpy( path, full_file_path, len_path - 1 );
	
	return path;
}

//-----------------------------------------------------------------------------