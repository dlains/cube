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

/** @brief Initialize the scanner with a pointer to the source code.
 *
 * TODO: This will be removed once the add_source* functions are working.
 * This initializes the Scanner struct and sets the scanning position
 * to the beginning of the source code.
 *
 * @param source The source code to be scanned.
 */
void init_scanner(const char *source);

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
