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
#include "object.h"

// Function declarations.
static int simple_instruction(const char *name, int offset);
static int byte_instruction(const char *name, Chunk *chunk, int offset);
static int constant_instruction(const char *name, Chunk *chunk, int offset);
static int jump_instruction(const char *name, int sign, Chunk *chunk, int offset);

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
    case OP_NIL:
      return simple_instruction("OP_NIL", offset);
    case OP_POP:
      return simple_instruction("OP_POP", offset);
    case OP_GET_LOCAL:
      return byte_instruction("OP_GET_LOCAL", chunk, offset);
    case OP_SET_LOCAL:
      return byte_instruction("OP_SET_LOCAL", chunk, offset);
    case OP_GET_GLOBAL:
      return constant_instruction("OP_GET_GLOBAL", chunk, offset);
    case OP_DEFINE_GLOBAL:
      return constant_instruction("OP_DEFINE_GLOBAL", chunk, offset);
    case OP_SET_GLOBAL:
      return constant_instruction("OP_SET_GLOBAL", chunk, offset);
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
    case OP_PRINT:
      return simple_instruction("OP_PRINT", offset);
    case OP_JUMP:
      return jump_instruction("OP_JUMP", 1, chunk, offset);
    case OP_JUMP_IF_FALSE:
      return jump_instruction("OP_JUMP_IF_FALSE", 1, chunk, offset);
    case OP_LOOP:
      return jump_instruction("OP_LOOP", -1, chunk, offset);
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
 * @param offset The current offset into the Chunk array.
 * @return The new offset for the next bytecode instruction.
 */
static int simple_instruction(const char *name, int offset)
{
  printf("%s\n", name);
  return offset + 1;
}

/** @brief Output a byte instruction.
 *
 * Print the byte instruction.
 *
 * @param name The bytecode instruction name.
 * @param chunk The Chunk array gives access to the Objectarray of objects.
 * @param offset The current offset into the Chunk array.
 * @return The new offset for the next bytecode instruction.
 */
static int byte_instruction(const char *name, Chunk *chunk, int offset)
{
  Byte slot = chunk->code[offset + 1];
  printf("%-16s %4d\n", name, slot);
  return offset + 2;
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
  print_object(chunk->constants.objects[constant]);
  printf("'\n");
  return offset + 2;
}

/** @brief Output a jump instruction.
 *
 * The Jump and Loop instructions have a 16 bit offset.
 *
 * @param name The bytecode instruction name.
 * @param sign The sign of the instruction.
 * @param chunk The Chuck array gives access to the code.
 * @param offset The current offset into the Chunk array for this instruction.
 * @return The new offset for the next bytecode instruction.
 */
static int jump_instruction(const char *name, int sign, Chunk *chunk, int offset)
{
  uint16_t jump = (uint16_t)(chunk->code[offset + 1] << 8);
  jump |= chunk->code[offset + 2];
  printf("%-16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
  return offset + 3;
}
