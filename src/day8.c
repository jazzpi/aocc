#include "day8.h"
#include "dynarr.h"
#include "hashmap.h"
#include "re.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef enum { LEFT, RIGHT } instruction_t;

typedef DYNARR(instruction_t) instruction_arr;

instruction_arr* parse_instructions(const char* line) {
  instruction_arr* instructions = dynarr_create(instruction_arr, strlen(line));
  for (; *line != '\0'; line++) {
    instruction_t* inst = dynarr_incsize(instructions);
    switch (*line) {
    case 'L':
      *inst = LEFT;
      break;
    case 'R':
      *inst = RIGHT;
      break;
    default:
      assert(0);
    }
  }

  return instructions;
}

typedef struct {
  const char* name;
  const char* left;
  const char* right;
} node_t;

size_t hash_name(const void* n) {
  const char* name = n;

  size_t hash = 0;
  for (size_t i = 0; name[i] != '\0'; i++) {
    hash = 31 * hash + name[i];
  }
  return hash;
}

int cmp_names(const void* avoid, const void* bvoid) {
  return strcmp(avoid, bvoid);
}

node_t* parse_node(const char* line) {
  static regex_t re = {.re_nsub = 0};
  if (re.re_nsub == 0) {
    re_compile(&re, "([A-Z]+) = \\(([A-Z]+), ([A-Z]+)\\)");
  }
  size_t nmatches;
  re_match_t* matches = re_match_all(&re, line, &nmatches, 0);
  assert(nmatches == 1);

  node_t* node = malloc(sizeof(*node));
  node->name = strdup(matches[0].groups[1].str);
  node->left = strdup(matches[0].groups[2].str);
  node->right = strdup(matches[0].groups[3].str);
  re_free_matches(matches, nmatches);

  return node;
}

void free_node(const void* key, void* nodeptr) {
  (void)key; // Suppress unused argument warning.

  node_t* node = nodeptr;
  free((void*)node->name);
  free((void*)node->left);
  free((void*)node->right);
  free(node);
}

int day8_part1(const char** lines, size_t nlines) {
  assert(nlines > 2);
  instruction_arr* instructions = parse_instructions(lines[0]);
  assert(instructions->size > 0);
  assert(strlen(lines[1]) == 0);

  hashmap_t* nodes = hashmap_create(hash_name, cmp_names);
  for (size_t i = 2; i < nlines; i++) {
    node_t* node = parse_node(lines[i]);
    hashmap_set(nodes, node->name, node);
  }

  const node_t* cur = hashmap_get(nodes, "AAA");
  size_t inst_idx = 0;
  int steps = 0;
  while (assert(cur != NULL), strcmp(cur->name, "ZZZ") != 0) {
    steps++;
    instruction_t inst = instructions->data[inst_idx];
    switch (inst) {
    case LEFT:
      cur = hashmap_get(nodes, cur->left);
      break;
    case RIGHT:
      cur = hashmap_get(nodes, cur->right);
      break;
    }
    inst_idx = (inst_idx + 1) % instructions->size;
  }

  hashmap_foreach(nodes, free_node);
  hashmap_free(nodes);
  dynarr_free(instructions);

  return steps;
}
