#include "day4.h"
#include "re.h"

#include <assert.h>
#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int id;
  int* winning;
  size_t n_winning;
  int* have;
  size_t n_have;
} card;

int* parse_numbers(const char* str, size_t* n) {
  size_t buf_size = 2;
  *n = 0;
  int* nums = malloc(sizeof(*nums) * buf_size);

  char* str_copy = strdup(str);
  char* num_str = strtok(str_copy, " ");
  while (num_str != NULL) {
    if (*n == buf_size) {
      buf_size *= 2;
      nums = realloc(nums, sizeof(*nums) * buf_size);
    }
    nums[*n] = atoi(num_str);
    *n += 1;
    num_str = strtok(NULL, " ");
  }

  free(str_copy);
  nums = realloc(nums, sizeof(*nums) * *n);
  return nums;
}

void parse_card(card* c, const char* line) {
  // Only compile the regex when the function is first called
  static regex_t re = {.re_nsub = 0};
  if (re.re_nsub == 0) {
    re_compile(&re, "Card +([0-9]+): +(.*) +\\| +(.*)");
  }

  size_t nmatches;
  re_match_t* match = re_match_all(&re, line, &nmatches, 0);
  assert(nmatches == 1);
  assert(match->ngroups == 4);

  c->id = atoi(match->groups[1].str);
  c->winning = parse_numbers(match->groups[2].str, &c->n_winning);
  c->have = parse_numbers(match->groups[3].str, &c->n_have);

  re_free_matches(match, nmatches);
}

int day4_part1(const char** lines, size_t nlines) {
  int sum = 0;

  for (size_t l = 0; l < nlines; l++) {
    card c;
    parse_card(&c, lines[l]);

    int value = 0;
    for (size_t i = 0; i < c.n_winning; i++) {
      for (size_t j = 0; j < c.n_have; j++) {
        if (c.winning[i] == c.have[j]) {
          if (value == 0) {
            value = 1;
          } else {
            value *= 2;
          }
        }
      }
    }

    sum += value;

    free(c.winning);
    free(c.have);
  }

  return sum;
}
