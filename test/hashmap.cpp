#include "hashmap.h"

#include <gtest/gtest.h>

size_t hash_ints(const void* key) { return *(const int*)key; }
int cmp_ints(const void* key_a, const void* key_b) {
  return *(const int*)key_a - *(const int*)key_b;
}

TEST(HashmapTest, SimpleInsertions) {
  hashmap_t* map = hashmap_create(hash_ints, cmp_ints);
  EXPECT_EQ(0, map->size);
  int zero = 0, one = 1;
  EXPECT_EQ(nullptr, hashmap_get(map, &zero));
  EXPECT_EQ(nullptr, hashmap_get(map, &one));
  EXPECT_EQ(nullptr, hashmap_get(map, "foo"));

  hashmap_set(map, &zero, "zero");
  EXPECT_EQ(1, map->size);
  hashmap_set(map, &one, "one");
  EXPECT_EQ(2, map->size);
  EXPECT_STREQ("zero", (const char*)hashmap_get(map, &zero));
  EXPECT_STREQ("one", (const char*)hashmap_get(map, &one));

  hashmap_free(map);
}

TEST(HashmapTest, ManyInsertions) {
  hashmap_t* map = hashmap_create(hash_ints, cmp_ints);

  int keys[1000];
  char values[1000][10];
  for (int i = 0; i < 1000; i++) {
    keys[i] = i;
    sprintf(values[i], "%d", i);
    hashmap_set(map, &keys[i], values[i]);
  }

  EXPECT_EQ(map->size, 1000);
  for (int i = 0; i < 1000; i++) {
    EXPECT_STREQ(values[i], (const char*)hashmap_get(map, &keys[i]));
  }

  hashmap_free(map);
}

TEST(HashmapTest, Removals) {
  hashmap_t* map = hashmap_create(hash_ints, cmp_ints);

  int keys[1000];
  char values[1000][10];
  for (int i = 0; i < 1000; i++) {
    keys[i] = i;
    sprintf(values[i], "%d", i);
    hashmap_set(map, &keys[i], values[i]);
  }

  for (int i = 0; i < 1000; i += 2) {
    hashmap_remove(map, &keys[i]);
  }

  EXPECT_EQ(map->size, 500);
  for (int i = 0; i < 1000; i++) {
    const char* value = (const char*)hashmap_get(map, &keys[i]);
    if (i % 2 == 0) {
      EXPECT_EQ(nullptr, value);
    } else {
      EXPECT_STREQ(values[i], value);
    }
  }

  hashmap_free(map);
}

size_t hash_ints_collisions(const void* key) { return *(const int*)key % 300; }

TEST(HashmapTest, Collisions) {
  hashmap_t* map = hashmap_create(hash_ints, cmp_ints);

  int keys[1000];
  char values[1000][10];
  for (int i = 0; i < 1000; i++) {
    keys[i] = i;
    sprintf(values[i], "%d", i);
    hashmap_set(map, &keys[i], values[i]);
  }

  for (int i = 0; i < 1000; i += 2) {
    hashmap_remove(map, &keys[i]);
  }

  EXPECT_EQ(map->size, 500);
  for (int i = 0; i < 1000; i++) {
    const char* value = (const char*)hashmap_get(map, &keys[i]);
    if (i % 2 == 0) {
      EXPECT_EQ(nullptr, value);
    } else {
      EXPECT_STREQ(values[i], value);
    }
  }

  hashmap_free(map);
}
