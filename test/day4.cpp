#include "day4.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day4Test, Part1Example) {
  InputFile in("day4_ex");
  int result = day4_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 13);
}

TEST(Day4Test, Part1Input) {
  InputFile in("day4");
  int result = day4_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(4, 1));
}

TEST(Day4Test, Part2Example) {
  InputFile in("day4_ex");
  int result = day4_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 30);
}

TEST(Day4Test, Part2Input) {
  InputFile in("day4");
  int result = day4_part2(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(4, 2));
}
