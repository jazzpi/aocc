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

typedef struct {
  size_t row;
  size_t col;
} coord_t;

typedef struct {
  size_t width;
  size_t height;
  coord_t start;
  tile_t tiles[];
} grid_t;

#define G(grid, row, col) ((grid)->tiles[(row) * (grid)->width + (col)])

grid_t* parse_grid(const char** lines, size_t nlines) {
  assert(nlines > 0);
  size_t width = strlen(lines[0]) + 2;
  size_t height = nlines + 2;

  grid_t* grid = malloc(sizeof(*grid) + sizeof(*grid->tiles) * width * height);
  grid->height = height;
  grid->width = width;
  // Fill first and last rows/columns with GROUND to facilitate bounds checking
  for (size_t col = 0; col < width; col++) {
    G(grid, 0, col) = GROUND;
    G(grid, grid->height - 1, col) = GROUND;
  }
  for (size_t row = 0; row < height; row++) {
    G(grid, row, 0) = GROUND;
    G(grid, row, grid->width - 1) = GROUND;
  }

  bool found_start = false;
  for (size_t l = 0; l < nlines; l++) {
    assert(strlen(lines[l]) == grid->width - 2);
    for (size_t c = 0; c < grid->width - 2; c++) {
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
  for (size_t row = 0; row < grid->height; row++) {
    for (size_t col = 0; col < grid->width; col++) {
      char c;
      switch (G(grid, row, col)) {
      case GROUND:
        c = '.';
        break;
      case NS:
        c = '|';
        break;
      case WE:
        c = '-';
        break;
      case NE:
        c = 'L';
        break;
      case NW:
        c = 'J';
        break;
      case SW:
        c = '7';
        break;
      case SE:
        c = 'F';
        break;
      case START:
        c = 'S';
        break;
        break;
      default:
        assert(0);
      }
      printf("%c", c);
    }
    printf("\n");
  }
}

long loop_length(grid_t* grid) {
  coord_t cur = grid->start; //
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
