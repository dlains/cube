/** @file value.h
 *
 * @brief A dynamic array to hold constant values.
 *
 * As the parser comes across constants it saves them to
 * this array.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#ifndef VALUE_H
#define VALUE_H

#include "common.h"

typedef double Value;

/** @struct ValueArray
 *
 * A dynamic array to store constant values found in the
 * source code.
 */
typedef struct {
  int capacity;
  int count;
  Value *values;
} ValueArray;

/** @brief Initialize a new value array.
 *
 * Allocate space for the initial value array structure.
 *
 * @param array The ValueArray structure to initialize.
 */
void init_value_array(ValueArray *array);

/** @brief Free the memory for the given ValueArray.
 *
 * Release the memory used for the given ValueArray.
 *
 * @param array The ValueArray to free.
 */
void free_value_array(ValueArray *array);

/** @brief Write a new value to the value array.
 *
 * Add a new constant value to the given value array.
 *
 * @param array The ValueArray to add the value to.
 * @param value The constant value to add.
 */
void write_value_array(ValueArray *array, Value value);

/** @brief Print the value to stdout.
 *
 * Print the given value to stdout.
 *
 * @param value The value to print.
 */
void print_value(Value value);

#endif // VALUE_H
