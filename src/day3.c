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

typedef struct {
  size_t row;
  size_t start_col;
  size_t end_col;
  int value;
} part_no;

part_no* find_part_numbers(const char** lines, size_t nlines, size_t* n_parts) {
  assert(nlines > 0);
  size_t linelen = strlen(lines[0]);

  regex_t re;
  if (regcomp(&re, "[0-9]+", REG_EXTENDED) != 0) {
    fprintf(stderr, "Failed to compile regex\n");
    exit(1);
  }

  *n_parts = 0;
  size_t buf_size = 128;
  part_no* numbers = malloc(sizeof(*numbers) * buf_size);

  for (size_t l = 0; l < nlines; l++) {
    assert(strlen(lines[l]) == linelen);
    size_t nmatches;
    regmatch_t* matches = re_match_all(&re, lines[l], &nmatches, 0);
    for (size_t n = 0; n < nmatches; n++) {
      if (is_part_number(l, &matches[n], lines, nlines, linelen)) {
        char* str = re_match_dup(lines[l], &matches[n]);
        int value = atoi(str);
        free(str);

        if (*n_parts == buf_size) {
          buf_size *= 2;
          numbers = realloc(numbers, sizeof(*numbers) * buf_size);
        }

        part_no* no = &numbers[*n_parts];
        *n_parts += 1;
        no->row = l;
        no->start_col = matches[n].rm_so;
        no->end_col = matches[n].rm_eo - 1;
        no->value = value;
      }
    }
    free(matches);
  }

  regfree(&re);

  return numbers;
}

void part1(const char** lines, size_t nlines) {
  size_t n_parts;
  part_no* numbers = find_part_numbers(lines, nlines, &n_parts);
  size_t sum = 0;
  for (size_t i = 0; i < n_parts; i++) {
    sum += numbers[i].value;
  }
  free(numbers);

  printf("Sum of part numbers: %zu\n", sum);
}

part_no* part_at(size_t row, size_t col, part_no* parts, size_t n_parts) {
  for (size_t i = 0; i < n_parts; i++) {
    part_no* part = &parts[i];
    if (part->row == row && part->start_col <= col && part->end_col >= col) {
      return part;
    }
  }

  return NULL;
}

void part2(const char** lines, size_t nlines) {
  size_t n_parts;
  part_no* parts = find_part_numbers(lines, nlines, &n_parts);

  size_t linelen = strlen(lines[0]);

  int sum = 0;

  for (int row = 0; row < (int)nlines; row++) {
    for (int col = 0; col < (int)linelen; col++) {
      if (lines[row][col] != '*') {
        continue;
      }
      int gear_ratio = 1;
      int parts_adjacent = 0;
      part_no* prev_part = NULL;
      for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
          if (r < 0 || r > (int)nlines || c < 0 || c > (int)linelen ||
              (r == row && c == col)) {
            continue;
          }
          part_no* p = part_at(r, c, parts, n_parts);
          if (p == NULL || p == prev_part) {
            continue;
          }
          prev_part = p;
          parts_adjacent++;
          gear_ratio *= p->value;
        }
      }
      if (parts_adjacent == 2) {
        sum += gear_ratio;
      }
    }
  }

  free(parts);

  printf("Sum of gear ratios: %d\n", sum);
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
}
