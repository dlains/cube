/** @file scanner.h
 *
 * @brief Scan the input source code and generate valid tokens for Cube.
 *
 * A very basic scanner. Use initScanner to set the source code to be
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
 * This initializes the Scanner struct and sets the scanning position
 * to the beginning of the source code.
 *
 * @param source The source code to be scanned.
 * @return
 */
void initScanner(const char *source);

/** @brief Get the next token from the source code.
 *
 * Get the next token from the source code. If there is a problem
 * the token type returned will be TOKEN_ERROR. If there are no more
 * tokens in the source code it will return a token type of TOKEN_EOF.
 *
 * @return Token the next token found in the source code.
 */
Token nextToken();

#endif // SCANNER_H
