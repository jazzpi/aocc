#include "re.h"
#include "dynarr.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void re_compile(regex_t* re, const char* pattern) {
  int ret = regcomp(re, pattern, REG_EXTENDED);
  if (ret != 0) {
    size_t bufsize = regerror(ret, re, NULL, 0);
    char* buf = malloc(bufsize);
    regerror(ret, re, buf, bufsize);
    fprintf(stderr, "Couldn't compile pattern `%s`: %s\n", pattern, buf);
    exit(EXIT_FAILURE);
  }
}

typedef DYNARR(re_match_t) re_match_arr;

re_match_t* re_match_all(const regex_t* re, const char* str, size_t* nmatches,
                         int overlapping_matches) {
  size_t n_groups = re->re_nsub + 1;

  re_match_arr* matches = dynarr_create(re_match_arr, 1);
  size_t str_offset = 0;
  regmatch_t group_matches[n_groups];

  while (str[str_offset] != '\0' &&
         regexec(re, str + str_offset, n_groups, group_matches, 0) == 0) {
    re_match_t* match = dynarr_incsize(matches);
    match->groups = malloc(sizeof(*match->groups) * n_groups);
    for (size_t i = 0; i < n_groups; i++) {
      regmatch_t* group_match = &group_matches[i];
      re_group_t* group = &match->groups[i];
      group->matched = group_match->rm_so != -1;
      if (!group->matched) {
        continue;
      }
      group->start = group_match->rm_so + str_offset;
      group->len = group_match->rm_eo - group_match->rm_so;
      group->str = strndup(str + group->start, group->len);
    }
    match->ngroups = n_groups;

    if (overlapping_matches) {
      str_offset = match->groups[0].start + 1;
    } else {
      str_offset = match->groups[0].start + match->groups[0].len;
    }
  }

  *nmatches = matches->size;
  return dynarr_extract(matches);
}

void re_free_matches(re_match_t* matches, size_t nmatches) {
  for (size_t i = 0; i < nmatches; i++) {
    for (size_t j = 0; j < matches[i].ngroups; j++) {
      free(matches[i].groups[j].str);
    }
    free(matches[i].groups);
  }
  free(matches);
}
