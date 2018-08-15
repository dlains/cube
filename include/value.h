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

/** @enum ValueType
 *
 * Keep track of the actual type for a Value.
 */
typedef enum {
  CUBE_BOOL,     /**< Boolean (true / false) */
  CUBE_NIL,      /**< Nil, no value */
  CUBE_INTEGER,  /**< Signed integers */
  CUBE_REAL,     /**< Signed floating point values */
} ValueType;

/** @struct Value
 *
 * Tagged union for the value types that Cube supports.
 */
typedef struct {
  ValueType type;   /**< The ValueType for this value. */
  union {
    bool boolean;
    long integer;
    double real;
  } as;             /** The actual value stored in the correct type. */
} Value;

//
// Create Value helper macros.
//

/** Create a Value with type CUBE_BOOL */
#define BOOL_VAL(value)      ((Value){ CUBE_BOOL,    { .boolean = value } })

/** Create a Value with type CUBE_NIL */
#define NIL_VAL              ((Value){ CUBE_NIL,     { .real = 0 } })

/** Create a Value with type CUBE_INTEGER */
#define INTEGER_VAL(value)   ((Value){ CUBE_INTEGER, { .integer = value } })

/** Create a Value with type CUBE_REAL */
#define REAL_VAL(value)      ((Value){ CUBE_REAL,    { .real = value } })

//
// Get Value helper macros.
//

/** Extract a CUBE_BOOL value from a Value. */
#define AS_BOOL(value)       ((value).as.boolean)

/** Extract a CUBE_INTEGER value from a Value. */
#define AS_INTEGER(value)    ((value).as.integer)

/** Extract a CUBE_REAL value from a Value. */
#define AS_REAL(value)       ((value).as.real)

//
// Check ValueType helper macros.
//

/** Check if Value is a CUBE_BOOL. */
#define IS_BOOL(value)       ((value).type == CUBE_BOOL)

/** Check if Value is a CUBE_NIL. */
#define IS_NIL(value)        ((value).type == CUBE_NIL)

/** Check if Value is a CUBE_INTEGER. */
#define IS_INTEGER(value)    ((value).type == CUBE_INTEGER)

/** Check if Value is a CUBE_REAL. */
#define IS_REAL(value)       ((value).type == CUBE_REAL)

/** Check if Value is a CUBE_INTEGER or CUBE_REAL. */
#define IS_NUMBER(value)     ((value).type == CUBE_INTEGER || (value).type == CUBE_REAL)

/** @brief Check if Values are equivalent.
 *
 * Check if the passed in values are equivalent.
 *
 * @param a The first Value to check.
 * @param b The second Value to check.
 * @return True if the underlying values are equivalent, false otherwise.
 */
bool values_equal(Value a, Value b);

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
