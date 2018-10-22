/** @file memory.c
 *
 * @brief Implementation of the memory management functions.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include "config.h"
#include <stdlib.h>
#include "common.h"
#include "memory.h"

/** @brief Reallocate memory to the given size.
 *
 * Works in four modes:
 *   1. new_size == 0 - free the memory.
 *   2. old_size == 0 and new_size > 0 - allocate new memory.
 *   3. old_size > 0 and new_size > old_size - grow to new size.
 *   4. old_size > 0 and new_size < old_size - reduce to new size.
 *
 * If the allocation fails an error message is displayed and the program
 * exits.
 *
 * @param previous A pointer to the existing memory.
 * @param old_size The existing size of the allocated memory.
 * @param new_size The new size to reallocate the memory to.
 * @param file Diagnostic source file of the reallocate call.
 * @param line Diagnostic source file line of the reallocate call.
 * @return A pointer to the reallocated memory.
 */
void *reallocate(void *previous, size_t old_size, size_t new_size, const char *file, int line)
{
  if(new_size == 0)
  {
    free(previous);
    previous = NULL;
    return NULL;
  }

  if(new_size > old_size)
  {
    // Garbage collection will be here.
  }

  void *result = realloc(previous, new_size);
  if(result == NULL)
    die("Memory management error.", file, line);

  return result;
}
