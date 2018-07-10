/** @file compiler.h
 *
 * @brief Define the interface to the compiler module.
 *
 * Only a single function here for now. Simply call `compile` to kick
 * off the scanning and compiling stage.
 *
 * @author David J. Lains (dlains)
 * @bug No Known Bugs
 */

#ifndef COMPILER_H
#define COMPILER_H

/** @brief Compile any source code available in the scanner.
 *
 * Compiles any source code that is available in the scanner. If no source
 * code has been read the compile function just returns.
 */
void compile();

#endif // COMPILER_H
