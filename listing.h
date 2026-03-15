#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <string.h>

#define long long long

typedef unsigned char byte;

typedef enum Result {
  LISTING_SUCCESS,
  LISTING_NOT_SANE,
  LISTING_CANNOT_ALLOCATE,
  LISTING_OUT_OF_BOUNDS,
  LISTING_EMPTY,
  LISTING_BAD_ARG,
} Result;

typedef struct DynamicArray {
  byte* __internal;
  size_t length;
  size_t __capacity;
  size_t __element_size;
  unsigned long __integrity;
} DynamicArray;

DynamicArray newDynamicArray(size_t element_size);
bool checkDynamicArray(DynamicArray* dyn);
Result reserveDynamicArray(DynamicArray* dyn, size_t size);
Result appendDynamicArray(DynamicArray* dyn, void* item);
Result insertDynamicArray(DynamicArray* dyn, size_t index, void* item);
Result popDynamicArray(DynamicArray* dyn);
Result removeDynamicArray(DynamicArray* dyn, size_t index);
void freeDynamicArray(DynamicArray* dyn);
void* getDynamicArray(DynamicArray* dyn, size_t index);
Result setDynamicArray(DynamicArray* dyn, size_t index, void* item);
Result clearDynamicArray(DynamicArray* dyn);

#undef long
