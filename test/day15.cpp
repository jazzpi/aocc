#include "day15.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day15Test, Part1Example) {
  InputFile in("day15_ex");
  long result = day15_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 52);
  InputFile in2("day15_ex2");
  result = day15_part1(in2.lines, in2.nlines);
  EXPECT_EQ(result, 1320);
}

TEST(Day15Test, Part1Input) {
  InputFile in("day15");
  long result = day15_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(15, 1));
}

TEST(Day15Test, Part2Example) {
  InputFile in("day15_ex2");
  long result = day15_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 145);
}

TEST(Day15Test, Part2Input) {
  InputFile in("day15");
  long result = day15_part2(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(15, 2));
}
