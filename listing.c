#include "listing.h"

#define long long long
#define BASE_CAP 8
#define INTEGRITY_FLAG 0xCAFEBABEDEADBEEFFEEBDAEDEBABEFAC
#define ceildiv(a, b) (((a) + (b) - 1) / (b))

DynamicArray newDynamicArray(size_t element_size) {
  return (DynamicArray) {
    .__internal = (byte*)malloc(BASE_CAP * element_size),
    .__integrity = INTEGRITY_FLAG,
    .__element_size = element_size,
    .__capacity = BASE_CAP,
    .length = 0
  };
}

bool checkDynamicArray(DynamicArray* dyn) {
  return dyn->__integrity == INTEGRITY_FLAG;
}

Result reserveDynamicArray(DynamicArray* dyn, size_t size) {
  byte* new_ptr = (byte*)realloc(dyn->__internal, size * dyn->__element_size);
  if (new_ptr == NULL) {
    return LISTING_CANNOT_ALLOCATE;
  }
  dyn->__internal = new_ptr;
  dyn->__capacity = size;
  return LISTING_SUCCESS;
}

Result appendDynamicArray(DynamicArray* dyn, void* item) {
  if (!checkDynamicArray(dyn)) {
    return LISTING_NOT_SANE;
  }
  if (item == NULL) {
    return LISTING_BAD_ARG;
  }
  if (dyn->length == dyn->__capacity) {
    Result temp = reserveDynamicArray(dyn, 2 * dyn->__capacity);
    if (temp) {
      return temp;
    }
  }
  memcpy(dyn->__internal + dyn->length * dyn->__element_size, item, dyn->__element_size);
  dyn->length++;
  return LISTING_SUCCESS;
}

Result insertDynamicArray(DynamicArray* dyn, size_t index, void* item) {
  if (!checkDynamicArray(dyn)) {
    return LISTING_NOT_SANE;
  }
  if (item == NULL) {
    return LISTING_BAD_ARG;
  }
  if (index == dyn->length) {
    return appendDynamicArray(dyn, item);
  }
  if (index > dyn->length) {
    return LISTING_OUT_OF_BOUNDS;
  }
  if (dyn->length == dyn->__capacity) {
    Result temp = reserveDynamicArray(dyn, 2 * dyn->__capacity);
    if (temp) {
      return temp;
    }
  }
  memmove(dyn->__internal + (index + 1) * dyn->__element_size, dyn->__internal + index * dyn->__element_size, (dyn->length - index) * dyn->__element_size);
  memcpy(dyn->__internal + index * dyn->__element_size, item, dyn->__element_size);
  dyn->length++;
  return LISTING_SUCCESS;
}

Result popDynamicArray(DynamicArray* dyn) {
  if (!checkDynamicArray(dyn)) {
    return LISTING_NOT_SANE;
  }
  if (dyn->length == 0) {
    return LISTING_EMPTY;
  }
  size_t needed_cap = ceildiv((dyn->length) - 1, BASE_CAP);
  if (needed_cap != dyn->__capacity) {
    Result temp = reserveDynamicArray(dyn, needed_cap);
    if (temp) {
      return temp;
    }
  }
  dyn->length--;
  return LISTING_SUCCESS;
}

Result removeDynamicArray(DynamicArray* dyn, size_t index) {
  if (!checkDynamicArray(dyn)) {
    return LISTING_NOT_SANE;
  }
  if (index >= dyn->length) {
    return LISTING_OUT_OF_BOUNDS;
  }
  if (index == dyn->length - 1) {
    return popDynamicArray(dyn);
  } 
  memmove(dyn->__internal + index * dyn->__element_size, dyn->__internal + (index+1) * dyn->__element_size, (dyn->length - index - 1) * dyn->__element_size);
  size_t needed_cap = ceildiv(dyn->length, BASE_CAP);
  if (needed_cap != dyn->__capacity) {
    Result temp = reserveDynamicArray(dyn, needed_cap);
    if (temp) {
      return temp;
    }
  }
  dyn->length--;
  return LISTING_SUCCESS;
}

void freeDynamicArray(DynamicArray* dyn) {
  free(dyn->__internal);
  memset(dyn, 0, sizeof(DynamicArray));
}

void* getDynamicArray(DynamicArray* dyn, size_t index) {
  if (!checkDynamicArray(dyn)) {
    return NULL;
  }
  if (index >= dyn->length) {
    return NULL;
  }
  return dyn->__internal + index * dyn->__element_size;
}

Result setDynamicArray(DynamicArray* dyn, size_t index, void* item) {
  if (!checkDynamicArray(dyn)) {
    return LISTING_NOT_SANE;
  }
  if (index >= dyn->length) {
    return LISTING_OUT_OF_BOUNDS;
  }
  if (item == NULL) {
    return LISTING_BAD_ARG;
  }
  memcpy(dyn->__internal + index * dyn->__element_size, item, dyn->__element_size);
  return LISTING_SUCCESS;
}

Result clearDynamicArray(DynamicArray* dyn) {
  if (!checkDynamicArray(dyn)) {
    return LISTING_NOT_SANE;
  }
  dyn->length = 0;
  return reserveDynamicArray(dyn, BASE_CAP);
}
