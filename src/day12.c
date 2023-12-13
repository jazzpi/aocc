#include "day12.h"
#include "re.h"
#include "util.h"

#include <assert.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t cond_idx;
  size_t group_idx;
  long group_size;
  const char* conditions;
} state_t;

#define RIDX(state)                                                            \
  ((state)->cond_idx * (ngroups + 1) * (max_group + 1) +                       \
   (state)->group_idx * (max_group + 1) + (state)->group_size)

long count_group_arrangements_dyn(const char* conditions, const long* groups,
                                  size_t ngroups, long max_group, long* results,
                                  const state_t* state) {
  if (results[RIDX(state)] != -1) {
    return results[RIDX(state)];
  }

  long result;
  state_t new_state = *state;
  new_state.cond_idx++;
  switch (conditions[state->cond_idx]) {
  case '#': {
    new_state.group_size++;
    if (new_state.group_idx >= ngroups ||
        new_state.group_size > groups[state->group_idx]) {
      result = 0;
    } else {
      result = count_group_arrangements_dyn(conditions, groups, ngroups,
                                            max_group, results, &new_state);
    }
    break;
  }
  case '.': {
    if (state->group_size > 0) {
      if (state->group_idx < ngroups &&
          groups[state->group_idx] == state->group_size) {
        new_state.group_idx++;
        new_state.group_size = 0;
        result = count_group_arrangements_dyn(conditions, groups, ngroups,
                                              max_group, results, &new_state);
      } else {
        result = 0;
      }
    } else {
      result = count_group_arrangements_dyn(conditions, groups, ngroups,
                                            max_group, results, &new_state);
    }
    break;
  }
  case '?': {
    long arrangements = 0;
    // Replace with .
    char* cond_str = strdup(state->conditions);
    cond_str[state->cond_idx] = '.';
    state_t period_state = new_state;
    period_state.conditions = cond_str;
    if (state->group_size == 0) {
      arrangements += count_group_arrangements_dyn(
          conditions, groups, ngroups, max_group, results, &period_state);
    } else if (state->group_idx < ngroups &&
               groups[state->group_idx] == state->group_size) {
      period_state.group_idx++;
      period_state.group_size = 0;
      arrangements += count_group_arrangements_dyn(
          conditions, groups, ngroups, max_group, results, &period_state);
    }
    // Replace with #
    cond_str[state->cond_idx] = '#';
    state_t hash_state = new_state;
    hash_state.conditions = cond_str;
    hash_state.group_size++;
    if (hash_state.group_idx < ngroups && hash_state.group_size <= max_group) {
      arrangements += count_group_arrangements_dyn(
          conditions, groups, ngroups, max_group, results, &hash_state);
    }

    free(cond_str);
    result = arrangements;
    break;
  }
  case '\0': {
    if ((state->group_idx == ngroups && state->group_size == 0) ||
        (state->group_idx == ngroups - 1 &&
         state->group_size == groups[ngroups - 1])) {
      result = 1;
    } else {
      result = 0;
    }
    break;
  }
  default:
    assert(0);
  }

  // if (result > 0) {
  //   printf("Matched all groups: %s (%ld arrangements) \n", state->conditions,
  //          result);
  // }

  results[RIDX(state)] = result;
  return result;
}

long count_group_arrangements(const char* conditions, const long* groups,
                              size_t ngroups) {
  long max_group = 0;
  for (size_t i = 0; i < ngroups; i++) {
    if (groups[i] > max_group) {
      max_group = groups[i];
    }
  }

  size_t results_size =
      sizeof(long) * (strlen(conditions) + 1) * (ngroups + 1) * (max_group + 1);
  long* results = malloc(results_size);
  memset(results, -1, results_size);

  state_t initial_state = {
      .cond_idx = 0, .group_idx = 0, .group_size = 0, .conditions = conditions};
  long result = count_group_arrangements_dyn(
      conditions, groups, ngroups, max_group, results, &initial_state);

  free(results);

  return result;
}

long day12_part1(const char** lines, size_t nlines) {
  regex_t re;
  re_compile(&re, "^([.#?]+) ([0-9,]+)$");

  long result = 0;
  for (size_t l = 0; l < nlines; l++) {
    size_t nmatches;
    re_match_t* matches = re_match_all(&re, lines[l], &nmatches, 0);
    assert(nmatches == 1);

    size_t ngroups;
    long* groups = parse_ints_sep(matches->groups[2].str, &ngroups, ",");
    result += count_group_arrangements(matches->groups[1].str, groups, ngroups);

    re_free_matches(matches, nmatches);
    free(groups);
  }

  regfree(&re);

  return result;
}

char* repeat(const char* src, char sep, size_t n) {
  size_t len = strlen(src);
  char* result = malloc((len + 1) * n);
  strcpy(result, src);
  char* ptr = result + len;
  for (size_t i = 1; i < n; i++) {
    *ptr = sep;
    ptr++;
    strcpy(ptr, src);
    ptr += len;
  }

  return result;
}

long day12_part2(const char** lines, size_t nlines) {
  regex_t re;
  re_compile(&re, "^([.#?]+) ([0-9,]+)$");

  long result = 0;
  for (size_t l = 0; l < nlines; l++) {
    size_t nmatches;
    re_match_t* matches = re_match_all(&re, lines[l], &nmatches, 0);
    assert(nmatches == 1);

    char* groups_str = repeat(matches->groups[2].str, ',', 5);
    size_t ngroups;
    long* groups = parse_ints_sep(groups_str, &ngroups, ",");
    free(groups_str);

    char* cond_str = repeat(matches->groups[1].str, '?', 5);
    long line_result = count_group_arrangements(cond_str, groups, ngroups);
    result += line_result;

    // printf("%s: %ld\n", matches->groups[0].str, line_result);

    free(cond_str);
    re_free_matches(matches, nmatches);
    free(groups);
  }

  regfree(&re);

  return result;
}
