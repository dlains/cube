/** @file common.h
 *
 * Common includes and typedefs used througout the Cude code base.
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include "error.h"

/**
 * Because Byte is so much easier to type than uint8_t, and also much
 * more meaningful.
 */
typedef uint8_t Byte;

/** Make it easier to work with C Strings. */
typedef char* String;

/** @brief Create a new String.
 *
 * Allocate new memory for the C String and copy over
 * the characters so there is no issue with ownership.
 * This works for valid C Strings that are null terminated.
 *
 * @param s The C String to be copied.
 * @return The new String containing the original C String.
 */
String string_init(const char *s);

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
String string_copy(const char *s, int length);

#endif // COMMON_H
