#include "day13.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

long analyze_pattern(const char** lines, size_t height) {
  assert(height > 0);
  size_t width = strlen(lines[0]);

  for (size_t after_row = 0; after_row < height - 1; after_row++) {
    bool symmetric = true;
    for (size_t diff = 0; diff <= after_row && after_row + diff < height - 1;
         diff++) {
      const char* above = lines[after_row - diff];
      const char* below = lines[after_row + diff + 1];
      if (strcmp(above, below) != 0) {
        symmetric = false;
        break;
      }
    }
    if (symmetric) {
      return (after_row + 1) * 100;
    }
  }

  for (size_t after_col = 0; after_col < width - 1; after_col++) {
    bool symmetric = true;
    for (size_t diff = 0; diff <= after_col && after_col + diff < width - 1;
         diff++) {
      for (size_t row = 0; row < height; row++) {
        if (lines[row][after_col - diff] != lines[row][after_col + diff + 1]) {
          symmetric = false;
          break;
        }
      }
      if (!symmetric) {
        break;
      }
    }
    if (symmetric) {
      return after_col + 1;
    }
  }

  return 0;
}

long day13_part1(const char** lines, size_t nlines) {
  long result = 0;
  size_t start_pattern = 0;
  for (size_t l = 0; l < nlines; l++) {
    if (strlen(lines[l]) == 0) {
      result += analyze_pattern(lines + start_pattern, l - start_pattern);
      start_pattern = l + 1;
    }
  }
  result += analyze_pattern(lines + start_pattern, nlines - start_pattern);

  return result;
}

long analyze_pattern_smudged(const char** lines, long height, long orig_line) {
  assert(height > 0);
  long width = strlen(lines[0]);

  for (long after_row = 0; after_row < height - 1; after_row++) {
    if (orig_line == (after_row + 1) * 100) {
      continue;
    }
    bool symmetric = true;
    for (long diff = 0; diff <= after_row && after_row + diff < height - 1;
         diff++) {
      const char* above = lines[after_row - diff];
      const char* below = lines[after_row + diff + 1];
      if (strcmp(above, below) != 0) {
        symmetric = false;
        break;
      }
    }
    if (symmetric) {
      return (after_row + 1) * 100;
    }
  }

  for (long after_col = 0; after_col < width - 1; after_col++) {
    if (orig_line == after_col + 1) {
      continue;
    }
    bool symmetric = true;
    for (long diff = 0; diff <= after_col && after_col + diff < width - 1;
         diff++) {
      for (long row = 0; row < height; row++) {
        if (lines[row][after_col - diff] != lines[row][after_col + diff + 1]) {
          symmetric = false;
          break;
        }
      }
      if (!symmetric) {
        break;
      }
    }
    if (symmetric) {
      return after_col + 1;
    }
  }

  return 0;
}

long day13_part2(const char** lines, size_t nlines) {
  long result = 0;
  size_t start_pattern = 0;
  for (size_t l = 0; l < nlines; l++) {
    size_t pattern_lines = 0;
    if (strlen(lines[l]) == 0) {
      pattern_lines = l - start_pattern;
    } else if (l == nlines - 1) {
      pattern_lines = nlines - start_pattern;
    }
    if (pattern_lines != 0) {
      long orig_line = analyze_pattern(lines + start_pattern, pattern_lines);
      char** pattern = malloc(sizeof(*pattern) * pattern_lines);
      for (size_t row = 0; row < pattern_lines; row++) {
        pattern[row] = strdup(lines[start_pattern + row]);
      }
      for (size_t row = 0; row < pattern_lines; row++) {
        bool found_line = false;
        for (size_t col = 0; col < strlen(pattern[0]); col++) {
          if (pattern[row][col] == '.') {
            pattern[row][col] = '#';
          } else {
            pattern[row][col] = '.';
          }

          long smudged_line =
              analyze_pattern_smudged(pattern, pattern_lines, orig_line);
          if (smudged_line != 0) {
            found_line = true;
            result += smudged_line;
            break;
          }

          if (pattern[row][col] == '.') {
            pattern[row][col] = '#';
          } else {
            pattern[row][col] = '.';
          }
        }
        if (found_line) {
          break;
        }
      }
      for (size_t row = 0; row < pattern_lines; row++) {
        free(pattern[row]);
      }
      free(pattern);
      start_pattern += pattern_lines + 1;
    }
  }

  return result;
}
