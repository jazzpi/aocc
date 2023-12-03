#pragma once

#include <stddef.h>
#include <string.h>

#define MAIN_BOILERPLATE                                                       \
  if (argc != 3) {                                                             \
    usage_abrt(argv[0]);                                                       \
  }                                                                            \
  int part;                                                                    \
  if (strcmp(argv[1], "p1") == 0) {                                            \
    part = 1;                                                                  \
  } else if (strcmp(argv[1], "p2") == 0) {                                     \
    part = 2;                                                                  \
  } else {                                                                     \
    usage_abrt(argv[0]);                                                       \
  }                                                                            \
  size_t nlines;                                                               \
  const char **lines = readlines(argv[2], &nlines);

const char **readlines(const char *filename, size_t *nlines);
void freelines(const char **lines);

__attribute__((noreturn)) void usage_abrt(char *progname);
