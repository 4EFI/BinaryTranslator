#ifndef LOG_H
#define LOG_H

//{----------------------------------------------------------------------------
/// @file LOG.h
///
/// \brief To remove logging, you need to declare the NLOG macro (#define NLOG)
///
/// \brief The library also has a macro $$(code) (#define $$(code)). This macro 
///        logs the code and executes it, which is passed to it as an argument.
///
/// \brief Macro $LOG_LVL_UP (#define $LOG_LVL_UP). This macro raises the logging 
///       level by 1 until it exits the corresponding block, in which case 
///       the block level is lowered by 1.
///
/// @mainpage
/// Files:
/// - @ref LOG.h
//}----------------------------------------------------------------------------

#include <stdio.h>

//-----------------------------------------------------------------------------

const int MaxStr    = 100;
const int TabNumSym = 4;

/// \brief Logging level
extern int LogLvlTree;

//{----------------------------------------------------------------------------
/// \brief ������� ��� �������� ����� ��� �����������
///
/// \param path ���� � ����� ��� �����������
///
/// \return LogFile ���� ��� �����������
//}----------------------------------------------------------------------------

FILE* OpenLogFile (const char* path = "LOG.txt");

#ifndef ON_LOG_FILE
    #define OpenLogFile() NULL
#endif

/// \brief 	The file to which the logs are output
static FILE* LogFile = OpenLogFile();

#undef OpenLogFile()

//-----------------------------------------------------------------------------

struct FunctionsCallTree
{
public:
    FunctionsCallTree (const char* _funcName);
   ~FunctionsCallTree();

private:
    const char* funcName = NULL;
};

//-----------------------------------------------------------------------------

void _LOG (FILE* file, const char fileName[], const int line, const char str[], ...);

bool IsTTY (FILE* file);

void FinishLog();

void PutsSpaces  (FILE* file, /*Space*/ int numSpaces);
void PutsSymbols (FILE* file, char sym, int numSyms);

void PrintCurTime (FILE* file);

//-----------------------------------------------------------------------------

#define LOG_INFO(file, fileName, line) \
    fprintf (file, "%s:%d: ", fileName, line);


#ifndef NLOG

//{----------------------------------------------------------------------------
/// \brief ������ ��� ����������� � ����
///
/// \param str ��������� ������
/// \param ... ��������� ������������ � �������
//}----------------------------------------------------------------------------

#define FLOG(str, ...)                                      \
    _LOG (LogFile, __FILE__, __LINE__, str, ##__VA_ARGS__); \

//{----------------------------------------------------------------------------
/// \brief ������ ��� ����������� � � ����, � � stderr
///
/// \param str ��������� ������
/// \param ... ��������� ������������ � �������
//}----------------------------------------------------------------------------

#define LOG(str, ...)                                      \
{                                                          \
    FLOG (str, ##__VA_ARGS__);                             \
    _LOG (stderr, __FILE__, __LINE__, str, ##__VA_ARGS__); \
}

#define $$(code)  \
{                 \
    FLOG (#code); \
    code;         \
}

//{----------------------------------------------------------------------------
/// \brief ������ ��� ���������� ������ ����������� �� 1
///
/// \note ��� ������ �� ����� - ������� ����������� ���������� �� 1
//}----------------------------------------------------------------------------

#define $LOG_LVL_UP \
    FunctionsCallTree __funcsTree(__PRETTY_FUNCTION__);

#else

#define FLOG(str, ...) ;
#define  LOG(str, ...) ;

#define $$(code, ...) code ##__VA_ARGS__

#define $LOG_LVL_UP ;

#endif

//-----------------------------------------------------------------------------

#endif
