#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <string.h>

#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

/**
 * Read all lines from a file.
 *
 * @param filename The path to the file to read.
 * @param nlines The number of lines read is written to this pointer.
 *
 * @return An array of strings, one for each line in the file. Must be freed
 * with freelines().
 */
const char** readlines(const char* filename, size_t* nlines);

/**
 * Free the lines returned by readlines().
 */
void freelines(const char** lines);

#ifdef __cplusplus
}
#endif
