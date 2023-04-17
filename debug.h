#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, "[DEBUG] " fmt, ##args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

#endif