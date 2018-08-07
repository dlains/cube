/** @file value.h
 *
 * @brief Implementation of the ValueArray dynamic array.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include <stdio.h>
#include "memory.h"
#include "value.h"

/** @brief Initialize a new value array.
 *
 * Allocate space for the initial value array structure.
 *
 * @param array The ValueArray structure to initialize.
 */
void init_value_array(ValueArray *array)
{
  array->capacity = 0;
  array->count = 0;
  array->values = NULL;
}

/** @brief Free the memory for the given ValueArray.
 *
 * Release the memory used for the given ValueArray.
 *
 * @param array The ValueArray to free.
 */
void free_value_array(ValueArray *array)
{
  FREE_ARRAY(Value, array->values, array->capacity);
  init_value_array(array);
}

/** @brief Write a new value to the value array.
 *
 * Add a new constant value to the given value array.
 *
 * @param array The ValueArray to add the value to.
 * @param value The constant value to add.
 */
void write_value_array(ValueArray *array, Value value)
{
  if(array->capacity < array->count + 1)
  {
    int old_size    = array->capacity;
    array->capacity = GROW_CAPACITY(old_size);
    array->values   = GROW_ARRAY(array->values, Value, old_size, array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}

/** @brief Print the value to stdout.
 *
 * Print the given value to stdout.
 *
 * @param value The value to print.
 */
void print_value(Value value)
{
  printf(" => %g", value);
}
