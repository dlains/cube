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
#include "chunk.h"

/** Retrieve the ObjectType from the given object. */
#define OBJ_TYPE(value)       (AS_OBJECT(value)->type)

/** Check if the given Value is an object with OBJ_STRING type. */
#define IS_STRING(value)      is_object_type(value, OBJ_STRING)

/** Get the Value as a ObjectString pointer. */
#define AS_STRING(value)      ((ObjectString*)AS_OBJECT(value))

/** Get the Value as a raw C String pointer. */
#define AS_CSTRING(value)     (((ObjectString*)AS_OBJECT(value))->chars)

/** Enumerate the different kinds of Objects. */
typedef enum {
  OBJ_STRING,      /**< The String ObjectType. */
} ObjectType;

/** Define the object structure. */
struct object {
  ObjectType type;          /**< This objects type. */
  struct object *next;      /**< The next object in the list. */
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

/** @brief Check if the Value is an object with an particular object type.
 *
 * Do not use this function directly. Instead use the IS_* macros.
 *
 * @param value The Value to check against.
 * @param type The ObjectType to check for.
 * @return True if the Value is an object and has the specified ObjectType, false otherwise.
 */
static inline bool is_object_type(Value value, ObjectType type)
{
  return IS_OBJECT(value) && AS_OBJECT(value)->type == type;
}

#endif // OBJECT_H
