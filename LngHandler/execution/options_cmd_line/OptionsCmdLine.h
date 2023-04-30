#ifndef OPTIONS_CMD_LINE_H
#define OPTIONS_CMD_LINE_H

#include <string.h>

//{----------------------------------------------------------------------------
/// @file OptionsCmdLine.h
///
/// @mainpage
/// Files:
/// - @ref OptionsCmdLine.h
//}----------------------------------------------------------------------------

static const char** Argv = NULL; 

static int __NUM_OPTIONS__ = 0;

static int __CUR_OPT__ = 1;

//{----------------------------------------------------------------------------
/// \brief ������� ��� ������ ����� � ������� �����
///
/// \param word �����, ������� ����� �����
/// \param arr ������ �����
/// \param sizeArr ���������� ����� ��� ������
///
/// \return ���������� ������ ������� ����� � �������, ���� ������ �����
///         �� ������� ������������ -1.
//}----------------------------------------------------------------------------

int NumWordInArray( const char* word, const char* arr[], int sizeArr );

const char* GetCurOptionStr();

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
