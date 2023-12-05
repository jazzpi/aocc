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

/**
 * Parse a space-separated list of integers.
 *
 * The integers may be separated by multiple space characters.
 *
 * @param str The string to parse.
 * @param n The number of integers parsed is written to this pointer.
 *
 * @return An array of integers. NULL if no integers were read. Must be freed
 * with free() otherwise.
 */
long* parse_ints(const char* str, size_t* n);

#ifdef __cplusplus
}
#endif
