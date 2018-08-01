/** @file vm.h
 *
 * @brief The virtual machine module for the Cube project.
 *
 * Define the structures and interface to the Cube virtual
 * machine.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#ifndef VM_H
#define VM_H

#include "chunk.h"
#include "value.h"
#include "options.h"

#define STACK_MAX 256

typedef struct
{
  Chunk *chunk;
  Byte *ip;
  Options options;
  Value stack[STACK_MAX];
  Value *stack_top;
} VM;

typedef enum
{
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

/** @brief Initialize the virtual machine.
 *
 * Create the VM structure and store any command line
 * options that were provided.
 *
 * @param options The command line options.
 */
void vm_init(Options options);

/** @brief Free the resources used by the virtual machine.
 *
 * Release any memory and resources used by the virtual machine.
 */
void vm_free();

/** @brief Interpret the current code.
 *
 * The compiler adds code the the VM and this function
 * determines what should happen based on the OpCode.
 *
 * @return The interpret results lets the caller know if there
 * was a compilation error, a runtime error or no error.
 */
InterpretResult interpret();

/** @brief Push a value onto the stack.
 *
 * Places a value at the top of the runtime stack.
 *
 * @param value The value to put on the stack.
 */
void push(Value value);

/** @brief Remove a value from the stack.
 *
 * Removes and returns the top value from the runtime stack.
 *
 * @return The top Value from the stack.
 */
Value pop();

#endif // VM_H
