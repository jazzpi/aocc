#include "day3.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day3Test, Part1Example) {
  InputFile in("day3_ex");
  int result = day3_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 4361);
}

TEST(Day3Test, Part1Input) {
  InputFile in("day3");
  int result = day3_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 530849);
}

TEST(Day3Test, Part2Example) {
  InputFile in("day3_ex");
  int result = day3_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 467835);
}

TEST(Day3Test, Part2Input) {
  InputFile in("day3");
  int result = day3_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 84900879);
}
