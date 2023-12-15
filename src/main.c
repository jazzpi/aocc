#include "day1.h"
#include "day10.h"
#include "day11.h"
#include "day12.h"
#include "day13.h"
#include "day14.h"
#include "day15.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "day6.h"
#include "day7.h"
#include "day8.h"
#include "day9.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

__attribute__((noreturn)) void usage_abrt(char* progname) {
  fprintf(stderr, "Usage: %s DAY PART <input file>\n", progname);
  exit(EX_USAGE);
}

typedef long (*solver_t)(const char**, size_t);
static solver_t solvers[][2] = {
    {day1_part1, day1_part2},   {day2_part1, day2_part2},
    {day3_part1, day3_part2},   {day4_part1, day4_part2},
    {day5_part1, day5_part2},   {day6_part1, day6_part2},
    {day7_part1, day7_part2},   {day8_part1, day8_part2},
    {day9_part1, day9_part2},   {day10_part1, day10_part2},
    {day11_part1, day11_part2}, {day12_part1, day12_part2},
    {day13_part1, day13_part2}, {day14_part1, day14_part2},
    {day15_part1, day15_part2},
};

int main(int argc, char** argv) {
  if (argc != 4) {
    usage_abrt(argv[0]);
  }
  int day = atoi(argv[1]);
  int part = atoi(argv[2]);
  if (part < 1 || part > 2) {
    usage_abrt(argv[0]);
  }
  if (day < 1 || ((size_t)day) > sizeof(solvers) / sizeof(*solvers)) {
    usage_abrt(argv[0]);
  }
  solver_t solver = solvers[day - 1][part - 1];

  size_t nlines;
  const char** lines = readlines(argv[3], &nlines);

  printf("Result: %ld\n", solver(lines, nlines));

  freelines(lines);
}
