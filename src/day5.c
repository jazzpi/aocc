#include "day5.h"
#include "util.h"

#include <assert.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_MAPS 7

typedef struct {
  long source_start;
  long dest_start;
  long len;
} map_entry_t;

typedef struct {
  map_entry_t* entries;
  size_t nentries;
} map_t;

map_t* parse_map(const char** lines, size_t nlines) {
  map_t* map = malloc(sizeof(*map));
  map->entries = malloc(sizeof(*map->entries) * nlines);
  map->nentries = nlines;
  for (size_t i = 0; i < nlines; i++) {
    const char* line = lines[i];
    map_entry_t* entry = &map->entries[i];
    int ret = sscanf(line, "%ld %ld %ld", &entry->dest_start,
                     &entry->source_start, &entry->len);
    assert(ret == 3);
  }

  return map;
}

void free_map(map_t* map) {
  free(map->entries);
  free(map);
}

long map_seed(long seed, map_t* map) {
  for (size_t i = 0; i < map->nentries; i++) {
    map_entry_t* entry = &map->entries[i];
    long diff = seed - entry->source_start;
    if (diff >= 0 && diff < entry->len) {
      return entry->dest_start + diff;
    }
  }
  return seed;
}

int day5_part1(const char** lines, size_t nlines) {
  static const char* seed_start = "seeds: ";
  assert(strncmp(lines[0], seed_start, strlen(seed_start)) == 0);
  size_t n_seeds;
  long* seeds = parse_ints(&lines[0][strlen(seed_start)], &n_seeds);
  assert(n_seeds > 0);

  const char** lineptr = &lines[1];
  for (size_t i = 0; i < N_MAPS; i++) {
    // Skip empty line & map header
    assert(strlen(lineptr[0]) == 0);
    assert(strcmp(&lineptr[1][strlen(lineptr[1]) - 5], " map:") == 0);
    lineptr += 2;

    // Find number of lines in map
    const char** map_lines = lineptr;
    size_t n_map_lines = 0;
    while ((size_t)(lineptr - lines) < nlines && strlen(*lineptr) > 0) {
      lineptr++;
      n_map_lines++;
    }

    map_t* map = parse_map(map_lines, n_map_lines);
    for (size_t n = 0; n < n_seeds; n++) {
      seeds[n] = map_seed(seeds[n], map);
    }

    free_map(map);
  }

  long min_seed = seeds[0];
  for (size_t i = 1; i < n_seeds; i++) {
    if (seeds[i] < min_seed) {
      min_seed = seeds[i];
    }
  }

  free(seeds);
  return min_seed;
}
