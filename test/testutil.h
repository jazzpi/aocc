#pragma once

#include <string>

struct InputFile {
public:
  InputFile(const std::string& filename);
  ~InputFile();

  const char** lines;
  size_t nlines;
};

long get_solution(int day, int part);
