#include "day12.h"
#include "re.h"
#include "util.h"

#include <assert.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long count_group_arrangements(const char* conditions, const long* groups,
                              size_t ngroups) {
  long group_size = 0;
  size_t group_idx = 0;
  for (size_t i = 0; conditions[i] != '\0'; i++) {
    char c = conditions[i];
    switch (c) {
    case '#':
      group_size++;
      break;
    case '.': {
      if (group_size > 0) {
        if (group_idx < ngroups && groups[group_idx] == group_size) {
          group_idx++;
        } else {
          // printf("Couldn't match group %ld: %s\n", group_idx, conditions);
          return 0;
        }
        group_size = 0;
      }
      break;
    }
    case '?': {
      char* dup = strdup(conditions);
      dup[i] = '.';
      long arrangements = count_group_arrangements(dup, groups, ngroups);
      dup[i] = '#';
      arrangements += count_group_arrangements(dup, groups, ngroups);
      free(dup);
      return arrangements;
    }
    default:
      assert(0);
    }
  }

  if ((group_idx == ngroups && group_size == 0) ||
      (group_idx == ngroups - 1 && groups[group_idx] == group_size)) {
    // printf("Found valid arrangement: %s\n", conditions);
    return 1;
  } else {
    // printf("Didn't match all groups: %s\n", conditions);
    return 0;
  }
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
