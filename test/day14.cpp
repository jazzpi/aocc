#include "day14.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day14Test, Part1Example) {
  InputFile in("day14_ex");
  long result = day14_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 136);
}

TEST(Day14Test, Part1Input) {
  InputFile in("day14");
  long result = day14_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(14, 1));
}

TEST(Day14Test, Part2Example) {
  InputFile in("day14_ex");
  long result = day14_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 64);
  // From
  // https://www.reddit.com/r/adventofcode/comments/18i45eo/2023_day_14_part_2_worst_case_complexity/
  InputFile in2("day14_ex2");
  result = day14_part2(in2.lines, in2.nlines);
  EXPECT_EQ(result, 165);
}

TEST(Day14Test, Part2Input) {
  InputFile in("day14");
  long result = day14_part2(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(14, 2));
}
