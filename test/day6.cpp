#include "day6.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day6Test, Part1Example) {
  InputFile in("day6_ex");
  int result = day6_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 288);
}

TEST(Day6Test, Part1Input) {
  InputFile in("day6");
  int result = day6_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(6, 1));
}

TEST(Day6Test, Part2Example) {
  InputFile in("day6_ex");
  int result = day6_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 71503);
}

TEST(Day6Test, Part2Input) {
  InputFile in("day6");
  int result = day6_part2(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(6, 2));
}
