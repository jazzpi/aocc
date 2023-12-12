#include "day8.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day8Test, Part1Example) {
  InputFile in("day8_ex");
  long result = day8_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 2);
  InputFile in2("day8_ex2");
  result = day8_part1(in2.lines, in2.nlines);
  EXPECT_EQ(result, 6);
}

TEST(Day8Test, Part1Input) {
  InputFile in("day8");
  long result = day8_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(8, 1));
}

TEST(Day8Test, Part2Example) {
  InputFile in("day8_ex3");
  long result = day8_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 6);
}

TEST(Day8Test, Part2Input) {
  InputFile in("day8");
  long result = day8_part2(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(8, 2));
}
