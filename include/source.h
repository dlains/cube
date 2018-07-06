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

/** @brief Create a new source buffer.
 *
 * This function creates a new source buffer from a string.
 */
void source_create(const char *source);

/** @brief Create a new source buffer from a file.
 *
 * This function creates a new source buffer from a file name.
 */
void source_create_file(const char *file);

#endif // SOURCE_H
