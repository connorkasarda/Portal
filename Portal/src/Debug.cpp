#include <cstdio>
#include <cstdarg>
#include "Debug.h"

void error(int line, const char* file, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::fprintf(stderr, "[%s:%d] ", file, line);
    std::vfprintf(stderr, format, args);
    va_end(args);
    std::fprintf(stderr, "\n");
}