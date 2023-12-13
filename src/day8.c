#include "day8.h"
#include "dynarr.h"
#include "hashmap.h"
#include "re.h"
#include "util.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
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
    re_compile(&re, "([0-9A-Z]+) = \\(([0-9A-Z]+), ([0-9A-Z]+)\\)");
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

long day8_part1(const char** lines, size_t nlines) {
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

typedef DYNARR(node_t*) node_arr;
static node_arr* simul_nodes = NULL;

static void find_simul_nodes(const void* key, void* value) {
  const char* name = key;
  if (name[strlen(name) - 1] != 'A') {
    return;
  }

  if (simul_nodes == NULL) {
    simul_nodes = dynarr_create(node_arr, 1);
  }

  node_t* node = value;
  node_t** nptr = dynarr_incsize(simul_nodes);
  *nptr = node;
}

static bool is_finished(const char* name) {
  return name[strlen(name) - 1] == 'Z';
}

long day8_part2(const char** lines, size_t nlines) {
  assert(nlines > 2);
  instruction_arr* instructions = parse_instructions(lines[0]);
  assert(instructions->size > 0);
  assert(strlen(lines[1]) == 0);

  hashmap_t* nodes = hashmap_create(hash_name, cmp_names);
  for (size_t i = 2; i < nlines; i++) {
    node_t* node = parse_node(lines[i]);
    hashmap_set(nodes, node->name, node);
  }

  hashmap_foreach(nodes, find_simul_nodes);

  printf("Simul nodes: %zu\n", simul_nodes->size);
  long* steps = malloc(sizeof(long) * simul_nodes->size);

  for (size_t i = 0; i < simul_nodes->size; i++) {
    node_t* cur = simul_nodes->data[i];
    const char* orig_name = cur->name;
    size_t inst_idx = 0;
    steps[i] = 0;
    while (!is_finished(cur->name)) {
      steps[i]++;
      instruction_t inst = instructions->data[inst_idx];
      switch (inst) {
      case LEFT:
        cur = (node_t*)hashmap_get(nodes, cur->left);
        break;
      case RIGHT:
        cur = (node_t*)hashmap_get(nodes, cur->right);
        break;
      }
      inst_idx = (inst_idx + 1) % instructions->size;
    }
    printf("Node %s: %ld\n", orig_name, steps[i]);
  }

  long result = steps[0];
  for (size_t i = 1; i < simul_nodes->size; i++) {
    result = lcm(result, steps[i]);
  }

  hashmap_foreach(nodes, free_node);
  hashmap_free(nodes);
  dynarr_free(instructions);
  dynarr_free(simul_nodes);
  simul_nodes = NULL;
  free(steps);

  return result;
}
