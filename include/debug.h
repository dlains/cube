/** @file debug.h
 *
 * @brief Some dubugging helper functions to display Chunks and instructions.
 *
 * Used with the -d / --dump flag to display the bytecode before it is
 * interpreted.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#ifndef DEBUG_H
#define DEBUG_H

#include "chunk.h"

/** @brief Disassemble the entire Chunk array of code.
 *
 * Displays the entire array of bytecode stored in the Chunk
 * array.
 *
 * @param chunk A pointer to the Chunk array to display.
 * @param name A descriptive name to describe the displayed values.
 */
void disassemble_chunk(Chunk *chunk, const char *name);

/** @brief Disassemble a single instruction.
 *
 * Disassemble a single instruction. Each bytecode found in the Chunk
 * array is passed to this function for display.
 *
 * @param chunk A pointer to the bytecode chunk to display.
 * @param i The offset to the bytecode instruction in the Chunk array.
 * @return The offset to the next bytecode instruction.
 */
int disassemble_instruction(Chunk *chunk, int i);

#endif // DEBUG_H
