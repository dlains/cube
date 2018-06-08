#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "array.h"
#include "memory.h"

struct array_type
{
  int length;
  int size;
  Byte *data;
};

static void ArrayInit(Array array, int length, int size, void *ary);

Array ArrayNew(int length, int size)
{
  Array array;

  NEW(array);
  if(length > 0)
    ArrayInit(array, length, size, CALLOC(length, size));
  else
    ArrayInit(array, length, size, NULL);

  return array;
}

static void ArrayInit(Array array, int length, int size, void *data)
{
  assert(array);
  assert((data && length > 0) || (length == 0 && data == NULL));
  assert(size > 0);

  array->length = length;
  array->size   = size;
  array->data   = (length > 0) ? data : NULL;
}

void ArrayFree(Array array)
{
  assert(array);
  FREE(array->data);
  FREE(array);
}

int ArrayLength(Array array)
{
  assert(array);
  return array->length;
}

int ArraySize(Array array)
{
  assert(array);
  return array->size;
}

void *ArrayGet(Array array, int i)
{
  assert(array);
  assert(i >= 0 && i < array->length);
  return array->data + i * array->size;
}

void *ArrayPut(Array array, int i, void *elem)
{
  assert(array);
  assert(i >= 0 && i < array->length);
  assert(elem);
  memcpy(array->data + i * array->size, elem, array->size);
  return elem;
}

void ArrayResize(Array array, int length)
{
  assert(array);
  assert(length >= 0);

  if(length == 0)
    FREE(array->data);
  else if(array->length == 0)
    array->data = ALLOC(length * array->size);
  else
    RESIZE(array->data, length * array->size);

  array->length = length;
}

Array ArrayCopy(Array array, int length)
{
  Array copy;

  assert(array);
  assert(length >= 0);

  copy = ArrayNew(length, array->size);
  if(copy->length >= array->length && array->length > 0)
    memcpy(copy->data, array->data, array->length * array->size);
  else if(array->length > copy->length && copy->length > 0)
    memcpy(copy->data, array->data, copy->length * array->size);

  return copy;
}
