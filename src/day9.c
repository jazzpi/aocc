#include "day9.h"

#include "dynarr.h"
#include "util.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef DYNARR(long) long_arr;

static bool is_zero(long* seq, size_t nseq) {
  for (size_t i = 0; i < nseq; i++) {
    if (seq[i] != 0) {
      return false;
    }
  }
  return true;
}

static long* diff(long* seq, size_t nseq) {
  long* result = malloc(sizeof(*result) * (nseq - 1));

  for (size_t i = 0; i < nseq - 1; i++) {
    result[i] = seq[i + 1] - seq[i];
  }

  return result;
}

long day9_part1(const char** lines, size_t nlines) {
  long sum = 0;
  for (size_t l = 0; l < nlines; l++) {
    size_t nseq;
    long* seq = parse_ints(lines[l], &nseq);
    long_arr* last_longs = dynarr_create(long_arr, 1);
    dynarr_append(last_longs, &seq[nseq - 1]);
    while (!is_zero(seq, nseq)) {
      assert(nseq > 0);
      long* next = diff(seq, nseq);
      free(seq);
      seq = next;
      nseq--;
      dynarr_append(last_longs, &seq[nseq - 1]);
    }

    assert(last_longs->data[last_longs->size - 1] == 0);
    long n = 0;
    for (ssize_t i = last_longs->size - 2; i >= 0; i--) {
      n += last_longs->data[i];
    }
    sum += n;

    free(seq);
    dynarr_free(last_longs);
  }

  return sum;
}
