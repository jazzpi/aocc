#include "day13.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day13Test, Part1Example) {
  InputFile in("day13_ex");
  long result = day13_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 405);
}

TEST(Day13Test, Part1Input) {
  InputFile in("day13");
  long result = day13_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(13, 1));
}

// TEST(Day13Test, Part2Example) {
//   InputFile in("day13_ex");
//   long result = day13_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, 525152);
// }

// TEST(Day13Test, Part2Input) {
//   InputFile in("day13");
//   long result = day13_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, get_solution(13, 2));
// }
