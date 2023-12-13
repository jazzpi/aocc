#include "day10.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day10Test, Part1Example) {
  InputFile in("day10_ex");
  long result = day10_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 8);
}

TEST(Day10Test, Part1Input) {
  InputFile in("day10");
  long result = day10_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(10, 1));
}

TEST(Day10Test, Part2Example) {
  InputFile in("day10_ex2");
  long result = day10_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 4);
  InputFile in2("day10_ex3");
  result = day10_part2(in2.lines, in2.nlines);
  EXPECT_EQ(result, 10);
}

TEST(Day10Test, Part2Input) {
  InputFile in("day10");
  long result = day10_part2(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(10, 2));
}
