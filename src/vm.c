/** @file vm.c
 *
 * @brief Implementation of the Virtual Machine for Cube.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "vm.h"
#include "compiler.h"
#include "debug.h"

VM vm;

/** @brief Reset the stack.
 *
 * Reset the stack.
 */
static void reset_stack();

/** @brief Run the code in the Chunk array.
 *
 * The run() function cycles through all the the OpCodes in the Chunk array
 * and performs the necessary actions.
 *
 * @return The InterpretResult of running the code.
 */
static InterpretResult run();

/** @brief Initialize the virtual machine.
 *
 * Create the VM structure and store any command line
 * options that were provided.
 *
 * @param options The command line options.
 */
void vm_init(Options options)
{
  vm.options = options;
  reset_stack();
}

/** @brief Free the resources used by the virtual machine.
 *
 * Release any memory and resources used by the virtual machine.
 */
void vm_free()
{
}

/** @brief Interpret the current code.
 *
 * The compiler adds code the the VM and this function
 * determines what should happen based on the OpCode.
 *
 * @return The interpret results lets the caller know if there
 * was a compilation error, a runtime error or no error.
 */
InterpretResult interpret()
{
  Chunk chunk;
  init_chunk(&chunk);

  if(!compile(&chunk))
  {
    free_chunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  // If the dump code flag is set show the bytecode.
  if(options_get_show_code(vm.options))
  {
    disassembleChunk(&chunk, "code");
  }

  vm.chunk = &chunk;
  vm.ip    = vm.chunk->code;

  InterpretResult result = run();

  free_chunk(&chunk);

  return result;
}

/** @brief Push a value onto the stack.
 *
 * Places a value at the top of the runtime stack.
 *
 * @param value The value to put on the stack.
 */
void push(Value value)
{
  *vm.stack_top = value;
  vm.stack_top++;
}

/** @brief Remove a value from the stack.
 *
 * Removes and returns the top value from the runtime stack.
 *
 * @return The top Value from the stack.
 */
Value pop()
{
  vm.stack_top--;
  return *vm.stack_top;
}

/** @brief Run the code in the Chunk array.
 *
 * The run() function cycles through all the the OpCodes in the Chunk array
 * and performs the necessary actions.
 *
 * @return The InterpretResult of running the code.
 */
static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
  do {\
    double b = pop();\
    double a = pop();\
    push(a op b);\
  } while(false)

  for(;;)
  {
    Byte instruction;
    switch(instruction = READ_BYTE())
    {
      case OP_CONSTANT:
      {
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }
      case OP_ADD:
      {
        BINARY_OP(+);
        break;
      }
      case OP_SUBTRACT:
      {
        BINARY_OP(-);
        break;
      }
      case OP_MULTIPLY:
      {
        BINARY_OP(*);
        break;
      }
      case OP_DIVIDE:
      {
        BINARY_OP(/);
        break;
      }
      case OP_MODULUS:
      {
        // TODO: Make modulo work for more numeric types.
        double b = pop();
        double a = pop();
        push ((int)a % (int)b);
        break;
      }
      case OP_POWER:
      {
        // TODO: Better way to handle powers? This probably needs to change when Cube can handle more types than double.
        double b = pop();
        double a = pop();
        push(pow((int)a, (int)b));
        break;
      }
      case OP_NEGATE:
      {
        push(-pop());
        break;
      }
      case OP_RETURN:
      {
        print_value(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

/** @brief Reset the stack.
 *
 * Reset the stack.
 */
static void reset_stack()
{
  vm.stack_top = vm.stack;
}
