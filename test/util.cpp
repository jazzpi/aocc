#include "util.h"

#include <gtest/gtest.h>
#include <vector>

void compare_parse_ints_result(const std::vector<int>& expected, int* actual,
                               size_t n_actual) {
  EXPECT_EQ(expected.size(), n_actual);
  for (size_t i = 0; i < n_actual; i++) {
    EXPECT_EQ(expected[i], actual[i]);
  }
}

TEST(UtilTest, ParseInts) {
  size_t nints;
  int* actual;

  actual = parse_ints("1 2 3 4 5", &nints);
  compare_parse_ints_result({1, 2, 3, 4, 5}, actual, nints);
  free(actual);

  actual = parse_ints("1  2     3 45", &nints);
  compare_parse_ints_result({1, 2, 3, 45}, actual, nints);
  free(actual);

  actual = parse_ints("1 -2 +3 -4", &nints);
  compare_parse_ints_result({1, -2, 3, -4}, actual, nints);
  free(actual);

  actual = parse_ints("", &nints);
  EXPECT_EQ(0, nints);
  EXPECT_EQ(nullptr, actual);
}
