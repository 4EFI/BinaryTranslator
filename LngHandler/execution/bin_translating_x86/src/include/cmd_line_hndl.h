#ifndef CMD_LINE_HANDLER_H
#define CMD_LINE_HANDLER_H

#include <string.h>

static const char** Argv = NULL; 

static int __NUM_OPTIONS__ = 0;

static int __CUR_OPT__ = 1;

//-----------------------------------------------------------------------------

#define START_OPT_HANDLER( argc, argv )        \
    Argv = argv;                               \
    __NUM_OPTIONS__ = argc;                    \
    for( ; __CUR_OPT__ < argc; __CUR_OPT__++ )                                           

#define FINISH_OPT_HANDLER( ... ) __VA_ARGS__

#define OPT_HANDLER( option_name, ... )                  \
{                                                        \
    if ( strcmp( Argv[__CUR_OPT__], option_name ) == 0 ) \
    {                                                    \
        __VA_ARGS__ ;                                    \
        continue;                                        \
    }                                                    \
}

//-----------------------------------------------------------------------------

#endif
