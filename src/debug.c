/** @file debug.c
 *
 * @brief Implementation of disassembly functions.
 *
 * Used by the -d / --dump flag to show the VM instructions for each statement or
 * expression.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include "config.h"
#include <stdio.h>
#include "debug.h"
#include "value.h"

// Function declarations.
static int simple_instruction(const char *name, int offset);
static int constant_instruction(const char *name, Chunk *chunk, int offset);

/** @brief Disassemble the entire Chunk array of code.
 *
 * Displays the entire array of bytecode stored in the Chunk
 * array.
 *
 * @param chunk A pointer to the Chunk array to display.
 * @param name A descriptive name to describe the displayed values.
 */
void disassemble_chunk(Chunk *chunk, const char *name)
{
  printf("== %s ==\n", name);

  for(int i = 0; i < chunk->count;)
  {
    i = disassemble_instruction(chunk, i);
  }
}

/** @brief Disassemble a single instruction.
 *
 * Disassemble a single instruction. Each bytecode found in the Chunk
 * array is passed to this function for display.
 *
 * @param chunk A pointer to the bytecode chunk to display.
 * @param i The offset to the bytecode instruction in the Chunk array.
 * @return The offset to the next bytecode instruction.
 */
int disassemble_instruction(Chunk *chunk, int offset)
{
  printf("%04d ", offset);

  if(offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
  {
    printf("   | ");
  }
  else
  {
    printf("%4d ", chunk->lines[offset]);
  }

  uint8_t instruction = chunk->code[offset];
  switch(instruction)
  {
    case OP_CONSTANT:
      return constant_instruction("OP_CONSTANT", chunk, offset);
    case OP_FALSE:
      return simple_instruction("OP_FALSE", offset);
    case OP_NIL:
      return simple_instruction("OP_NIL", offset);
    case OP_TRUE:
      return simple_instruction("OP_TRUE", offset);
    case OP_EQUAL:
      return simple_instruction("OP_EQUAL", offset);
    case OP_GREATER:
      return simple_instruction("OP_GREATER", offset);
    case OP_LESS:
      return simple_instruction("OP_LESS", offset);
    case OP_ADD:
      return simple_instruction("OP_ADD", offset);
    case OP_SUBTRACT:
      return simple_instruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
      return simple_instruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
      return simple_instruction("OP_DIVIDE", offset);
    case OP_MODULUS:
      return simple_instruction("OP_MODULUS", offset);
    case OP_POWER:
      return simple_instruction("OP_POWER", offset);
    case OP_NOT:
      return simple_instruction("OP_NOT", offset);
    case OP_NEGATE:
      return simple_instruction("OP_NEGATE", offset);
    case OP_RETURN:
      return simple_instruction("OP_RETURN", offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}

/** @brief Output a simple one byte instruction.
 *
 * Print the OpCode name to stdout and return the modified
 * offset for a singe bytecode instruction.
 *
 * @param name The bytecode instruction name.
 * @param offset The currect offset into the Chunk array.
 * @return The new offset for the next bytecode instruction.
 */
static int simple_instruction(const char *name, int offset)
{
  printf("%s\n", name);
  return offset + 1;
}

/** @brief Output a constant bytecode instruction.
 *
 * The OP_CONSTANT instruction spans two bytes, one for the
 * instruction name and one for the offset into the ValueArray
 * where the constant resides.
 *
 * @param name The bytecode instruction name.
 * @param chunk The Chunk array gives access to the ValueArray of constants.
 * @param offset The current offset into the Chunk array for this instruction.
 * @return The new offset for the next bytecode instruction.
 */
static int constant_instruction(const char *name, Chunk *chunk, int offset)
{
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}
