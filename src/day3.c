#include "util.h"

#include <assert.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

int is_part_number(size_t line_no, regmatch_t* match, const char** lines,
                   size_t nlines, int linelen) {
  int start_check = match->rm_so;
  if (start_check != 0) {
    start_check--;
    // Check left of number
    if (lines[line_no][start_check] != '.') {
      return 1;
    }
  }
  int end_check = match->rm_eo - 1;
  if (end_check != linelen - 1) {
    end_check++;
    // Check right of number
    if (lines[line_no][end_check] != '.') {
      return 1;
    }
  }

  if (line_no != 0) {
    // Check above number (also diagonally)
    for (int i = start_check; i <= end_check; i++) {
      if (lines[line_no - 1][i] != '.') {
        return 1;
      }
    }
  }
  if (line_no != nlines - 1) {
    // Check below nunber (also diagonally)
    for (int i = start_check; i <= end_check; i++) {
      if (lines[line_no + 1][i] != '.') {
        return 1;
      }
    }
  }

  return 0;
}

void part1(const char** lines, size_t nlines) {
  assert(nlines > 0);
  size_t linelen = strlen(lines[0]);

  regex_t re;
  if (regcomp(&re, "[0-9]+", REG_EXTENDED) != 0) {
    fprintf(stderr, "Failed to compile regex\n");
    exit(1);
  }

  size_t sum = 0;
  for (size_t l = 0; l < nlines; l++) {
    assert(strlen(lines[l]) == linelen);
    size_t nmatches;
    regmatch_t* matches = re_match_all(&re, lines[l], &nmatches, 0);
    for (size_t n = 0; n < nmatches; n++) {
      if (is_part_number(l, &matches[n], lines, nlines, linelen)) {
        char* str = re_match_dup(lines[l], &matches[n]);
        sum += atoi(str);
        free(str);
      }
    }
    free(matches);
  }

  regfree(&re);

  printf("Sum of part numbers: %zu\n", sum);
}

int main(int argc, char** argv) {
  MAIN_BOILERPLATE

  if (part == 1) {
    part1(lines, nlines);
  } else {
    usage_abrt(argv[0]);
  }

  freelines(lines);
}
