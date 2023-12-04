#include "day4.h"
#include "dynarr.h"
#include "re.h"

#include <assert.h>
#include <math.h>
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

typedef DYNARR(int) int_arr;

int* parse_numbers(const char* str, size_t* n) {
  int_arr* nums = dynarr_create(int_arr, 1);

  char* str_copy = strdup(str);
  char* num_str = strtok(str_copy, " ");
  while (num_str != NULL) {
    int num = atoi(num_str);
    dynarr_append(nums, &num);
    num_str = strtok(NULL, " ");
  }

  free(str_copy);
  *n = nums->size;
  return dynarr_extract(nums);
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

void free_cards(card* c, size_t num_cards) {
  for (size_t i = 0; i < num_cards; i++) {
    free(c[i].winning);
    free(c[i].have);
  }
  free(c);
}

int num_winning(card* c) {
  int n = 0;
  for (size_t i = 0; i < c->n_winning; i++) {
    for (size_t j = 0; j < c->n_have; j++) {
      if (c->winning[i] == c->have[j]) {
        n++;
      }
    }
  }
  return n;
}

int day4_part1(const char** lines, size_t nlines) {
  int sum = 0;

  for (size_t l = 0; l < nlines; l++) {
    card* c = malloc(sizeof(*c));
    parse_card(c, lines[l]);

    int n = num_winning(c);
    if (n > 0) {
      sum += exp2(num_winning(c) - 1);
    }

    free_cards(c, 1);
  }

  return sum;
}

int day4_part2(const char** lines, size_t nlines) {
  card* cards = malloc(sizeof(*cards) * nlines);
  int* card_insts = malloc(sizeof(*card_insts) * nlines);

  for (size_t l = 0; l < nlines; l++) {
    parse_card(&cards[l], lines[l]);
    card_insts[l] = 1;
  }

  int num_cards = 0;
  for (size_t l = 0; l < nlines; l++) {
    num_cards += card_insts[l];

    int n = num_winning(&cards[l]);
    for (int i = 0; i < n; i++) {
      size_t idx = l + i + 1;
      if (idx >= nlines) {
        break;
      }
      card_insts[idx] += card_insts[l];
    }
  }

  free_cards(cards, nlines);
  free(card_insts);

  return num_cards;
}
