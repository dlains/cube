/** @file scanner.h
 *
 * @brief Scan the input source code and generate valid tokens for Cube.
 *
 * A very basic scanner. Use init_scanner to set the source code to be
 * scanned. Then call nextToken in a loop until you see the EOF token.
 *
 * Many improvements need to be made to this code.
 *
 * 1. It currently does not handle wide characters.
 * 2. All of the source code has to be read into memory currently.
 * 3. Column numbers are not currently tracked.
 * 4. Source file names are not currently tracked.
 * 5. The skipWhitespace function doesn't skip all whitespace.
 * 6. The isAlpha and isDigit functions need to be improved.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */

#ifndef SCANNER_H
#define SCANNER_H

#include "common.h"
#include "token.h"

/** @brief Add a source line to be scanned.
 *
 * This function is used by the REPL to add a line at a time to the
 * scanner. Each line is added as a Source buffer.
 *
 * @param line The string source code input from the REPL.
 */
void add_source_line(const char *line);

/** @brief Add a source file to be scanned.
 *
 * This function creates a new Source buffer based on the supplied
 * file.
 *
 * @param file_path The full path to the source file to scan.
 */
void add_source_file(const char *file_path);

/** @brief Check if source buffers remain to be processed.
 *
 * The scanner maintains a source buffer stack. When import statements
 * are found a new source buffer is created and the current buffer
 * is pushed onto the stack. When the imported source buffer is
 * complete the original source buffer is popped off the stack and
 * scanning continues. This method returns true if there are
 * source buffers on the stack remaining to be processed.
 *
 * @return Boolean True if there are source buffers to be processed, false otherwise.
 */
bool source_buffers_remain();

/** @brief Activate the next source buffer on the stack.
 *
 * When the current source buffer reaches the end of the file and there
 * are source buffers on the stack to be processed this function will
 * activate the next source buffer and release the memory for the finished
 * buffer.
 */
void activate_next_buffer();

/** @brief Remove the current source buffer from the scanner.
 *
 * When the current source buffer reaches the end of the file this
 * function releases any memory associated with the buffer.
 */
void remove_current_buffer();

/** @brief Get the next token from the source code.
 *
 * Get the next token from the source code. If there is a problem
 * the token type returned will be TOKEN_ERROR. If there are no more
 * tokens in the source code it will return a token type of TOKEN_EOF.
 *
 * @return Token the next token found in the source code.
 */
Token next_token();

#endif // SCANNER_H
