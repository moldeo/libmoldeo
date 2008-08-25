#include <cstdio>

#ifndef _ITOA_H_
#define _ITOA_H_

inline char* itoa(int value, char* str, int radix)
{
    char buffer[100];
    char* res;
    snprintf(buffer, 100, "%i", value); // Memory-safe version of sprintf.
    res = buffer;
    return res;
}

#endif
