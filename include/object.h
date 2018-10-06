/** @file object.h
 *
 * @brief Define the Cube object type.
 *
 * Definitions and helper macros for dealing with the Object type.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#ifndef OBJECT_H
#define OBJECT_H

#include "common.h"

typedef struct object Object;
typedef struct object_boolean ObjectBoolean;
typedef struct object_nil ObjectNil;
typedef struct object_integer ObjectInteger;
typedef struct object_real ObjectReal;
typedef struct object_string ObjectString;

/** Retrieve the ObjectType from the given object. */
#define OBJ_TYPE(object)      (object)->type

/** Cast the concrete object to a bare object. */
#define AS_OBJECT(object)     ((Object*)object)

/** Check if the given object is a Boolean object. */
#define IS_BOOLEAN(object)    ((object)->type == OBJ_BOOLEAN)

/** Check if the given object is a Nil object. */
#define IS_NIL(object)        ((object)->type == OBJ_NIL)

/** Check if the given Value is an object with OBJ_INTEGER type. */
#define IS_INTEGER(object)    ((object)->type == OBJ_INTEGER)

/** Check if the given object is a Real object. */
#define IS_REAL(object)       ((object)->type == OBJ_REAL)

/** Check if the given object is a Real or Integer object. */
#define IS_NUMBER(object)     (((object)->type == OBJ_INTEGER) || ((object)->type == OBJ_REAL))

/** Check if the given Value is an object with OBJ_STRING type. */
#define IS_STRING(object)     ((object)->type == OBJ_STRING)

/** Get the Object as a ObjectBoolean pointer. */
#define AS_BOOLEAN(value)     ((ObjectBoolean*)value)

/** Get the Object as a ObjectNil pointer. */
#define AS_NIL(value)         ((ObjectNil*)value)

/** Get the Value as a ObjectInteger pointer. */
#define AS_INTEGER(value)     ((ObjectInteger*)value)

/** Get the Value as a ObjectReal pointer. */
#define AS_REAL(value)        ((ObjectReal*)value)

/** Get the Value as a ObjectString pointer. */
#define AS_STRING(value)      ((ObjectString*)value)

/** Get the Value as a raw C String pointer. */
#define AS_CSTRING(value)     (((ObjectString*)value)->chars)

/** Enumerate the different kinds of Objects. */
typedef enum {
  OBJ_BOOLEAN,     /**< The Boolean ObjectType. */
  OBJ_NIL,         /**< The Nil ObjectType. */
  OBJ_INTEGER,     /**< The Integer ObjectType. */
  OBJ_REAL,        /**< The Real ObjectType. */
  OBJ_STRING,      /**< The String ObjectType. */
} ObjectType;

/** Define the object structure. */
struct object {
  ObjectType type;          /**< This objects type. */
  struct object *next;      /**< The next object in the list. */
};

/** Define the object_boolean structure. */
struct object_boolean {
  Object object;            /**< The actual object pointer. */
  bool value;               /**< The boolean value. */
};

/** Define the object_nil structure. */
struct object_nil {
  Object object;            /**< The actual object pointer. */
  int value;                /**< Nil is represented as int = 0. */
};

/** Define the object_integer structure. */
struct object_integer {
  Object object;            /**< The actual object pointer. */
  long value;               /**< The long int value. */
};

/** Define the object_real structure. */
struct object_real {
  Object object;            /**< The actual object pointer. */
  double value;             /**< The double floating point value. */
};

/** Define the object_string structure. */
struct object_string {
  Object object;            /**< The actual object pointer. */
  int length;               /**< The length of the string object. */
  char *chars;              /**< The raw C String pointer. */
};

/** @brief Free the Object resources.
 *
 * Free any memory associated with the Object.
 *
 * @param object The Object to free.
 */
void free_object(Object *object);

/** @brief Create a boolean object.
 *
 * Take the bool value and turn it into a ObjectBoolean.
 *
 * @param value The bool value to convert.
 * @return The newly created ObjectBoolean.
 */
ObjectBoolean *create_boolean(bool value);

/** @brief Create a nil object.
 *
 * Create an ObjectNil constant.
 *
 * @return The newly created ObjectNil.
 */
ObjectNil *create_nil(void);

/** @brief Create an Integer object.
 *
 * Create a new ObjectInteger value.
 *
 * @param value The integer value to convert.
 * @return The newly created ObjectInteger.
 */
ObjectInteger *create_integer(long value);

/** @brief Create a Real object.
 *
 * Create a new ObjectReal value.
 *
 * @param value The double value to convert.
 * @return The newly crated ObjectReal.
 */
ObjectReal *create_real(double value);

/** @brief Take a string and turn it into a ObjectString.
 *
 * Copies the C String into an ObjectString.
 *
 * @param chars The C String to copy.
 * @param length The length of the C String.
 * @return The new ObjectString.
 */
ObjectString *take_string(char *chars, int length);

/** @brief Copy a string and turn it into a ObjectString.
 *
 * Copies the C String characters into a new ObjectString.
 *
 * @param chars The C String to copy.
 * @param length The length of the C String.
 * @return The new ObjectString just created.
 */
ObjectString *copy_string(const char *chars, int length);

/** @brief Check if Objects are equivalent.
 *
 * Check if the passed in objects are equivalent.
 *
 * @param a The first Object to check.
 * @param b The second Object to check.
 * @return True if the underlying Objects are equivalent, false otherwise.
 */
bool objects_equal(Object *a, Object *b);

/** @brief Print the object value to stdout.
 *
 * Print the given object to stdout.
 *
 * @param value The object value to print.
 */
void print_object(Object *object);

/** @brief Calculate a hash value for the object
 *
 * Determine the type of object and calculate a hash value for it.
 *
 * @param object The object to generate the hash for.
 * @param table_size The current hash table size.
 * @return The object's hash
 */
int object_hash(Object *object, int table_size);

/** @brief Generate a hash value for a string.
 *
 * Three kinds of objects generate a hash with a C string,
 * OBJ_STRING, OBJ_BOOLEAN, and OBJ_NIL. This function handles
 * all three cases.
 *
 * @param string The C string to hash
 * @param table_size The current hash table size.
 * @return The hash of the string.
 */
int string_hash(const char *string, int table_size);

/** @struct ObjectArray
 *
 * A dynamic array to store objects found in the
 * source code.
 */
typedef struct {
  int capacity;
  int count;
  Object **objects;
} ObjectArray;

/** @brief Initialize a new object array.
 *
 * Allocate space for the initial object array structure.
 *
 * @param array The ObjectArray structure to initialize.
 */
void init_object_array(ObjectArray *array);

/** @brief Free the memory for the given ObjectArray.
 *
 * Release the memory used for the given ObjectArray.
 *
 * @param array The ObjectArray to free.
 */
void free_object_array(ObjectArray *array);

/** @brief Write a new object to the array.
 *
 * Add a new object to the given array.
 *
 * @param array The ObjectArray to add the value to.
 * @param value The object to add.
 */
void write_object_array(ObjectArray *array, Object *object);

#endif // OBJECT_H
