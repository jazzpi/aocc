#include "day9.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day9Test, Part1Example) {
  InputFile in("day9_ex");
  long result = day9_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 114);
}

TEST(Day9Test, Part1Input) {
  InputFile in("day9");
  long result = day9_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(9, 1));
}

TEST(Day9Test, Part2Example) {
  InputFile in("day9_ex");
  long result = day9_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 2);
}

TEST(Day9Test, Part2Input) {
  InputFile in("day9");
  long result = day9_part2(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(9, 2));
}
