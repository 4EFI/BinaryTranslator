
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bin_trtor.h"

//-----------------------------------------------------------------------------

int BinTrtorCtor( BinTrtor* bin_trtor, const char* bin_code )
{
    bin_trtor->bin_code = ( char* )bin_code;
    bin_trtor->num_cmds = 0;

    bin_trtor->max_num_cmds = strlen( bin_code );
    bin_trtor->commands = ( Command* )calloc( bin_trtor->max_num_cmds, sizeof( Command ) ); 
    
    return 1;
}

//-----------------------------------------------------------------------------

int BinTrtorDtor( BinTrtor* bin_trtor )
{
    bin_trtor->bin_code = NULL;

    free( bin_trtor->commands );
    bin_trtor->commands = NULL;

    bin_trtor->num_cmds     = 0;
    bin_trtor->max_num_cmds = 0;

    return 1;
}

//-----------------------------------------------------------------------------