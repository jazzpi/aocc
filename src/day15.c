#include "day15.h"
#include "util.h"

#include <assert.h>
#include <stdint.h>

uint8_t hash(const char* str) {
  uint8_t result = 0;
  while (*str != '\0') {
    result = (result + *str) * 17;
    str++;
  }

  return result;
}

long day15_part1(const char** lines, size_t nlines) {
  assert(nlines == 1);
  size_t n;
  char** steps = split_str(lines[0], &n, ",");
  long result = 0;
  for (size_t i = 0; i < n; i++) {
    result += hash(steps[i]);
  }
  freelines(steps);
  return result;
}
