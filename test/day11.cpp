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

// TEST(Day11Test, Part2Example) {
//   InputFile in("day11_ex2");
//   long result = day11_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, 4);
//   InputFile in2("day11_ex3");
//   result = day11_part2(in2.lines, in2.nlines);
//   EXPECT_EQ(result, 10);
// }

// TEST(Day11Test, Part2Input) {
//   InputFile in("day11");
//   long result = day11_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, get_solution(11, 2));
// }
