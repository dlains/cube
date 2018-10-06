/** @file object.c
 *
 * @brief Implementation of the Object module.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include "config.h"
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "memory.h"
#include "object.h"
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
static ObjectString *allocate_string(String chars, int length);

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
    case OBJ_BOOLEAN:
    {
      FREE(ObjectBoolean, object);
      break;
    }
    case OBJ_NIL:
    {
      FREE(ObjectNil, object);
      break;
    }
    case OBJ_INTEGER:
    {
      FREE(ObjectInteger, object);
      break;
    }
    case OBJ_REAL:
    {
      FREE(ObjectReal, object);
      break;
    }
    case OBJ_STRING:
    {
      ObjectString *string = AS_STRING(object);
      FREE_ARRAY(char, string->chars, string->length + 1);
      FREE(ObjectString, object);
      break;
    }
  }
}

/** @brief Create a boolean object.
 *
 * Take the bool value and turn it into a ObjectBoolean.
 *
 * @param value The bool value to convert.
 * @return The newly created ObjectBoolean.
 */
ObjectBoolean *create_boolean(bool value)
{
  ObjectBoolean *boolean = ALLOCATE_OBJECT(ObjectBoolean, OBJ_BOOLEAN);
  boolean->value         = value;

  return boolean;
}

/** @brief Create a nil object.
 *
 * Create an ObjectNil constant.
 *
 * @return The newly created ObjectNil.
 */
ObjectNil *create_nil(void)
{
  ObjectNil *nil = ALLOCATE_OBJECT(ObjectNil, OBJ_NIL);
  nil->value     = 0;

  return nil;
}

/** @brief Create an Integer object.
 *
 * Create a new ObjectInteger value.
 *
 * @param value The integer value to convert.
 * @return The newly created ObjectInteger.
 */
ObjectInteger *create_integer(long value)
{
  ObjectInteger *integer = ALLOCATE_OBJECT(ObjectInteger, OBJ_INTEGER);
  integer->value         = value;

  return integer;
}

/** @brief Create a Real object.
 *
 * Create a new ObjectReal value.
 *
 * @param value The double value to convert.
 * @return The newly crated ObjectReal.
 */
ObjectReal *create_real(double value)
{
  ObjectReal *real = ALLOCATE_OBJECT(ObjectReal, OBJ_REAL);
  real->value      = value;

  return real;
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

  table_insert(vm.strings, AS_OBJECT(string), AS_OBJECT(string));

  return string;
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
  Object *interned = table_search_string(vm.strings, chars, length);
  if(interned != NULL)
    return AS_STRING(interned);

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
  Object *interned = table_search_string(vm.strings, chars, length);
  if(interned != NULL)
    return AS_STRING(interned);

  char *result = ALLOC(char, length + 1);
  memcpy(result, chars, length);
  result[length] = '\0';

  return allocate_string(result, length);
}

/** @brief Check if Objects are equivalent.
 *
 * Check if the passed in objects are equivalent.
 *
 * @param a The first Object to check.
 * @param b The second Object to check.
 * @return True if the underlying objects are equivalent, false otherwise.
 */
bool objects_equal(Object *a, Object *b)
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

/** @brief Print the object value to stdout.
 *
 * Print the given object to stdout.
 *
 * @param value The object value to print.
 */
void print_object(Object *object)
{
  switch(OBJ_TYPE(object))
  {
    case OBJ_BOOLEAN:
    {
      ObjectBoolean* b = AS_BOOLEAN(object);
      printf("%s", b->value ? "true" : "false");
      break;
    }
    case OBJ_NIL:
      printf("nil");
      break;
    case OBJ_INTEGER:
    {
      ObjectInteger* i = AS_INTEGER(object);
      printf("%ld", i->value);
      break;
    }
    case OBJ_REAL:
    {
      ObjectReal* r = AS_REAL(object);
      printf("%g", r->value);
      break;
    }
    case OBJ_STRING:
      printf("\"%s\"", AS_CSTRING(object));
      break;
    default:
      printf("Unknown object type passed to print_object.");
      break;
  }
}

/** @brief Calculate a hash value for the object
 *
 * Determine the type of object and calculate a hash value for it.
 *
 * @param object The object to generate the hash for.
 * @param table_size The current hash table size.
 * @return The object's hash
 */
int object_hash(Object *object, int table_size)
{
  switch(OBJ_TYPE(object))
  {
    case OBJ_BOOLEAN:
    {
      ObjectBoolean *b = AS_BOOLEAN(object);
      return string_hash(b->value ? "true" : "false", table_size);
    }
    case OBJ_NIL:
    {
      return string_hash("nil", table_size);
    }
    case OBJ_INTEGER:
    {
      ObjectInteger *i = AS_INTEGER(object);
      return ((16161 * (unsigned)i->value) % table_size);
    }
    case OBJ_REAL:
    {
      ObjectReal *r = AS_REAL(object);
      return ((int)(.616161 * (float)r->value) % table_size);
    }
    case OBJ_STRING:
    {
      char *s = AS_CSTRING(object);
      return string_hash(s, table_size);
    }
  }
}

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
int string_hash(const char *string, int table_size)
{
  int hash, a = 31415, b = 27183;
  for(hash = 0; *string; string++, a = a * b % (table_size - 1))
    hash = (a * hash + *string) % table_size;

  return hash;
}

/** @brief Initialize a new object array.
 *
 * Allocate space for the initial object array structure.
 *
 * @param array The ObjectArray structure to initialize.
 */
void init_object_array(ObjectArray *array)
{
  array->capacity = 0;
  array->count = 0;
  array->objects = NULL;
}

/** @brief Free the memory for the given ObjectArray.
 *
 * Release the memory used for the given ObjectArray.
 *
 * @param array The ObjectArray to free.
 */
void free_object_array(ObjectArray *array)
{
  FREE_ARRAY(Object, array->objects, array->capacity);
  init_object_array(array);
}

/** @brief Write a new object to the array.
 *
 * Add a new object to the given array.
 *
 * @param array The ObjectArray to add the object to.
 * @param value The object to add.
 */
void write_object_array(ObjectArray *array, Object *object)
{
  if(array->capacity < array->count + 1)
  {
    int old_size    = array->capacity;
    array->capacity = GROW_CAPACITY(old_size);
    array->objects  = GROW_ARRAY(array->objects, Object*, old_size, array->capacity);
  }

  array->objects[array->count] = object;
  array->count++;
}
