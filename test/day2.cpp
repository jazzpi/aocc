#include "day2.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day2Test, Part1Example) {
  InputFile in("day2_ex");
  int result = day2_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 8);
}

TEST(Day2Test, Part1Input) {
  InputFile in("day2");
  int result = day2_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 2256);
}

TEST(Day2Test, Part2Example) {
  InputFile in("day2_ex");
  int result = day2_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 2286);
}

TEST(Day2Test, Part2Input) {
  InputFile in("day2");
  int result = day2_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 74229);
}
