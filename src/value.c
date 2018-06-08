#include <stdio.h>
#include "memory.h"
#include "value.h"

void initValueArray(ValueArray *array)
{
  array->capacity = 0;
  array->count = 0;
  array->values = NULL;
}

void writeValueArray(ValueArray *array, Value value)
{
  if(array->capacity < array->count + 1)
  {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    if(array->values)
      RESIZE(array->values, sizeof(Value) * array->capacity);
    else
      array->values = ALLOC(sizeof(Value) * array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}

void freeValueArray(ValueArray *array)
{
  FREE(array->values);
  initValueArray(array);
}

void printValue(Value value)
{
  printf("%g", value);
}
