#include "day12.h"
#include "testutil.h"

#include <gtest/gtest.h>

TEST(Day12Test, Part1Example) {
  InputFile in("day12_ex");
  long result = day12_part1(in.lines, in.nlines);
  EXPECT_EQ(result, 21);
}

TEST(Day12Test, Part1Input) {
  InputFile in("day12");
  long result = day12_part1(in.lines, in.nlines);
  EXPECT_EQ(result, get_solution(12, 1));
}

// TEST(Day12Test, Part2Example) {
//   InputFile in("day12_ex");
//   expansion_factor_part2 = 10;
//   long result = day12_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, 1030);
//   expansion_factor_part2 = 100;
//   result = day12_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, 8410);
// }

// TEST(Day12Test, Part2Input) {
//   expansion_factor_part2 = 1000000;
//   InputFile in("day12");
//   long result = day12_part2(in.lines, in.nlines);
//   EXPECT_EQ(result, get_solution(12, 2));
// }
