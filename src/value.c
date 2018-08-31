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
#include "config.h"
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
    case CUBE_OBJECT:
    {
      if(OBJ_TYPE(a) != OBJ_TYPE(b))
        return false;

      switch(OBJ_TYPE(a))
      {
        case OBJ_BOOLEAN:
        {
          ObjectBoolean* aBool = AS_BOOLEAN(a);
          ObjectBoolean* bBool = AS_BOOLEAN(b);
          return aBool->value == bBool->value;
        }
        case OBJ_NIL:
        {
          // Both object have already been establised to be nil, and nil always equals nil.
          return true;
        }
        case OBJ_INTEGER:
        {
          ObjectInteger* aInt = AS_INTEGER(a);
          ObjectInteger* bInt = AS_INTEGER(b);
          return aInt->value == bInt->value;
        }
        case OBJ_REAL:
        {
          ObjectReal* aReal = AS_REAL(a);
          ObjectReal* bReal = AS_REAL(b);
          return aReal->value == bReal->value;
        }
        case OBJ_STRING:
        {
          ObjectString* aString = AS_STRING(a);
          ObjectString* bString = AS_STRING(b);
          return aString->length == bString->length && memcmp(aString->chars, bString->chars, aString->length) == 0;
        }
        default:
          return false;
      }
    }
    default:
      return false;
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
    case OBJ_BOOLEAN:
    {
      ObjectBoolean* b = AS_BOOLEAN(value);
      printf("%s", b->value ? "true" : "false");
      break;
    }
    case OBJ_NIL:
      printf("nil");
      break;
    case OBJ_INTEGER:
    {
      ObjectInteger* i = AS_INTEGER(value);
      printf("%ld", i->value);
      break;
    }
    case OBJ_REAL:
    {
      ObjectReal* r = AS_REAL(value);
      printf("%g", r->value);
      break;
    }
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
