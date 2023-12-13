#include "util.h"

#include "dynarr.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#define FILEREAD_BUFSIZE 1024

const char** readlines(const char* filename, size_t* nlines) {
  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Failed to open %s: %s\n", filename, strerror(errno));
    exit(1);
  }

  size_t chars_read = 0;
  size_t buf_size = FILEREAD_BUFSIZE;
  char* buf = malloc(buf_size);
  while (!feof(f) && !ferror(f)) {
    size_t remaining = buf_size - chars_read;
    if (remaining == 0) {
      buf_size *= 2;
      buf = realloc(buf, buf_size);
      remaining = buf_size - chars_read;
    }
    chars_read += fread(buf + chars_read, 1, remaining, f);
  }

  if (ferror(f)) {
    fprintf(stderr, "Failed to read %s: %s\n", filename, strerror(errno));
    exit(1);
  }
  if (chars_read == 0) {
    fprintf(stderr, "[WARNING] %s is empty\n", filename);
  } else if (buf[chars_read - 1] != '\n') {
    fprintf(stderr, "[WARNING] %s doesn't end in a LF\n", filename);
  }

  *nlines = 0;
  for (size_t i = 0; i < chars_read; i++) {
    if (buf[i] == '\n') {
      *nlines += 1;
    }
  }

  const char** lines = malloc(sizeof(const char*) * *nlines);
  size_t line = 0;
  size_t line_begin = 0;
  size_t i = 0;
  while (i != chars_read) {
    if (buf[i] == '\n') {
      buf[i] = 0;
      lines[line++] = buf + line_begin;
      line_begin = i + 1;
    }
    i++;
  }

  return lines;
}

void freelines(const char** lines) {
  // The lines pointers simply point into the file buffer (where newlines have
  // been replaced by NULs), so the first line pointer just points to the file
  // buffer.
  free((void*)lines[0]);
  free(lines);
}

typedef DYNARR(long) long_arr;
long* parse_ints(const char* str, size_t* n) {
  long_arr* arr = dynarr_create(long_arr, 1);
  const char* ptr = str;
  errno = 0;
  while (*ptr != 0) {
    char* endptr;
    long num = strtol(ptr, &endptr, 10);
    assert(errno == 0);
    ptr = endptr;
    dynarr_append(arr, &num);
  }

  *n = arr->size;
  return dynarr_extract(arr);
}

long gcd(long a, long b) {
  if (a < b) {
    long tmp = a;
    a = b;
    b = tmp;
  }

  while (b != 0) {
    long rem = a % b;
    a = b;
    b = rem;
  }

  return a;
}

long lcm(long a, long b) { return labs(a) / gcd(a, b) * labs(b); }

size_t diffabs(size_t a, size_t b) { return (a > b) ? a - b : b - a; }
