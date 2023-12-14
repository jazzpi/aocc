#include "day14.h"

#include "dynarr.h"
#include "hashmap.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long day14_part1(const char** lines, size_t nlines) {
  assert(nlines > 0);
  size_t width = strlen(lines[0]);

  long result = 0;
  for (size_t col = 0; col < width; col++) {
    long stop = nlines + 1;
    for (size_t row = 0; row < nlines; row++) {
      switch (lines[row][col]) {
      case 'O':
        stop--;
        result += stop;
        break;
      case '#':
        stop = nlines - row;
        break;
      default:
        assert(lines[row][col] == '.');
      }
    }
  }

  return result;
}

typedef struct {
  ssize_t row;
  ssize_t col;
} coord_t;

int cmp_coords(const void* a, const void* b) {
  const coord_t* ca = a;
  const coord_t* cb = b;
  if (ca->row != cb->row) {
    return ca->row - cb->row;
  }
  return ca->col - cb->col;
}

typedef DYNARR(coord_t) coord_arr;
typedef DYNARR(ssize_t) ssizet_arr;

typedef struct {
  size_t height;
  size_t width;
  coord_arr* rounds;
  ssizet_arr** row_stops;
  ssizet_arr** col_stops;
} platform_t;

void platform_sort_rounds(platform_t* platform) {
  qsort(platform->rounds->data, platform->rounds->size, sizeof(coord_t),
        cmp_coords);
}

platform_t* parse_platform(const char** lines, size_t nlines) {
  platform_t* platform = malloc(sizeof(*platform));
  assert(nlines > 0);
  platform->height = nlines;
  platform->width = strlen(lines[0]);
  platform->rounds = dynarr_create(coord_arr, 1);
  platform->row_stops = malloc(sizeof(ssizet_arr*) * platform->height);
  platform->col_stops = malloc(sizeof(ssizet_arr*) * platform->width);

  for (size_t row = 0; row < platform->height; row++) {
    ssizet_arr* stops = dynarr_create(ssizet_arr, 2);
    platform->row_stops[row] = stops;

    *dynarr_incsize(stops) = -1;
    for (size_t col = 0; col < platform->width; col++) {
      coord_t coord = {.row = row, .col = col};
      switch (lines[row][col]) {
      case 'O':
        dynarr_append(platform->rounds, &coord);
        break;
      case '#':
        dynarr_append(stops, &col);
        break;
      default:
        assert(lines[row][col] == '.');
      }
    }
    *dynarr_incsize(stops) = platform->width;
  }

  for (size_t col = 0; col < platform->width; col++) {
    ssizet_arr* stops = dynarr_create(ssizet_arr, 2);
    platform->col_stops[col] = stops;

    *dynarr_incsize(stops) = -1;
    for (size_t row = 0; row < platform->height; row++) {
      if (lines[row][col] == '#') {
        dynarr_append(stops, &row);
      }
    }
    *dynarr_incsize(stops) = platform->height;
  }

  platform_sort_rounds(platform);

  return platform;
}

void free_platform(platform_t* platform) {
  dynarr_free(platform->rounds);
  for (size_t row = 0; row < platform->height; row++) {
    dynarr_free(platform->row_stops[row]);
  }
  for (size_t col = 0; col < platform->width; col++) {
    dynarr_free(platform->col_stops[col]);
  }
  free(platform->row_stops);
  free(platform->col_stops);
  free(platform);
}

void print_platform(platform_t* platform) {
  char* result =
      malloc(sizeof(*result) * ((platform->width + 1) * platform->height + 1));
  memset(result, '.', (platform->width + 1) * platform->height);
  result[(platform->width + 1) * platform->height] = '\0';

  for (size_t row = 0; row < platform->height; row++) {
    result[row * (platform->width + 1) + platform->width] = '\n';
    for (size_t i = 1; i < platform->row_stops[row]->size - 1; i++) {
      ssize_t stop = platform->row_stops[row]->data[i];
      result[row * (platform->width + 1) + stop] = '#';
    }
  }

  for (size_t i = 0; i < platform->rounds->size; i++) {
    coord_t* coord = &platform->rounds->data[i];
    char* tgt = &result[coord->row * (platform->width + 1) + coord->col];
    assert(*tgt == '.');
    *tgt = 'O';
  }

  printf("%s\n", result);

  free(result);
}

typedef enum { NORTH, EAST, SOUTH, WEST } dir_t;

void shift_platform(platform_t* platform, dir_t dir) {
  // Duplicate stops so we can modify them
  ssizet_arr** row_stops = malloc(sizeof(ssizet_arr*) * platform->height);
  for (size_t row = 0; row < platform->height; row++) {
    row_stops[row] = dynarr_dup(platform->row_stops[row]);
  }
  ssizet_arr** col_stops = malloc(sizeof(ssizet_arr*) * platform->width);
  for (size_t col = 0; col < platform->width; col++) {
    col_stops[col] = dynarr_dup(platform->col_stops[col]);
  }

  for (size_t i = 0; i < platform->rounds->size; i++) {
    coord_t* coord = &platform->rounds->data[i];
    switch (dir) {
    case NORTH: {
      ssizet_arr* stops = platform->col_stops[coord->col];
      for (size_t j = 1; j < stops->size; j++) {
        ssize_t* stop = &stops->data[j];
        assert(*stop != coord->row);
        if (*stop > coord->row) {
          ssize_t* new_stop = &col_stops[coord->col]->data[j - 1];
          (*new_stop)++;
          coord->row = *new_stop;
          break;
        }
        assert(j != stops->size - 1);
      }
      break;
    }
    case EAST: {
      ssizet_arr* stops = platform->row_stops[coord->row];
      for (ssize_t j = stops->size - 2; j >= 0; j--) {
        ssize_t* stop = &stops->data[j];
        assert(*stop != coord->col);
        if (*stop < coord->col) {
          ssize_t* new_stop = &row_stops[coord->row]->data[j + 1];
          (*new_stop)--;
          coord->col = *new_stop;
          break;
        }
        assert(j != 0);
      }
      break;
    }
    case SOUTH: {
      ssizet_arr* stops = platform->col_stops[coord->col];
      for (ssize_t j = stops->size - 2; j >= 0; j--) {
        ssize_t* stop = &stops->data[j];
        assert(*stop != coord->row);
        if (*stop < coord->row) {
          ssize_t* new_stop = &col_stops[coord->col]->data[j + 1];
          (*new_stop)--;
          coord->row = *new_stop;
          break;
        }
        assert(j != 0);
      }
      break;
    }
    case WEST: {
      ssizet_arr* stops = platform->row_stops[coord->row];
      for (size_t j = 1; j < stops->size; j++) {
        ssize_t* stop = &stops->data[j];
        assert(*stop != coord->col);
        if (*stop > coord->col) {
          ssize_t* new_stop = &row_stops[coord->row]->data[j - 1];
          (*new_stop)++;
          coord->col = *new_stop;
          break;
        }
        assert(j != stops->size - 1);
      }
      break;
    }
    default:
      assert(0);
    }
  }

  for (size_t row = 0; row < platform->height; row++) {
    dynarr_free(row_stops[row]);
  }
  for (size_t col = 0; col < platform->width; col++) {
    dynarr_free(col_stops[col]);
  }
  free(row_stops);
  free(col_stops);

  // Sort rounds to simplify hash/compare
  platform_sort_rounds(platform);
}

size_t hash_rounds(const void* p) {
  const coord_arr* rounds = p;
  size_t result = 0;
  for (size_t i = 0; i < rounds->size; i++) {
    coord_t* coord = &rounds->data[i];
    result = result * 31 + coord->row;
    result = result * 31 + coord->col;
  }
  return result;
}

int cmp_rounds(const void* a, const void* b) {
  const coord_arr* ra = a;
  const coord_arr* rb = b;
  if (ra->size != rb->size) {
    return 1;
  }
  for (size_t i = 0; i < ra->size; i++) {
    coord_t* ca = &ra->data[i];
    coord_t* cb = &rb->data[i];
    if (ca->row != cb->row || ca->col != cb->col) {
      return 1;
    }
  }
  return 0;
}

void free_data(const void* key, void* value) {
  dynarr_free((coord_arr*)key);
  free(value);
}

long calculate_load(const platform_t* platform) {
  long result = 0;
  for (size_t i = 0; i < platform->rounds->size; i++) {
    coord_t* coord = &platform->rounds->data[i];
    result += platform->height - coord->row;
  }
  return result;
}

#define CYCLES 1000000000L

long day14_part2(const char** lines, size_t nlines) {
  platform_t* platform = parse_platform(lines, nlines);
  hashmap_t* seen = hashmap_create(hash_rounds, cmp_rounds);
  for (size_t i = 0; i < CYCLES; i++) {
    const size_t* already_seen = hashmap_get(seen, platform->rounds);
    if (already_seen == NULL) {
      size_t* it_alloc = malloc(sizeof(*it_alloc));
      *it_alloc = i;
      coord_arr* rounds_alloc = dynarr_dup(platform->rounds);
      hashmap_set(seen, rounds_alloc, it_alloc);
    } else {
      size_t cycle = i - *already_seen;
      if (i + cycle < CYCLES) {
        printf("Found a %ld cycle after %ld iterations\n", cycle, i);
        i += (CYCLES - i) / cycle * cycle;
      }
    }
    shift_platform(platform, NORTH);
    shift_platform(platform, WEST);
    shift_platform(platform, SOUTH);
    shift_platform(platform, EAST);
  }

  // printf("Final platform:\n");
  // print_platform(platform);
  long result = calculate_load(platform);

  free_platform(platform);
  hashmap_foreach(seen, free_data);
  hashmap_free(seen);

  return result;
}
