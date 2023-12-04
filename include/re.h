#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <regex.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  bool matched;
  size_t start;
  size_t len;
  char* str;
} re_group_t;

typedef struct {
  size_t ngroups;
  re_group_t* groups;
} re_match_t;

/**
 * Compile a regular expression.
 *
 * Prints a message to stderr and exits if there is an error.
 */
void re_compile(regex_t* re, const char* pattern);

/**
 * Repeatedly match a regular expression against a string.
 *
 * @param re The compiled regular expression.
 * @param str The string to match against.
 * @param nmatches The number of matches found is written to this pointer.
 * @param overlapping_matches Whether or not matches can overlap.
 *
 * @return An array of matches. Must be freed with re_free_matches().
 */
re_match_t* re_match_all(const regex_t* re, const char* str, size_t* nmatches,
                         int overlapping_matches);

/**
 * Free the memory used by matches returned by re_match_all().
 */
void re_free_matches(re_match_t* matches, size_t nmatches);

#ifdef __cplusplus
}
#endif
