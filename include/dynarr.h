#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define DYNARR(TYPE)                                                           \
  struct {                                                                     \
    TYPE* data;                                                                \
    size_t size;                                                               \
    size_t capacity;                                                           \
    size_t data_size;                                                          \
  }

typedef DYNARR(void) dynarr_handle_t;

/**
 * Create a new dynamic array.
 *
 * @param data_size The size of each element in the array.
 * @param initial_capacity The initial capacity of the array (in number of
 * elements, not bytes).
 *
 * @return A pointer to the new array. Must be freed with dynarr_free() or
 * dynarr_extract() + free().
 */
dynarr_handle_t* dynarr_create_handle(size_t data_size,
                                      size_t initial_capacity);
#define dynarr_create(DYNARR_TYPE, INITIAL_CAPACITY)                           \
  (DYNARR_TYPE*)dynarr_create_handle(sizeof(((DYNARR_TYPE*)0)->data[0]),       \
                                     INITIAL_CAPACITY)

/**
 * Increase the size of the array by one element.
 *
 * @return A pointer to the new element.
 */
void* dynarr_incsize_handle(dynarr_handle_t* arr);
#define dynarr_incsize(ARR)                                                    \
  (typeof(ARR->data))dynarr_incsize_handle((dynarr_handle_t*)ARR)

/**
 * Append an element to the end of the array.
 *
 * Increases the size of the array by one element and copies the given item into
 * it.
 */
void dynarr_append_handle(dynarr_handle_t* arr, const void* item);
#define dynarr_append(ARR, ITEM)                                               \
  dynarr_append_handle((dynarr_handle_t*)ARR, ITEM)

/**
 * Free the memory used by the array (handle and data).
 */
void dynarr_free_handle(dynarr_handle_t* arr);
#define dynarr_free(ARR) dynarr_free_handle((dynarr_handle_t*)ARR)

/**
 * Free the memory used by the array handle and extract the data.
 *
 * @note The returned data pointer must be freed manually.
 */
void* dynarr_extract_handle(dynarr_handle_t* arr);
#define dynarr_extract(ARR)                                                    \
  (typeof(ARR->data))dynarr_extract_handle((dynarr_handle_t*)ARR)

#ifdef __cplusplus
}
#endif
