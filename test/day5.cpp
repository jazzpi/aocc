#include "day5.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day5Test, Part1Example) {
  InputFile in("day5_ex");
  int result = day5_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 35);
}

TEST(Day5Test, Part1Input) {
  InputFile in("day5");
  int result = day5_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(5, 1));
}

// TEST(Day5Test, Part2Example) {
//   InputFile in("day5_ex");
//   int result = day5_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, 30);
// }

// TEST(Day5Test, Part2Input) {
//   InputFile in("day5");
//   int result = day5_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, get_solutions(5, 2));
// }
