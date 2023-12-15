#include "day15.h"
#include "util.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t hash(const char* str) {
  uint8_t result = 0;
  while (*str != '\0') {
    result = (result + *str) * 17;
    str++;
  }

  return result;
}

long day15_part1(const char** lines, size_t nlines) {
  assert(nlines == 1);
  size_t n;
  char** steps = split_str(lines[0], &n, ",");
  long result = 0;
  for (size_t i = 0; i < n; i++) {
    result += hash(steps[i]);
  }
  freelines(steps);
  return result;
}

typedef struct lens {
  const char* label;
  int focal_length;
  struct lens* next;
} lens_t;

typedef struct {
  lens_t* b[256];
} boxes_t;

boxes_t* boxes_create() {
  boxes_t* boxes = malloc(sizeof(boxes_t));
  for (size_t i = 0; i < 256; i++) {
    boxes->b[i] = NULL;
  }

  return boxes;
}

void boxes_free(boxes_t* boxes) {
  for (size_t i = 0; i < 256; i++) {
    lens_t* lens = boxes->b[i];
    while (lens != NULL) {
      lens_t* next = lens->next;
      free(lens);
      lens = next;
    }
  }
  free(boxes);
}

void boxes_insert(boxes_t* boxes, const char* label, int focal_length) {
  uint8_t h = hash(label);
  lens_t* l = boxes->b[h];
  if (l == NULL) {
    // No lens present yet
    lens_t* new_lens = malloc(sizeof(*new_lens));
    new_lens->label = label;
    new_lens->focal_length = focal_length;
    new_lens->next = NULL;
    boxes->b[h] = new_lens;
    return;
  }

  while (l != NULL) {
    if (strcmp(l->label, label) == 0) {
      // Lens already present, just replace the focal length
      l->focal_length = focal_length;
      return;
    }
    if (l->next == NULL) {
      // Append new lens
      lens_t* new_lens = malloc(sizeof(*new_lens));
      new_lens->label = label;
      new_lens->focal_length = focal_length;
      new_lens->next = NULL;
      l->next = new_lens;
      return;
    }
    l = l->next;
  }

  assert(0);
}

void boxes_remove(boxes_t* boxes, const char* label) {
  uint8_t h = hash(label);
  lens_t* l = boxes->b[h];
  if (l == NULL) {
    return;
  }
  if (strcmp(l->label, label) == 0) {
    // First lens is the one to remove
    boxes->b[h] = l->next;
    free(l);
    return;
  }
  lens_t* prev = l;
  l = l->next;
  for (; l != NULL; prev = l, l = l->next) {
    if (strcmp(l->label, label) == 0) {
      prev->next = l->next;
      free(l);
      return;
    }
  }
}

void boxes_print(boxes_t* boxes) {
  for (size_t i = 0; i < 256; i++) {
    lens_t* l = boxes->b[i];
    if (l == NULL) {
      continue;
    }
    printf("Box %zu:", i);
    while (l != NULL) {
      printf(" [%s %d]", l->label, l->focal_length);
      l = l->next;
    }
    printf("\n");
  }
}

typedef struct {
  const char* label;
  int focal_length;
  bool insert;
} step_t;

step_t parse_step(char* str) {
  char* sep_ptr = strpbrk(str, "-=");
  assert(sep_ptr != NULL);

  step_t step;
  step.insert = *sep_ptr == '=';
  *sep_ptr = '\0';
  step.label = str;
  step.focal_length = atoi(sep_ptr + 1);
  return step;
}

long focusing_power(boxes_t* boxes) {
  long result = 0;

  for (size_t box = 0; box < sizeof(boxes->b) / sizeof(boxes->b[0]); box++) {
    lens_t* l = boxes->b[box];
    for (size_t lens = 0; l != NULL; lens++, l = l->next) {
      result += (box + 1) * (lens + 1) * l->focal_length;
    }
  }

  return result;
}

long day15_part2(const char** lines, size_t nlines) {
  assert(nlines == 1);
  size_t n;
  char** steps = split_str(lines[0], &n, ",");
  boxes_t* boxes = boxes_create();
  for (size_t i = 0; i < n; i++) {
    // printf("After \"%s\":\n", steps[i]);
    step_t step = parse_step(steps[i]);
    if (step.insert) {
      boxes_insert(boxes, step.label, step.focal_length);
    } else {
      boxes_remove(boxes, step.label);
    }
    // boxes_print(boxes);
    // printf("\n");
  }

  long result = focusing_power(boxes);

  boxes_free(boxes);
  freelines(steps);

  return result;
}
