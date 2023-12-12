#include "testutil.h"
#include "util.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>

InputFile::InputFile(const std::string& filename) {
  lines = readlines(("../../input/" + filename).c_str(), &nlines);
}

InputFile::~InputFile() { freelines(lines); }

long get_solution(int day, int part) {
  size_t nlines;
  const char** lines = readlines("../../input/solutions", &nlines);
  for (size_t i = 0; i < nlines; i++) {
    int d, p;
    long s;
    assert(sscanf(lines[i], "%d %d %ld", &d, &p, &s) == 3);
    if (d == day && p == part) {
      freelines(lines);
      return s;
    }
  }

  freelines(lines);
  fprintf(stderr, "No solution found for day %d part %d\n", day, part);
  exit(1);
}
