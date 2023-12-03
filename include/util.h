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

regmatch_t* re_match_all(const regex_t* re, const char* str, size_t* nmatches,
                         int overlapping_matches);
regmatch_t* re_match_all_subgroups(const regex_t* re, const char* str,
                                   size_t nmatches_per_re, size_t* nmatches,
                                   int overlapping_matches);
char* re_match_dup(const char* source_str, const regmatch_t* match);

#ifdef __cplusplus
}
#endif
