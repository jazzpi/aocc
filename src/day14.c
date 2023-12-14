#include "day14.h"

#include "dynarr.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long day14_part1(const char** lines, size_t nlines) {
  assert(nlines > 0);
  size_t width = strlen(lines[0]);

  long result = 0;
  for (size_t col = 0; col < width; col++) {
    long stop = nlines + 1;
    for (size_t row = 0; row < nlines; row++) {
      switch (lines[row][col]) {
      case 'O':
        stop--;
        result += stop;
        break;
      case '#':
        stop = nlines - row;
        break;
      default:
        assert(lines[row][col] == '.');
      }
    }
  }

  return result;
}
