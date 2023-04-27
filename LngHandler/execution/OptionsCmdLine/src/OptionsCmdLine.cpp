#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <algorithm>

#include "../OptionsCmdLine.h"

//-----------------------------------------------------------------------------

const char* GetCurOptionStr()
{
    return Argv[__CUR_OPT__];
}

//-----------------------------------------------------------------------------

int NumWordInArray( const char* word, const char* arr[], int sizeArr )
{
    //{ ASSERT
    assert( word != NULL );
    assert( arr  != NULL );
    //}

    for( int i = 0; i < sizeArr; i++ )
    {
        if( strcmp( word, arr[i] ) == 0 )
        {
            return i;
        }
    }

    return -1;
}

//-----------------------------------------------------------------------------
