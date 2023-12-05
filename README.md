# Advent of Code 2023, C Edition

Because *real* Fak IV programmers use C.

## Building

Build with CMake:

```sh
mkdir build
cd build
cmake ..
make -j4
```

## Test Suite

There is a test suite that can be run with CTest:

```sh
cd build
ctest --test-dir test --output-on-failure
```

It mostly tests the example inputs against their solution. If you save your own
inputs (e.g. for day 1 as `input/day1`) and solutions (under `input/solutions`),
the tests will also check that. The solutions format is `day part solution`, so
e.g. for days 1--3 (with the example solutions):

```
1 1 142
1 2 281
2 1 8
2 2 2286
3 1 4361
3 2 467835
```
