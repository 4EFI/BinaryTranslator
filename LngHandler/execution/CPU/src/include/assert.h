#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>

static FILE* AssertFileOut = stderr;

#define Assert(condition, ...)                          \
    if (!(condition))                                   \
    {                                                   \
        fprintf (AssertFileOut,                         \
                 "%s:%d: %s: Assertion '%s' failed.\n", \
                 __FILE__,                              \
                 __LINE__,                              \
                 __PRETTY_FUNCTION__,                   \
                 #condition);                           \
        return __VA_ARGS__;                             \
    }        

#endif
