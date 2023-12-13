#include "day11.h"
#include "dynarr.h"
#include "hashmap.h"
#include "util.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t expansion_factor_part2 = 1000000;

typedef struct {
  size_t row;
  size_t col;
} coord_t;

typedef DYNARR(coord_t) coord_arr;

typedef struct {
  size_t width;
  size_t height;
  coord_arr* galaxies;
} universe_t;

size_t hash_coord(const void* key) {
  const coord_t* coord = key;
  return coord->row * 1000 + coord->col;
}

int cmp_coord(const void* a, const void* b) {
  const coord_t* coord_a = a;
  const coord_t* coord_b = b;
  if (coord_a->row < coord_b->row) {
    return -1;
  } else if (coord_a->row > coord_b->row) {
    return 1;
  } else if (coord_a->col < coord_b->col) {
    return -1;
  } else if (coord_a->col > coord_b->col) {
    return 1;
  } else {
    return 0;
  }
}

universe_t* parse_universe(const char** lines, size_t nlines) {
  assert(nlines > 0);
  universe_t* universe = malloc(sizeof(*universe));
  universe->height = nlines;
  universe->width = strlen(lines[0]);
  universe->galaxies = dynarr_create(coord_arr, 1);

  for (size_t row = 0; row < nlines; row++) {
    const char* line = lines[row];
    assert(strlen(line) == universe->width);
    for (size_t col = 0; col < universe->width; col++) {
      if (line[col] == '#') {
        coord_t* coord = dynarr_incsize(universe->galaxies);
        coord->row = row;
        coord->col = col;
      }
    }
  }

  return universe;
}

void free_universe(universe_t* universe) {
  dynarr_free(universe->galaxies);
  free(universe);
}

void print_universe(const universe_t* universe) {
  char* grid = malloc(universe->width * universe->height);
  memset(grid, '.', universe->width * universe->height);
  for (size_t i = 0; i < universe->galaxies->size; i++) {
    coord_t* coord = &universe->galaxies->data[i];
    grid[coord->row * universe->width + coord->col] = '#';
  }
  for (size_t row = 0; row < universe->height; row++) {
    for (size_t col = 0; col < universe->width; col++) {
      printf("%c", grid[row * universe->width + col]);
    }
    printf("\n");
  }
  free(grid);
}

void expand_universe(universe_t* universe, size_t expansion_factor) {
  bool occupied_rows[universe->height];
  bool occupied_cols[universe->width];
  memset(occupied_rows, 0, sizeof(occupied_rows));
  memset(occupied_cols, 0, sizeof(occupied_cols));
  for (size_t i = 0; i < universe->galaxies->size; i++) {
    coord_t* coord = &universe->galaxies->data[i];
    occupied_rows[coord->row] = true;
    occupied_cols[coord->col] = true;
  }

  size_t expansion = expansion_factor - 1;

  coord_arr* new_galaxies = dynarr_dup(universe->galaxies);
  size_t new_height = universe->height;
  for (size_t row = 0; row < universe->height; row++) {
    if (!occupied_rows[row]) {
      printf("Expanding row %zu\n", row);
      new_height += expansion;
      for (size_t i = 0; i < universe->galaxies->size; i++) {
        if (universe->galaxies->data[i].row > row) {
          new_galaxies->data[i].row += expansion;
        }
      }
    }
  }
  universe->height = new_height;
  size_t new_width = universe->width;
  for (size_t col = 0; col < universe->width; col++) {
    if (!occupied_cols[col]) {
      printf("Expanding col %zu\n", col);
      new_width += expansion;
      for (size_t i = 0; i < universe->galaxies->size; i++) {
        if (universe->galaxies->data[i].col > col) {
          new_galaxies->data[i].col += expansion;
        }
      }
    }
  }
  universe->width = new_width;

  dynarr_free(universe->galaxies);
  universe->galaxies = new_galaxies;
}

size_t** bfs_pairwise(const universe_t* universe) {
  size_t** distances = malloc(universe->galaxies->size * sizeof(*distances));
  for (size_t i = 0; i < universe->galaxies->size; i++) {
    distances[i] = malloc(universe->galaxies->size * sizeof(**distances));
    coord_t* a = &universe->galaxies->data[i];
    for (size_t j = i + 1; j < universe->galaxies->size; j++) {
      coord_t* b = &universe->galaxies->data[j];
      distances[i][j] = diffabs(a->row, b->row) + diffabs(a->col, b->col);
    }
  }

  return distances;
}

long day11_part1(const char** lines, size_t nlines) {
  universe_t* universe = parse_universe(lines, nlines);
  // printf("Initial universe:\n");
  // print_universe(universe);
  expand_universe(universe, 2);
  // printf("\nAfter expansion:\n");
  // print_universe(universe);
  size_t** distances = bfs_pairwise(universe);
  // printf("\nDistances:\n");
  long result = 0;
  for (size_t i = 0; i < universe->galaxies->size; i++) {
    for (size_t j = i + 1; j < universe->galaxies->size; j++) {
      // printf("[%3zu, %3zu] %zu\n", i, j, distances[i][j]);
      result += distances[i][j];
    }
    free(distances[i]);
  }

  free(distances);
  free_universe(universe);

  return result;
}

long day11_part2(const char** lines, size_t nlines) {
  universe_t* universe = parse_universe(lines, nlines);
  // printf("Initial universe:\n");
  // print_universe(universe);
  expand_universe(universe, expansion_factor_part2);
  // printf("\nAfter expansion:\n");
  // print_universe(universe);
  size_t** distances = bfs_pairwise(universe);
  // printf("\nDistances:\n");
  long result = 0;
  for (size_t i = 0; i < universe->galaxies->size; i++) {
    for (size_t j = i + 1; j < universe->galaxies->size; j++) {
      // printf("[%3zu, %3zu] %zu\n", i, j, distances[i][j]);
      result += distances[i][j];
    }
    free(distances[i]);
  }

  free(distances);
  free_universe(universe);

  return result;
}
