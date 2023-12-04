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

dynarr_handle_t* dynarr_create_handle(size_t data_size,
                                      size_t initial_capacity);
#define dynarr_create(DYNARR_TYPE, INITIAL_CAPACITY)                           \
  (DYNARR_TYPE*)dynarr_create_handle(sizeof(((DYNARR_TYPE*)0)->data[0]),       \
                                     INITIAL_CAPACITY)
void* dynarr_incsize_handle(dynarr_handle_t* arr);
#define dynarr_incsize(ARR)                                                    \
  (typeof(ARR->data))dynarr_incsize_handle((dynarr_handle_t*)ARR)
void dynarr_append_handle(dynarr_handle_t* arr, const void* item);
#define dynarr_append(ARR, ITEM)                                               \
  dynarr_append_handle((dynarr_handle_t*)ARR, ITEM)
void dynarr_free_handle(dynarr_handle_t* arr);
#define dynarr_free(ARR) dynarr_free_handle((dynarr_handle_t*)ARR)
void* dynarr_extract_handle(dynarr_handle_t* arr);
#define dynarr_extract(ARR)                                                    \
  (typeof(ARR->data))dynarr_extract_handle((dynarr_handle_t*)ARR)

#ifdef __cplusplus
}
#endif
