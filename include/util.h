#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <regex.h>
#include <stddef.h>
#include <string.h>

#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

const char** readlines(const char* filename, size_t* nlines);
void freelines(const char** lines);

#ifdef __cplusplus
}
#endif
