/** @file object.c
 *
 * @breif Implementation of the Object module.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

/** @brief Allocate an object of ObjectType type.
 *
 * Create an Object with the given ObjectType.
 *
 * @param size The amount of memory to allocate.
 * @param type The object type to assign to the Object.
 * @return The newly allocated Object.
 */
static Object *allocate_object(size_t size, ObjectType type);

/** @brief Allocate an object of ObjectType string.
 *
 * Create a ObjectString object and set up the length and
 * chars data.
 *
 * @param chars The raw C String characters for the ObjectString.
 * @param length The length of the C String.
 * @return The newly created ObjectString object.
 */
static ObjectString *allocate_string(char *chars, int length);

/** Helper macro for creating Objects. */
#define ALLOCATE_OBJECT(type, object_type)\
  (type*)allocate_object(sizeof(type), object_type)

/** @brief Allocate an object of ObjectType type.
 *
 * Create an Object with the given ObjectType.
 *
 * @param size The amount of memory to allocate.
 * @param type The object type to assign to the Object.
 * @return The newly allocated Object.
 */
static Object *allocate_object(size_t size, ObjectType type)
{
  Object *object = (Object*)reallocate(NULL, 0, size, __FILE__, __LINE__);
  object->type   = type;
  object->next   = vm.objects;
  vm.objects     = object;

  return object;
}

/** @brief Allocate an object of ObjectType string.
 *
 * Create a ObjectString object and set up the length and
 * chars data.
 *
 * @param chars The raw C String characters for the ObjectString.
 * @param length The length of the C String.
 * @return The newly created ObjectString object.
 */
static ObjectString *allocate_string(char *chars, int length)
{
  ObjectString *string = ALLOCATE_OBJECT(ObjectString, OBJ_STRING);
  string->length       = length;
  string->chars        = chars;

  return string;
}

/** @brief Free the Object resources.
 *
 * Free any memory associated with the Object.
 *
 * @param object The Object to free.
 */
void free_object(Object *object)
{
  switch(object->type)
  {
    case OBJ_STRING:
    {
      ObjectString *string = (ObjectString*)object;
      FREE_ARRAY(char, string->chars, string->length + 1);
      FREE(ObjectString, object);
      break;
    }
  }
}

/** @brief Take a string and turn it into a ObjectString.
 *
 * Copies the C String into an ObjectString.
 *
 * @param chars The C String to copy.
 * @param length The length of the C String.
 * @return The new ObjectString.
 */
ObjectString *take_string(char *chars, int length)
{
  return allocate_string(chars, length);
}

/** @brief Copy a string and turn it into a ObjectString.
 *
 * Copies the C String characters into a new ObjectString.
 *
 * @param chars The C String to copy.
 * @param length The length of the C String.
 * @return The new ObjectString just created.
 */
ObjectString *copy_string(const char *chars, int length)
{
  char *result = ALLOC(char, length + 1);
  memcpy(result, chars, length);
  result[length] = '\0';

  return allocate_string(result, length);
}
