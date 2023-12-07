#include "day7.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day7Test, Part1Example) {
  InputFile in("day7_ex");
  int result = day7_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 6440);
}

TEST(Day7Test, Part1Input) {
  InputFile in("day7");
  int result = day7_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(7, 1));
}

// TEST(Day7Test, Part2Example) {
//   InputFile in("day7_ex");
//   int result = day7_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, 71503);
// }

// TEST(Day7Test, Part2Input) {
//   InputFile in("day7");
//   int result = day7_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, get_solution(7, 2));
// }
