#include "testutil.h"
#include "util.h"

InputFile::InputFile(const std::string& filename) {
  lines = readlines(("../../input/" + filename).c_str(), &nlines);
}

InputFile::~InputFile() { freelines(lines); }
