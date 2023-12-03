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
 * Compile a regular expression, printing a message to stderr and exiting if
 * there is an error.
 */
void re_compile(regex_t* re, const char* pattern);

re_match_t* re_match_all(const regex_t* re, const char* str, size_t* nmatches,
                         int overlapping_matches);

void re_free_matches(re_match_t* matches, size_t nmatches);

#ifdef __cplusplus
}
#endif
