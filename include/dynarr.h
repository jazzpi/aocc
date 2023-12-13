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
 * Increase the size of the array by n elements.
 *
 * @return A pointer to the first new element.
 */
void* dynarr_incsize_n_handle(dynarr_handle_t* arr, size_t n);
#define dynarr_incsize_n(ARR, N)                                               \
  (typeof(ARR->data))dynarr_incsize_n_handle((dynarr_handle_t*)ARR, N)

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
 * Extend the array with the given array.
 */
void dynarr_extend_handle(dynarr_handle_t* dest, const dynarr_handle_t* src);
#define dynarr_extend(DEST, SRC)                                               \
  dynarr_extend_handle((dynarr_handle_t*)DEST, (dynarr_handle_t*)SRC)

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

/**
 * Duplicate an array.
 */
void* dynarr_dup_handle(const dynarr_handle_t* arr);
#define dynarr_dup(ARR) (typeof(ARR))dynarr_dup_handle((dynarr_handle_t*)ARR)

#ifdef __cplusplus
}
#endif
