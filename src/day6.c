#include "day6.h"
#include "dynarr.h"
#include "util.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

long day6_part1(const char** lines, size_t nlines) {
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

typedef DYNARR(char) char_arr;
long parse_int_part2(const char* str) {
  char_arr* digits = dynarr_create(char_arr, 1);
  for (; *str != '\0'; str++) {
    if (*str >= '0' && *str <= '9') {
      dynarr_append(digits, str);
    }
  }
  char nul = '\0';
  dynarr_append(digits, &nul);
  long result = atol(digits->data);
  dynarr_free(digits);
  return result;
}

long dist_travelled(long t_hold, long t) { return t_hold * (t - t_hold); }

long day6_part2(const char** lines, size_t nlines) {
  const char* time_start = "Time: ";
  const char* dist_start = "Distance: ";
  assert(nlines == 2);
  assert(strncmp(lines[0], time_start, strlen(time_start)) == 0);
  assert(strncmp(lines[1], dist_start, strlen(dist_start)) == 0);

  long time = parse_int_part2(&lines[0][strlen(time_start)]);
  long dist = parse_int_part2(&lines[1][strlen(dist_start)]);

  // d = t_hold * (t - t_hold) = t*t_hold - t_hold*t_hold
  // d > dist
  // t*t_hold - t_hold^2 > dist
  // -t_hold^2 + t*t_hold - dist > 0
  // t_hold^2 - t*t_hold + dist < 0
  // t_hold = t/2 +/- sqrt(t^2/4 - dist)
  double t_hold_min = time / 2.0 - sqrt(time * time / 4.0 - dist);
  double t_hold_max = time / 2.0 + sqrt(time * time / 4.0 - dist);
  if (fmod(t_hold_min, 1.0) == 0.0) {
    t_hold_min = t_hold_min + 1;
  } else {
    t_hold_min = ceil(t_hold_min);
  }
  if (fmod(t_hold_max, 1.0) == 0.0) {
    t_hold_max = t_hold_max - 1;
  } else {
    t_hold_max = floor(t_hold_max);
  }
  assert(dist_travelled(t_hold_min, time) > dist);
  assert(dist_travelled(t_hold_min - 1, time) <= dist);
  assert(dist_travelled(t_hold_max, time) > dist);
  assert(dist_travelled(t_hold_max + 1, time) <= dist);

  return t_hold_max - t_hold_min + 1;
}
