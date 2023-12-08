#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct hashmap_entry {
  const void* key;
  const void* value;
  struct hashmap_entry* next;
} hashmap_entry_t;

typedef size_t (*hashmap_hash_fn_t)(const void* key);
typedef int (*hashmap_cmp_fn_t)(const void* key_a, const void* key_b);

typedef struct {
  size_t size;
  size_t capacity;
  hashmap_entry_t* entries;
  hashmap_hash_fn_t hash_fn;
  hashmap_cmp_fn_t cmp_fn;
} hashmap_t;

hashmap_t* hashmap_create(hashmap_hash_fn_t hash_fn, hashmap_cmp_fn_t cmp_fn);
void hashmap_free(hashmap_t* map);
void hashmap_rehash(hashmap_t* map, size_t new_capacity);

void hashmap_set(hashmap_t* map, const void* key, const void* value);
const void* hashmap_get(hashmap_t* map, const void* key);
void hashmap_remove(hashmap_t* map, const void* key);

#ifdef __cplusplus
}
#endif
