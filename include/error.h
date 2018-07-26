/** @file error.h
 *
 * @brief Define error handling and reporting functions.
 *
 * This module will house the error handling and reporting
 * functions that will be used by Cube. Currently only a
 * single function is defined, but over time other functions
 * will be added.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#ifndef ERROR_H
#define ERROR_H

#include "token.h"

/** @brief Stop all processing and print an error message.
 *
 * Output an error message along with the source file and
 * line number that generated the error.
 *
 * @param message An error message to be printed to stderr.
 * @param file The source file name where the error occurred.
 * @param line The source line number where the error occurred.
 */
void die(const char *message, const char *file, int line);

/** @brief Display a parse error.
 *
 * Prints an error message to stderr regarding the given token
 * and the given message.
 *
 * @param token The Token that generated the error.
 * @param message A message with details about the error.
 */
void parse_error(Token *token, const char *message);

#endif // ERROR_H
