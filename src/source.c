/** @file source.c
 *
 * Implementstion of the source buffer module.
 */

#include <stdio.h>
#include "common.h"
#include "source.h"

/** @brief Read a source file.
 *
 * Read a source file into a buffer.
 */
static char *source_read_file(const char *file_path);

/** @brief Create a new source buffer.
 *
 * Create a new source buffer from a pre-allocated buffer.
 *
 * @param source The buffer containing the source code.
 * @return Source The newly created source buffer.
 */
Source source_create(const char *source)
{
  Source s   = NEW(s);
  s->buffer  = source;
  s->start   = source;
  s->current = source;
  s->file    = NULL;
  s->line    = 1;
  s->col     = 1;

  return s;
}

/** @brief Create a source buffer from a file.
 *
 * Create a new source buffer with data from a file.
 *
 * @param file_path The full path to the file to be read.
 * @return Source The newly created source buffer.
 */
Source source_create_file(const char *file_path)
{
  Source s = NEW(s);
  s->buffer  = source_read_file(file_path);
  s->start   = s->buffer;
  s->current = s->buffer;
  s->file    = file_path;
  s->line    = 1;
  s->col     = 1;

  return s;
}

/** @brief Destroy a source buffer.
 *
 * Release the memory used by this source buffer.
 *
 * @param s The source buffer to destroy.
 */
void source_destroy(Source s)
{
  FREE(s->buffer);
  FREE(s);
}

static char *source_read_file(const char *file_path)
{
  FILE *file = fopen(file_path, "r");
  if(file == NULL)
    die("Could not open file '%s'.\n", file_path);

  fseek(file, 0L, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  char *buffer = ALLOC(size + 1);
  if(buffer == NULL)
    die("Not enough memory to read '%s'.\n", file_path);

  size_t bytes = fread(buffer, sizeof(char), size, file);
  if(bytes < size)
    die("Error reading file '%s'.\n", file_path);

  buffer[bytes] = '\0';

  fclose(file);

  return buffer;
}
