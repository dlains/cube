/** @file source.c
 *
 * Implementstion of the source buffer module.
 */

#include "source.h"

struct source_t {
  const char *buffer;      /**< The rolling buffer to hold the source code. */
  const char *start;       /**< The start of the current token being scanned. */
  const char *current;     /**< The current location in the buffer. */
  const char *file;        /**< The full path to the file associated with this buffer. Will be NULL for non file buffers. */
  int line;                /**< The current line number. */
  int col;                 /**< The current column. */
};

/** @brief Create a new source buffer.
 *
 */
void source_create(const char *source)
{
}

void source_create_file(const char *file)
{
}
