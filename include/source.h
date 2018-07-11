/** @file source.h
 *
 * @brief The source module represents a single chunk of input source code.
 *
 * The source module represents a single chunk of input source code. The code can
 * be generated from a number of places:
 *
 * 1. A file. This is the typical case. A source file can be specified on the
 *    command line, or `import` statements can include other library files
 * 2. In the Cube REPL each line entered is stored as a source buffer.
 * 3. Source provided at the command line with the -e command will also be
 *    stored as a source buffer.
 * 
 * The source buffer provides information about the source code as well as
 * managing a rolling buffer.
 */

#ifndef SOURCE_H
#define SOURCE_H

typedef struct source_t *Source;

struct source_t {
  const char *buffer;      /**< The rolling buffer to hold the source code. */
  const char *start;       /**< The start of the current token being scanned. */
  const char *current;     /**< The current location in the buffer. */
  const char *file;        /**< The full path to the file associated with this buffer. NULL for non file buffers. */
  int line;                /**< The current line number. */
  int col;                 /**< The current column. */
};

/** @brief Create a new source buffer.
 *
 * This function creates a new source buffer from a string.
 */
Source source_create(const char *source);

/** @brief Create a new source buffer from a file.
 *
 * This function creates a new source buffer from a file name.
 */
Source source_create_file(const char *file);

/** @brief Destroy a source buffer.
 *
 * Release the memory used by this Source buffer.
 */
void source_destroy(Source s);

/** @brief Is this source buffer at the EOF marker?
 *
 * Determine if the passed in source buffer is at the
 * end of the file or not.
 *
 * @param s The source buffer to check.
 * @return True if the source buffer is at the end of the file, false otherwise.
 */
bool is_at_end(Source s);

/** @brief Skip whitespace characters between tokens.
 *
 * The next_token function starts by skipping any whitespace before
 * it attempts to find the next token. This skips spaces, tabs,
 * vertical tabs, formfeeds and newlines.
 *
 * @param s The source buffer to skip whitespace for.
 */
void skip_whitespace(Source s);

/** @brief Advance the scanner on character and return the current character.
 *
 * Move the current pointer on character forward and return
 * the character that was just being pointed to.
 *
 * @param s The source buffer to advance.
 * @return char The current character in the source code.
 */
char advance(Source s);

/** @brief Look at the current character without advancing the pointer.
 *
 * Used to check one character ahead for certain two character tokens
 * such as '>=' or '!=', ect.
 *
 * @param s The source buffer to peek into.
 * @return char The current character in the source code.
 */
char peek(Source s);

/** @brief Look at the character past the current character.
 *
 * Used when scanning numbers, peek to see if the current
 * character is a '.', then peek_next to look for additional
 * digits.
 *
 * @param s The source buffer to peek at the next char in.
 * @return char The next character in the source code.
 */
char peek_next(Source s);

/** @brief Check for an expected character in the source code.
 *
 * If the current character matches the expected character advance
 * the scanner and return true. If the current character does not
 * match return false.
 *
 * @param s The source buffer to check.
 * @param char The expected character to check for.
 * @return Boolean True if the current character matches, false othewise.
 */
bool match(Source s, char expected);

#endif // SOURCE_H
