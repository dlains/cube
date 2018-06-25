/** @file keywords.h
 *
 * @brief Define the keywords used by Cube and a method to identify them.
 *
 * This module is essentially a little hash table specifically
 * for the keywords used in Cube. The hash values are computed for
 * each keyword ahead of time and stored in the keywords array, along
 * with the TokenType enum value and the keyword string.
 *
 * When the scanner finds an identifier it uses find_keyword to check
 * to see if the identifier is actually a keyword. After some parameter
 * validation a hash value is generated for the input word. Then
 * the hash value is compared against the hash values for each keyword.
 * If there is a match it also verifies that the input word is really
 * the keyword. This prevents confusion in the case of a hash collision.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */

#ifndef KEYWORDS_H
#define KEYWORDS_H

#include "common.h"

/** @brief Cube has a total of 23 keywords.
 *
 * @def NUM_KEYWORDS is the number of Cube keywords.
 */
#define NUM_KEYWORDS 23

/** @brief A maximum value for the hash function to use.
 *
 * @def HASH_MAX Generate hash values between 0 and 7919.
 */
#define HASH_MAX 7919

/** @brief Find a keyword that matches the input word.
 *
 * Will generate a hash value for the input word and
 * check against the Cube keyword hash values. If there
 * is a match a further verification check is performed
 * to prevent false positives in the case of a hash collision.
 *
 * If word is NULL or zero length, or if word is longer than
 * any of the existing Cube keywords the function returns zero.
 *
 * @param word A string provided by the scanner.
 * @return An integer matching the keyword TokenType if word is a keyword, zero otherwise.
 */
int find_keyword(const char *word);

#endif // KEYWORDS_H
