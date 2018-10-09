/** @file chunk.h
 *
 * @brief Define the Chunk data structure to hold the bytecode for Cube.
 *
 * Chunk is a dynamic array of byte codes generated by the compiler during
 * the parsing phase.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "object.h"

/** @enum OpCode
 * An enumeration of the operation codes supported by Cube.
 */
typedef enum {
  OP_CONSTANT,
  OP_POP,
  OP_NIL,
  OP_GET_GLOBAL,
  OP_DEFINE_GLOBAL,
  OP_SET_GLOBAL,
  OP_EQUAL,
  OP_GREATER,
  OP_LESS,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_MODULUS,
  OP_POWER,
  OP_NOT,
  OP_NEGATE,
  OP_PRINT,
  OP_RETURN,
} OpCode;

/** @struct Chunk
 *
 * The Chunk structure defines the dynamic array of byte codes generated
 * by the compiler.
 */
typedef struct {
  int count;
  int capacity;
  Byte *code;
  int *lines;
  ObjectArray constants;
} Chunk;

/** @brief Initialize a new Chunk dynamic array.
 *
 * Create a Chunk array to hold the bytecode for the program.
 *
 * @param chunk The Chunk structure to initialize.
 */
void init_chunk(Chunk *chunk);

/** @brief Free a Chunk dynamic array.
 *
 * Release the memory for the given Chunk array.
 *
 * @param chunk The Chunk array to free.
 */
void free_chunk(Chunk *chunk);

/** @brief Write a bytecode and its line number to the Chunk array.
 *
 * Add a new bytecode and its associated line number to the
 * Chunk dynamic array.
 *
 * @param chunk The Chunk array to write to.
 * @param byte The new bytecode to add.
 * @param line The line number in the source code where the bytecode originated.
 */
void write_chunk(Chunk *chunk, Byte byte, int line);

/** @brief Add a new Object to the ObjectArray.
 *
 * Adds an object to the ObjectArray maintained in the Chunk array.
 *
 * @param chunk The Chunk array to add the object to.
 * @param object The new object to store.
 */
int add_constant(Chunk *chunk, Object *object);

#endif // CHUNK_H
