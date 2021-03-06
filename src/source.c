/** @file source.c
 *
 * Implementstion of the source buffer module.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include "config.h"
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "memory.h"
#include "source.h"

/** @struct source
 *
 * Defines the source code to be parsed and interpreted.
 */
struct source {
  String buffer;           /**< The rolling buffer to hold the source code. */
  const char *start;       /**< The start of the current token being scanned. */
  const char *current;     /**< The current location in the buffer. */
  const char *file;        /**< The full path to the file associated with this buffer. NULL for non file buffers. */
  int line;                /**< The current line number. */
  int col;                 /**< The current column. */
};

/** @brief Read a source file.
 *
 * Read a source file into a buffer.
 *
 * @param file_path The full path to the file to read.
 */
static char *source_read_file(const char *file_path);

/** @brief Create a new source buffer.
 *
 * Create a new source buffer from a pre-allocated buffer.
 *
 * @param source The buffer containing the source code.
 * @return The newly created source buffer.
 */
Source source_create(const char *source)
{
  Source s   = NEW(s);
  s->buffer  = string_init(source);
  s->start   = s->buffer;
  s->current = s->buffer;
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
 * @return The newly created source buffer.
 */
Source source_create_file(const char *file_path)
{
  Source s   = NEW(s);
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
  FREE(char, s->buffer);
  FREE(Source, s);
}

/** @brief Get the start position for the token.
 *
 * Return the starting position for the current token.
 *
 * @param s The source buffer to operate on.
 * @return The starting position for the current token.
 */
const char *start_position(Source s)
{
  return s->start;
}

/** @brief Get the current line number being processed.
 *
 * Return the current line number that is being scanned in
 * the source buffer.
 *
 * @param s The source buffer to operate on.
 * @return The current line number.
 */
int line_number(Source s)
{
  return s->line;
}

/** @brief Increment the line number for this source buffer.
 *
 * Increment the line number for this source buffer.
 *
 * @param s The source buffer to operate on.
 * @return The current line number.
 */
int increment_line(Source s)
{
  s->line++;
  s->col = 1;
  return s->line;
}

/** @brief Get the current column number.
 *
 * The current column number for the start of the token.
 *
 * @param s The source buffer to operate on.
 * @return The current column number.
 */
int col_number(Source s)
{
  return s->col;
}

/** @brief Start scanning for a new token.
 *
 * Call this function at the start of a new token scan
 * to syncronize the buffers.
 *
 * @param s The source buffer to start the token for.
 */
void start_token(Source s)
{
  s->start = s->current;
}

/** @brief Get the length of the current token.
 *
 * Calculate the distance between the start and current
 * pointers. This is the length of the token.
 *
 * @param s The source buffer to get the length for.
 * @return The length of the current token.
 */
int token_length(Source s)
{
  return s->current - s->start;
}

/** @brief Is this source buffer at the EOF marker?
 *
 * Determine if the passed in source buffer is at the
 * end of the file or not.
 *
 * @param s The source buffer to check.
 * @return True if the source buffer is at the end of the file, false otherwise.
 */
bool is_at_end(Source s)
{
  return *s->current == '\0';
}

/** @brief Skip whitespace characters between tokens.
 *
 * The next_token function starts by skipping any whitespace before
 * it attempts to find the next token. This skips spaces, tabs,
 * vertical tabs, formfeeds and newlines.
 *
 * @param s The source buffer to skip whitespace for.
 */
void skip_whitespace(Source s)
{
  for(;;)
  {
    char c = peek(s);
    switch(c)
    {
      case ' ':
      case '\r':
      case '\t':
      case '\f':
      case '\v':
        advance(s);
        break;
      case '\n':
        increment_line(s);
        advance(s);
        break;
      case '#':
        while(peek(s) != '\n' && !is_at_end(s))
          advance(s);
        break;
      default:
        return;
    }
  }
}

/** @brief Advance the scanner on character and return the current character.
 *
 * Move the current pointer on character forward and return
 * the character that was just being pointed to.
 *
 * @param s The source buffer to advance.
 * @return The current character in the source code.
 */
char advance(Source s)
{
  s->current++;
  s->col++;
  return s->current[-1];
}

/** @brief Look at the current character without advancing the pointer.
 *
 * Used to check one character ahead for certain two character tokens
 * such as '>=' or '!=', ect.
 *
 * @param s The source buffer to peek into.
 * @return The current character in the source code.
 */
char peek(Source s)
{
  return *s->current;
}

/** @brief Look at the character past the current character.
 *
 * Used when scanning numbers, peek to see if the current
 * character is a '.', then peek_next to look for additional
 * digits.
 *
 * @param s The source buffer to peek at the next char in.
 * @return The next character in the source code.
 */
char peek_next(Source s)
{
  if(is_at_end(s))
    return '\0';
  return s->current[1];
}

/** @brief Check for an expected character in the source code.
 *
 * If the current character matches the expected character advance
 * the scanner and return true. If the current character does not
 * match return false.
 *
 * @param s The source buffer to check.
 * @param expected The expected character to check for.
 * @return True if the current character matches, false othewise.
 */
bool match(Source s, char expected)
{
  if(is_at_end(s))
    return false;

  if(*s->current != expected)
    return false;

  s->current++;
  s->col++;
  return true;
}

/** @brief Read a source file.
 *
 * Read a source file into a buffer.
 *
 * @param file_path The full path to the file to read.
 */
static char *source_read_file(const char *file_path)
{
  FILE *file = fopen(file_path, "r");
  if(file == NULL)
  {
    fprintf(stderr, "Could not open file '%s'.\n", file_path);
  }

  fseek(file, 0L, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  char *buffer = ALLOC(char, size + 1);
  if(buffer == NULL)
  {
    fprintf(stderr, "Not enough memory to read '%s'.\n", file_path);
  }

  size_t bytes = fread(buffer, sizeof(char), size, file);
  if(bytes < size)
  {
    fprintf(stderr, "Error reading file '%s'.\n", file_path);
  }

  buffer[bytes] = '\0';

  fclose(file);

  return buffer;
}
