#include "dynarr.h"

#include <stdlib.h>
#include <string.h>

dynarr_handle_t* dynarr_create_handle(size_t data_size,
                                      size_t initial_capacity) {
  dynarr_handle_t* arr = malloc(sizeof(*arr));
  arr->data_size = data_size;
  arr->capacity = initial_capacity;
  arr->size = 0;
  arr->data = malloc(data_size * initial_capacity);
  return arr;
}

void* dynarr_incsize_handle(dynarr_handle_t* arr) {
  if (arr->size == arr->capacity) {
    arr->capacity *= 2;
    arr->data = realloc(arr->data, arr->data_size * arr->capacity);
  }
  arr->size += 1;
  return &arr->data[(arr->size - 1) * arr->data_size];
}

void dynarr_append_handle(dynarr_handle_t* arr, const void* item) {
  void* dest = dynarr_incsize_handle(arr);
  memcpy(dest, item, arr->data_size);
}

void dynarr_free_handle(dynarr_handle_t* arr) {
  free(arr->data);
  free(arr);
}

void* dynarr_extract_handle(dynarr_handle_t* arr) {
  void* data = arr->data;
  if (arr->data_size == 0) {
    free(data);
    data = NULL;
  } else {
    data = realloc(data, arr->size * arr->data_size);
  }
  free(arr);
  return data;
}
