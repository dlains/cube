/** @file error.c
 *
 * @brief The implementation of the error handling functions.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

/** @brief Display an error message and stop processing.
 *
 * Used in cases where recovery is not on option, typically
 * when checking for NULL values returned from memory allocation
 * routines.
 *
 * @param message The error message to display.
 * @param file The source file name where the error occurred.
 * @param line The source file line where the error occurred.
 */
void die(const char *message, const char *file, int line)
{
  fprintf(stderr, "Cube error");
  if(message)
    fprintf(stderr, " %s", message);

  if(file && line > 0)
    fprintf(stderr, " at %s:%d\n", file, line);

  fprintf(stderr, "Aborting...\n");
  fflush(stderr);
  abort();
}
