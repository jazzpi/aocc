#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

__attribute__((noreturn)) void usage_abrt(char* progname) {
  fprintf(stderr, "Usage: %s DAY PART <input file>\n", progname);
  exit(EX_USAGE);
}

int main(int argc, char** argv) {
  if (argc != 4) {
    usage_abrt(argv[0]);
  }
  int day = atoi(argv[1]);
  int part = atoi(argv[2]);
  if (part < 1 || part > 2) {
    usage_abrt(argv[0]);
  }
  int (*solver)(const char** lines, size_t nlines);
  switch (day) {
  case 1:
    solver = (part == 1) ? day1_part1 : day1_part2;
    break;
  case 2:
    solver = (part == 1) ? day2_part1 : day2_part2;
    break;
  case 3:
    solver = (part == 1) ? day3_part1 : day3_part2;
    break;
  default:
    usage_abrt(argv[0]);
  }

  size_t nlines;
  const char** lines = readlines(argv[3], &nlines);

  printf("Result: %d\n", solver(lines, nlines));

  freelines(lines);
}
