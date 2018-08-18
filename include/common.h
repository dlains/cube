/** @file common.h
 *
 * Common includes and typedefs used througout the Cude code base.
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "error.h"

/**
 * Because Byte is so much easier to type than uint8_t, and also much
 * more meaningful.
 */
typedef uint8_t Byte;

#endif // COMMON_H
