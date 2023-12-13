#include "day10.h"
#include "dynarr.h"

#include "assert.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { NORTH, SOUTH, WEST, EAST } dir_t;

dir_t complement(dir_t d) {
  switch (d) {
  case NORTH:
    return SOUTH;
  case SOUTH:
    return NORTH;
  case WEST:
    return EAST;
  case EAST:
    return WEST;
  default:
    assert(0);
  }
}

typedef enum { GROUND, NS, WE, NE, NW, SW, SE, START } tile_t;

tile_t parse_tile(char c) {
  switch (c) {
  case '.':
    return GROUND;
  case '|':
    return NS;
  case '-':
    return WE;
  case 'L':
    return NE;
  case 'J':
    return NW;
  case '7':
    return SW;
  case 'F':
    return SE;
  case 'S':
    return START;
  default:
    assert(0);
  }
}

char tile_to_char(tile_t t) {
  switch (t) {
  case GROUND:
    return '.';
    break;
  case NS:
    return '|';
    break;
  case WE:
    return '-';
    break;
  case NE:
    return 'L';
    break;
  case NW:
    return 'J';
    break;
  case SW:
    return '7';
    break;
  case SE:
    return 'F';
    break;
  case START:
    return 'S';
    break;
    break;
  default:
    assert(0);
  }
}

typedef struct {
  ssize_t row;
  ssize_t col;
} coord_t;

typedef struct {
  ssize_t width;
  ssize_t height;
  coord_t start;
  tile_t tiles[];
} grid_t;

#define G(grid, row, col) ((grid)->tiles[(row) * (grid)->width + (col)])

grid_t* parse_grid(const char** lines, size_t nlines) {
  assert(nlines > 0);
  ssize_t width = strlen(lines[0]) + 2;
  ssize_t height = nlines + 2;

  grid_t* grid = malloc(sizeof(*grid) + sizeof(*grid->tiles) * width * height);
  grid->height = height;
  grid->width = width;
  // Fill first and last rows/columns with GROUND to facilitate bounds checking
  for (ssize_t col = 0; col < width; col++) {
    G(grid, 0, col) = GROUND;
    G(grid, grid->height - 1, col) = GROUND;
  }
  for (ssize_t row = 0; row < height; row++) {
    G(grid, row, 0) = GROUND;
    G(grid, row, grid->width - 1) = GROUND;
  }

  bool found_start = false;
  for (size_t l = 0; l < nlines; l++) {
    assert(grid->width > 2);
    assert(strlen(lines[l]) == (size_t)(grid->width - 2));
    for (ssize_t c = 0; c < grid->width - 2; c++) {
      tile_t t = parse_tile(lines[l][c]);
      if (t == START) {
        assert(!found_start);
        found_start = true;
        grid->start.row = l + 1;
        grid->start.col = c + 1;
      }
      G(grid, l + 1, c + 1) = t;
    }
  }
  assert(found_start);

  return grid;
}

bool grid_connects(grid_t* grid, coord_t coord, dir_t dir) {
  switch (dir) {
  case NORTH: {
    assert(coord.row != 0);
    tile_t a = G(grid, coord.row, coord.col);
    tile_t b = G(grid, coord.row - 1, coord.col);
    return (a == NS || a == NE || a == NW || a == START) &&
           (b == NS || b == SE || b == SW || b == START);
  }
  case SOUTH: {
    assert(coord.row != grid->height - 1);
    coord.row++;
    return grid_connects(grid, coord, NORTH);
  }
  case WEST: {
    assert(coord.col != 0);
    tile_t a = G(grid, coord.row, coord.col);
    tile_t b = G(grid, coord.row, coord.col - 1);
    return (a == WE || a == NW || a == SW || a == START) &&
           (b == WE || b == NE || b == SE || b == START);
  }
  case EAST: {
    assert(coord.col != grid->width - 1);
    coord.col++;
    return grid_connects(grid, coord, WEST);
  }
  default:
    assert(0);
  }
}

coord_t grid_move(coord_t coord, dir_t dir) {
  switch (dir) {
  case NORTH:
    coord.row--;
    break;
  case SOUTH:
    coord.row++;
    break;
  case WEST:
    coord.col--;
    break;
  case EAST:
    coord.col++;
    break;
  default:
    assert(0);
  }
  return coord;
}

void replace_start(grid_t* grid) {
  bool connects_north = grid_connects(grid, grid->start, NORTH);
  bool connects_south = grid_connects(grid, grid->start, SOUTH);
  bool connects_west = grid_connects(grid, grid->start, WEST);
  bool connects_east = grid_connects(grid, grid->start, EAST);
  tile_t result;
  if (connects_north && connects_south) {
    result = NS;
  } else if (connects_north && connects_west) {
    result = NW;
  } else if (connects_north && connects_east) {
    result = NE;
  } else if (connects_south && connects_west) {
    result = SW;
  } else if (connects_south && connects_east) {
    result = SE;
  } else if (connects_west && connects_east) {
    result = WE;
  } else {
    assert(0);
  }
  G(grid, grid->start.row, grid->start.col) = result;
}

void print_grid(grid_t* grid) {
  for (ssize_t row = 0; row < grid->height; row++) {
    for (ssize_t col = 0; col < grid->width; col++) {
      char c = tile_to_char(G(grid, row, col));
      printf("%c", c);
    }
    printf("\n");
  }
}

long loop_length(grid_t* grid) {
  coord_t cur = grid->start;
  dir_t prev_dir = NORTH;
  long dist = 0;
  do {
    bool found_next = false;
    for (dir_t d = 0; d < 4; d++) {
      if (d == complement(prev_dir)) {
        continue;
      } else if (grid_connects(grid, cur, d)) {
        cur = grid_move(cur, d);
        prev_dir = d;
        found_next = true;
        break;
      }
    }
    assert(found_next);
    dist++;
  } while (cur.row != grid->start.row || cur.col != grid->start.col);
  return dist;
}

long day10_part1(const char** lines, size_t nlines) {
  grid_t* grid = parse_grid(lines, nlines);
  replace_start(grid);
  long result = loop_length(grid) / 2;
  free(grid);
  return result;
}

bool* mark_loop(grid_t* grid) {
  bool* result = malloc(sizeof(*result) * grid->width * grid->height);
  memset(result, 0, sizeof(*result) * grid->width * grid->height);
  coord_t cur = grid->start;
  dir_t prev_dir = NORTH;
  do {
    result[cur.row * grid->width + cur.col] = true;
    bool found_next = false;
    for (dir_t d = 0; d < 4; d++) {
      if (d == complement(prev_dir)) {
        continue;
      } else if (grid_connects(grid, cur, d)) {
        cur = grid_move(cur, d);
        prev_dir = d;
        found_next = true;
        break;
      }
    }
    assert(found_next);
  } while (cur.row != grid->start.row || cur.col != grid->start.col);
  return result;
}

void print_loop(grid_t* grid, bool* in_loop, bool* outside, ssize_t height,
                ssize_t width) {
  for (ssize_t row = 0; row < height; row++) {
    for (ssize_t col = 0; col < width; col++) {
      ssize_t idx = row * width + col;
      assert(!(in_loop[idx] && outside[idx]));
      if (in_loop[idx]) {
        printf("%c", tile_to_char(G(grid, row, col)));
      } else if (outside[idx]) {
        printf(".");
      } else {
        printf("\033[1mI\033[0m");
      }
    }
    printf("\n");
  }
}

grid_t* double_grid(grid_t* orig) {
  ssize_t height = orig->height * 2;
  ssize_t width = orig->width * 2;
  grid_t* grid = malloc(sizeof(*grid) + sizeof(*grid->tiles) * width * height);
  grid->height = height;
  grid->width = width;
  grid->start.row = orig->start.row * 2;
  grid->start.col = orig->start.col * 2;

  for (ssize_t row = 0; row < orig->height; row++) {
    for (ssize_t col = 0; col < orig->width; col++) {
      switch (G(orig, row, col)) {
      case GROUND:
        // ..
        // ..
        G(grid, row * 2 + 0, col * 2 + 0) = GROUND;
        G(grid, row * 2 + 0, col * 2 + 1) = GROUND;
        G(grid, row * 2 + 1, col * 2 + 0) = GROUND;
        G(grid, row * 2 + 1, col * 2 + 1) = GROUND;
        break;
      case NS:
        // |.
        // |.
        G(grid, row * 2 + 0, col * 2 + 0) = NS;
        G(grid, row * 2 + 0, col * 2 + 1) = GROUND;
        G(grid, row * 2 + 1, col * 2 + 0) = NS;
        G(grid, row * 2 + 1, col * 2 + 1) = GROUND;
        break;
      case WE:
        // --
        // ..
        G(grid, row * 2 + 0, col * 2 + 0) = WE;
        G(grid, row * 2 + 0, col * 2 + 1) = WE;
        G(grid, row * 2 + 1, col * 2 + 0) = GROUND;
        G(grid, row * 2 + 1, col * 2 + 1) = GROUND;
        break;
      case NE:
        // L-
        // ..
        G(grid, row * 2 + 0, col * 2 + 0) = NE;
        G(grid, row * 2 + 0, col * 2 + 1) = WE;
        G(grid, row * 2 + 1, col * 2 + 0) = GROUND;
        G(grid, row * 2 + 1, col * 2 + 1) = GROUND;
        break;
      case NW:
        // J.
        // ..
        G(grid, row * 2 + 0, col * 2 + 0) = NW;
        G(grid, row * 2 + 0, col * 2 + 1) = GROUND;
        G(grid, row * 2 + 1, col * 2 + 0) = GROUND;
        G(grid, row * 2 + 1, col * 2 + 1) = GROUND;
        break;
      case SW:
        // 7.
        // |.
        G(grid, row * 2 + 0, col * 2 + 0) = SW;
        G(grid, row * 2 + 0, col * 2 + 1) = GROUND;
        G(grid, row * 2 + 1, col * 2 + 0) = NS;
        G(grid, row * 2 + 1, col * 2 + 1) = GROUND;
        break;
      case SE:
        // F-
        // |.
        G(grid, row * 2 + 0, col * 2 + 0) = SE;
        G(grid, row * 2 + 0, col * 2 + 1) = WE;
        G(grid, row * 2 + 1, col * 2 + 0) = NS;
        G(grid, row * 2 + 1, col * 2 + 1) = GROUND;
        break;
      case START:
      default:
        assert(0);
      }
    }
  }

  return grid;
}

typedef DYNARR(coord_t) coord_arr;

long day10_part2(const char** lines, size_t nlines) {
  grid_t* orig_grid = parse_grid(lines, nlines);
  replace_start(orig_grid);
  bool* is_loop_orig = mark_loop(orig_grid);
  grid_t* grid = double_grid(orig_grid);
  print_grid(grid);
  bool* is_loop = mark_loop(grid);
  puts("");

  bool* is_outside = malloc(sizeof(*is_outside) * grid->width * grid->height);
  memset(is_outside, 0, sizeof(*is_outside) * grid->width * grid->height);

  coord_arr* coords = dynarr_create(coord_arr, 1);
  coord_t* c = dynarr_incsize(coords);
  c->row = c->col = 0;
  assert(!is_loop[0]);
  is_outside[0] = true;

  while (coords->size != 0) {
    coord_arr* next = dynarr_create(coord_arr, 1);
    for (size_t i = 0; i < coords->size; i++) {
      coord_t c = coords->data[i];
      for (dir_t d = 0; d < 4; d++) {
        coord_t n = grid_move(c, d);
        if (n.row < 0 || n.row >= grid->height || n.col < 0 ||
            n.col >= grid->width) {
          continue;
        }
        ssize_t idx = n.row * grid->width + n.col;
        if (!is_outside[idx] && !is_loop[idx]) {
          is_outside[idx] = true;
          dynarr_append(next, &n);
        }
      }
    }
    dynarr_free(coords);
    coords = next;
  }

  print_loop(grid, is_loop, is_outside, grid->height, grid->width);

  long result = 0;
  for (ssize_t row = 0; row < grid->height; row += 2) {
    for (ssize_t col = 0; col < grid->width; col += 2) {
      ssize_t idx = row * grid->width + col;
      ssize_t orig_idx = (row / 2) * orig_grid->width + (col / 2);
      if (!is_outside[idx] && !is_loop_orig[orig_idx]) {
        result++;
      }
    }
  }

  free(orig_grid);
  free(grid);
  free(is_loop_orig);
  free(is_loop);
  free(is_outside);
  dynarr_free(coords);

  return result;
}
