#include "util.h"

#include <assert.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

void part1(const char** lines, size_t nlines) {
  int sum = 0;
  for (size_t l = 0; l < nlines; l++) {
    char d1 = 0;
    char d2 = 0;

    const char* c = lines[l];
    while (*c != 0) {
      if (*c >= '0' && *c <= '9') {
        if (d1 == 0) {
          d1 = *c;
        }
        d2 = *c;
      }
      c++;
    }
    assert(d1 != 0);
    char num[] = {d1, d2, 0};
    sum += atoi(num);
  }
  printf("Calibration: %d\n", sum);
}

int part2_digit(const char* digit_str) {
  if (strlen(digit_str) == 1) {
    return atoi(digit_str);
  } else if (strcmp(digit_str, "one") == 0) {
    return 1;
  } else if (strcmp(digit_str, "two") == 0) {
    return 2;
  } else if (strcmp(digit_str, "three") == 0) {
    return 3;
  } else if (strcmp(digit_str, "four") == 0) {
    return 4;
  } else if (strcmp(digit_str, "five") == 0) {
    return 5;
  } else if (strcmp(digit_str, "six") == 0) {
    return 6;
  } else if (strcmp(digit_str, "seven") == 0) {
    return 7;
  } else if (strcmp(digit_str, "eight") == 0) {
    return 8;
  } else if (strcmp(digit_str, "nine") == 0) {
    return 9;
  }

  fprintf(stderr, "`%s' is not a valid digit!\n", digit_str);
  exit(1);
}

void part2(const char** lines, size_t nlines) {
  regex_t re;
  if (regcomp(&re, "([0-9]|one|two|three|four|five|six|seven|eight|nine)",
              REG_EXTENDED) != 0) {
    fprintf(stderr, "Failed to compile regex\n");
    exit(1);
  }

  int sum = 0;
  for (size_t l = 0; l < nlines; l++) {
    size_t nmatches;
    regmatch_t* matches = re_match_all(&re, lines[l], &nmatches, 1);
    assert(nmatches > 0);
    char* d1s = re_match_dup(lines[l], &matches[0]);
    int d1 = part2_digit(d1s);
    free(d1s);
    char* d2s = re_match_dup(lines[l], &matches[nmatches - 1]);
    int d2 = part2_digit(d2s);
    free(d2s);
    free(matches);
    sum += d1 * 10 + d2;
  }

  regfree(&re);

  printf("Calibration: %d\n", sum);
}

int main(int argc, char** argv) {
  MAIN_BOILERPLATE
  if (part == 1) {
    part1(lines, nlines);
  } else if (part == 2) {
    part2(lines, nlines);
  } else {
    usage_abrt(argv[0]);
  }

  freelines(lines);
  return 0;
}
