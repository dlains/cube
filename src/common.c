/** @file common.c
 *
 * @brief Implementation of the common functions.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include "config.h"
#include <string.h>

#include "memory.h"
#include "common.h"

/** @brief Create a new String.
 *
 * Allocate new memory for the C String and copy over
 * the characters so there is no issue with ownership.
 *
 * @param s The C String to be copied.
 * @return The new String containing the original C String.
 */
String string_init(const char *s)
{
  if(!s)
    DIE("Invalid string pointer.");

  String result = ALLOC(char, strlen(s) + 1);
  strcpy(result, s);

  return result;
}

/** @brief Create a new String with the given length.
 *
 * Allocate new memory for the C String and copy over
 * the characters up to the given length. Can be used to
 * create a substring from a larger string.
 *
 * @param s The C String to be copied.
 * @param length The number of characters to be copied.
 * @return The new String containing the original C String.
 */
String string_copy(const char *s, int length)
{
  if(!s)
    DIE("Invalid string pointer.");

  if(length <= 0)
    DIE("String length must be larger than zero.");

  String result = ALLOC(char, length + 1);
  strncpy(result, s, length);
  result[length] = '\0';

  return result;
}


