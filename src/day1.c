#include "util.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void part1(const char **lines, size_t nlines) {
  int sum = 0;
  for (size_t l = 0; l < nlines; l++) {
    char d1 = 0;
    char d2 = 0;

    const char *c = lines[l];
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

int main(int argc, char **argv) {
  MAIN_BOILERPLATE
  if (part == 1) {
    part1(lines, nlines);
  }

  freelines(lines);
  return 0;
}
