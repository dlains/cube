/** @file chunk.c
 *
 * @brief Implementation of the Chunk module.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include <stdlib.h>
#include "chunk.h"
#include "memory.h"
#include "value.h"

/** @brief Initialize a new Chunk dynamic array.
 *
 * Create a Chunk array to hold the bytecode for the program.
 *
 * @param chunk The Chunk structure to initialize.
 */
void init_chunk(Chunk *chunk)
{
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  init_value_array(&chunk->constants);
}

/** @brief Free a Chunk dynamic array.
 *
 * Release the memory for the given Chunk array.
 *
 * @param chunk The Chunk array to free.
 */
void free_chunk(Chunk *chunk)
{
  FREE_ARRAY(Byte, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}

/** @brief Write a bytecode and its line number to the Chunk array.
 *
 * Add a new bytecode and its associated line number to the
 * Chunk dynamic array.
 *
 * @param chunk The Chunk array to write to.
 * @param byte The new bytecode to add.
 * @param line The line number in the source code where the bytecode originated.
 */
void write_chunk(Chunk *chunk, Byte byte, int line)
{
  if(chunk->capacity < chunk->count + 1)
  {
    int old_size    = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_size);
    chunk->code     = GROW_ARRAY(chunk->code, Byte, old_size, chunk->capacity);
    chunk->lines    = GROW_ARRAY(chunk->lines, int, old_size, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

/** @brief Add a new constant value to the Value array.
 *
 * Adds a constant value to the Value array maintained in the Chunk array.
 *
 * @param chunk The Chunk array to add the value to.
 * @param value The new constant value to store.
 */
int add_constant(Chunk *chunk, Value value)
{
  write_value_array(&chunk->constants, value);
  return chunk->constants.count - 1;
}
