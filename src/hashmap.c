#include "hashmap.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define MAX_LOAD_FACTOR 0.75

hashmap_t* hashmap_create(hashmap_hash_fn_t hash_fn, hashmap_cmp_fn_t cmp_fn) {
  hashmap_t* map = malloc(sizeof(hashmap_t));
  map->size = 0;
  map->capacity = INITIAL_CAPACITY;
  map->entries = malloc(sizeof(*map->entries) * map->capacity);
  memset(map->entries, 0, sizeof(*map->entries) * map->capacity);
  map->hash_fn = hash_fn;
  map->cmp_fn = cmp_fn;
  return map;
}

void hashmap_free(hashmap_t* map) {
  // First, free chained entries.
  for (size_t i = 0; i < map->capacity; i++) {
    hashmap_entry_t* entry = map->entries[i].next;
    while (entry != NULL) {
      hashmap_entry_t* next = entry->next;
      free(entry);
      entry = next;
    }
  }
  free(map->entries);
  free(map);
}

void hashmap_rehash(hashmap_t* map, size_t new_capacity) {
  size_t old_capacity = map->capacity;
  hashmap_entry_t* old_entries = map->entries;

  map->capacity = new_capacity;
  map->entries = malloc(sizeof(*map->entries) * map->capacity);
  memset(map->entries, 0, sizeof(*map->entries) * map->capacity);
  size_t old_size = map->size;
  map->size = 0;

  for (size_t i = 0; i < old_capacity; i++) {
    hashmap_entry_t* entry = &old_entries[i];
    for (; entry != NULL; entry = entry->next) {
      if (entry->key != NULL) {
        hashmap_set(map, entry->key, entry->value);
      }
    }
  }
  assert(map->size == old_size);

  free(old_entries);
}

void hashmap_set(hashmap_t* map, const void* key, const void* value) {
  if ((map->size + 1) > map->capacity * MAX_LOAD_FACTOR) {
    hashmap_rehash(map, map->capacity * 2);
  }

  size_t hash = map->hash_fn(key);
  size_t index = hash % map->capacity;
  hashmap_entry_t* entry = &map->entries[index];
  if (entry->key != NULL) {
    // Check (chained) entries for a match.
    for (; entry != NULL; entry = entry->next) {
      if (map->cmp_fn(entry->key, key) == 0) {
        entry->value = value;
        return;
      }
    }
    // Collision -- chain a new entry.
    entry->next = malloc(sizeof(*entry->next));
    entry = entry->next;
  }
  entry->key = key;
  entry->value = value;
  entry->next = NULL;
  map->size++;
}

const void* hashmap_get(hashmap_t* map, const void* key) {
  size_t hash = map->hash_fn(key);
  size_t index = hash % map->capacity;
  hashmap_entry_t* entry = &map->entries[index];
  if (entry->key == NULL) {
    return NULL;
  }
  for (; entry != NULL; entry = entry->next) {
    if (map->cmp_fn(entry->key, key) == 0) {
      return entry->value;
    }
  }
  return NULL;
}

void hashmap_remove(hashmap_t* map, const void* key) {
  size_t hash = map->hash_fn(key);
  size_t index = hash % map->capacity;
  hashmap_entry_t* entry = &map->entries[index];
  if (entry->key == NULL) {
    // Key doesn't exist.
    return;
  }

  if (map->cmp_fn(entry->key, key) == 0) {
    // First entry matches.
    if (entry->next == NULL) {
      // No chained entries.
      entry->key = NULL;
      entry->value = NULL;
      map->size--;
      return;
    }
    // Copy next entry into current entry, then free next entry.
    hashmap_entry_t* next = entry->next;
    entry->key = next->key;
    entry->value = next->value;
    entry->next = next->next;
    free(next);
    map->size--;
    return;
  }

  if (entry->next == NULL) {
    // No chained entries.
    return;
  }

  // Check chained entries for a match.
  for (; entry->next != NULL; entry = entry->next) {
    if (map->cmp_fn(entry->next->key, key) == 0) {
      hashmap_entry_t* next = entry->next;
      entry->next = next->next;
      free(next);
      map->size--;
      return;
    }
  }
}
