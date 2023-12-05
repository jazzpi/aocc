#include "day1.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day1Test, Part1Example) {
  InputFile in("day1_ex");
  int result = day1_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 142);
}

TEST(Day1Test, Part1Input) {
  InputFile in("day1");
  int result = day1_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(1, 1));
}

TEST(Day1Test, Part2Example) {
  InputFile in("day1_ex2");
  int result = day1_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 281);
}

TEST(Day1Test, Part2Input) {
  InputFile in("day1");
  int result = day1_part2(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(1, 2));
}
