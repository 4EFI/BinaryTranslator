
#include <stdio.h>
#include <locale.h>

#include "ru_translitor.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "lng_parsing.h"
#include "lng_saving.h"
#include "file_algs.h"
#include "LOG.h"

//-----------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
    setlocale( LC_ALL, ".UTF-8" );
    
    const char* fileDataName = "test.ppp";
    FILE* file = fopen( fileDataName, "r" ); 
    if(  !file  ) return -1;

    char* lngData = NULL;
    ReadAllFile( file, &lngData );

    Node* lngNode = GetLngTree( lngData );

    char fileOutName[ MaxStrLen ] = "";
    sprintf( fileOutName, "%s.tree", fileDataName );

    FILE* file_out = fopen( fileOutName, "w" ); 
    if(  !file_out  ) return -1;

    PrintPreorderLngNodes( lngNode, file_out );

    return 0;
}

//-----------------------------------------------------------------------------
