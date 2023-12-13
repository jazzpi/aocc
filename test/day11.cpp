#include "day11.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day11Test, Part1Example) {
  InputFile in("day11_ex");
  long result = day11_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 374);
}

TEST(Day11Test, Part1Input) {
  InputFile in("day11");
  long result = day11_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(11, 1));
}

TEST(Day11Test, Part2Example) {
  InputFile in("day11_ex");
  expansion_factor_part2 = 10;
  long result = day11_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 1030);
  expansion_factor_part2 = 100;
  result = day11_part2(in.lines, in.nlines);
  EXPECT_EQ(result, 8410);
}

TEST(Day11Test, Part2Input) {
  expansion_factor_part2 = 1000000;
  InputFile in("day11");
  long result = day11_part2(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(11, 2));
}
