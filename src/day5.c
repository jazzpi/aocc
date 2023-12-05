#include "day5.h"
#include "dynarr.h"
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

int map_entry_cmp(const void* avoid, const void* bvoid) {
  const map_entry_t* a = avoid;
  const map_entry_t* b = bvoid;
  long diff = a->source_start - b->source_start;
  // Just returning diff might overflow
  if (diff < 0) {
    return -1;
  } else if (diff == 0) {
    return 0;
  } else {
    return 1;
  }
}

void sort_map(map_t* map) {
  qsort(map->entries, map->nentries, sizeof(*map->entries), map_entry_cmp);
}

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

  sort_map(map);
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

typedef struct {
  long start;
  long len;
} range_t;

typedef DYNARR(range_t) range_arr;

range_arr* map_range(const range_t* range, const map_t* map) {
  range_arr* ranges = dynarr_create(range_arr, 1);

  range_t remaining_range = *range;
  for (size_t i = 0; i < map->nentries; i++) {
    map_entry_t* entry = &map->entries[i];
    long diff = remaining_range.start - entry->source_start;

    if (diff < 0) {
      // Range starts after the start of this map entry. Since the map is
      // sorted, there are no more entries that map the start of the range --
      // split it off into a new range.
      range_t* new_range = dynarr_incsize(ranges);
      new_range->start = remaining_range.start;
      new_range->len = MIN(remaining_range.len, -diff);
      remaining_range.start = entry->source_start;
      remaining_range.len -= new_range->len;
      diff = 0;

      if (remaining_range.len == 0) {
        break;
      }
    }

    if (diff >= map->entries[i].len) {
      // Range starts after the end of this map entry -- skip it.
      continue;
    }
    range_t* new_range = dynarr_incsize(ranges);
    new_range->start = entry->dest_start + diff;
    new_range->len = MIN(remaining_range.len, entry->len - diff);
    remaining_range.start += new_range->len;
    remaining_range.len -= new_range->len;

    if (remaining_range.len == 0) {
      break;
    }
  }
  if (remaining_range.len != 0) {
    range_t* new_range = dynarr_incsize(ranges);
    *new_range = remaining_range;
  }

  long sum_newlen = 0;
  for (size_t i = 0; i < ranges->size; i++) {
    sum_newlen += ranges->data[i].len;
  }
  assert(sum_newlen == range->len);

  return ranges;
}

int day5_part2(const char** lines, size_t nlines) {
  static const char* seed_start = "seeds: ";
  assert(strncmp(lines[0], seed_start, strlen(seed_start)) == 0);
  size_t n_seeds;
  long* seeds = parse_ints(&lines[0][strlen(seed_start)], &n_seeds);
  assert(n_seeds > 0);

  assert(n_seeds % 2 == 0);
  range_arr* ranges = dynarr_create(range_arr, n_seeds / 2);
  for (size_t i = 0; i < n_seeds / 2; i++) {
    range_t* r = dynarr_incsize(ranges);
    r->start = seeds[i * 2];
    r->len = seeds[i * 2 + 1];
  }
  free(seeds);

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
    range_arr* new_ranges = dynarr_create(range_arr, 1);
    for (size_t n = 0; n < ranges->size; n++) {
      range_arr* mapped_ranges = map_range(&ranges->data[n], map);
      dynarr_extend(new_ranges, mapped_ranges);
      dynarr_free(mapped_ranges);
    }
    dynarr_free(ranges);
    ranges = new_ranges;

    free_map(map);
  }

  long min_seed = ranges->data[0].start;
  for (size_t i = 0; i < ranges->size; i++) {
    if (ranges->data[i].start < min_seed) {
      min_seed = ranges->data[i].start;
    }
  }
  dynarr_free(ranges);
  return min_seed;
}
