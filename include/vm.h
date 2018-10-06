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
#include "object.h"
#include "table.h"
#include "options.h"

#define STACK_MAX 256

/** @struct The virtual machine structure.
 *
 * Holds all the necessary information for the virtual machine to do its work.
 */
typedef struct
{
  Chunk *chunk;               /**< Pointer to the Chunk array of compiled code. */
  Byte *ip;                   /**< The instruction pointer. This is the next operation to perform. */
  Options options;            /**< The command line options. */
  Object *stack[STACK_MAX];   /**< The object stack to hold intermediate results during processing. */
  Object **stack_top;         /**< Pointer to the top of the object stack. */
  Table strings;              /**< Interned strings. */
  Object *objects;            /**< Linked list of objects. */
} VM;

extern VM vm;

/** @enum Iterpret results enumeration.
 *
 * Result of interpreting the code.
 */
typedef enum
{
  INTERPRET_OK,              /**< Interpeting had no errors. */
  INTERPRET_COMPILE_ERROR,   /**< There was a compile error during interpreting. */
  INTERPRET_RUNTIME_ERROR    /**< There was a runtime error while running the code. */
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

/** @brief Push an object onto the stack.
 *
 * Places an object at the top of the runtime stack.
 *
 * @param object The object to put on the stack.
 */
void push(Object *object);

/** @brief Remove an object from the stack.
 *
 * Removes and returns the top object from the runtime stack.
 *
 * @return The top Object from the stack.
 */
Object *pop();

#endif // VM_H
