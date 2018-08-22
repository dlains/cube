/** @file value.c
 *
 * @brief Implementation of the ValueArray dynamic array.
 *
 * The ValueArray holds constants found in the source code for later
 * use by the compiler and virtual machine.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "object.h"
#include "value.h"

/** @brief Check if Values are equivalent.
 *
 * Check if the passed in values are equivalent.
 *
 * @param a The first Value to check.
 * @param b The second Value to check.
 * @return True if the underlying values are equivalent, false otherwise.
 */
bool values_equal(Value a, Value b)
{
  if(a.type != b.type)
    return false;

  switch(a.type)
  {
    case CUBE_BOOL:
      return AS_BOOL(a) == AS_BOOL(b);
    case CUBE_NIL:
      return true;
    case CUBE_INTEGER:
      return AS_INTEGER(a) == AS_INTEGER(b);
    case CUBE_REAL:
      return AS_REAL(a) == AS_REAL(b);
    case CUBE_OBJECT:
    {
      ObjectString* aString = AS_STRING(a);
      ObjectString* bString = AS_STRING(b);
      return aString->length == bString->length && memcmp(aString->chars, bString->chars, aString->length) == 0;
    }
  }
}

/** @brief Print the object value to stdout.
 *
 * Print the given object to stdout.
 *
 * @param value The object value to print.
 */
void print_object(Value value)
{
  switch(OBJ_TYPE(value))
  {
    case OBJ_STRING:
      printf("\"%s\"", AS_CSTRING(value));
      break;
    default:
      printf("Unknown object type passed to print_object.");
      break;
  }
}

/** @brief Print the value to stdout.
 *
 * Print the given value to stdout.
 *
 * @param value The value to print.
 */
void print_value(Value value)
{
  switch(value.type)
  {
    case CUBE_BOOL:
      printf(AS_BOOL(value) ? "true" : "false");
      break;
    case CUBE_NIL:
      printf("nil");
      break;
    case CUBE_INTEGER:
      printf("%ld", AS_INTEGER(value));
      break;
    case CUBE_REAL:
      printf("%g", AS_REAL(value));
      break;
    case CUBE_OBJECT:
      print_object(value);
      break;
  }
}

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
