#include "day6.h"
#include "util.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int day6_part1(const char** lines, size_t nlines) {
  const char* time_start = "Time: ";
  const char* dist_start = "Distance: ";
  assert(nlines == 2);
  assert(strncmp(lines[0], time_start, strlen(time_start)) == 0);
  assert(strncmp(lines[1], dist_start, strlen(dist_start)) == 0);

  size_t n_times;
  long* times = parse_ints(&lines[0][strlen(time_start)], &n_times);
  size_t n_dists;
  long* dists = parse_ints(&lines[1][strlen(dist_start)], &n_dists);
  assert(n_times == n_dists);
  size_t n = n_times;

  int winning_product = 1;
  for (size_t i = 0; i < n; i++) {
    long t = times[i];
    int winning = 0;
    for (int t_hold = 0; t_hold < t; t_hold++) {
      long dist_travelled = t_hold * (t - t_hold);
      if (dist_travelled > dists[i]) {
        winning++;
      }
    }
    winning_product *= winning;
  }

  free(times);
  free(dists);
  return winning_product;
}
