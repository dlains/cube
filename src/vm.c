/** @file vm.c
 *
 * @brief Implementation of the Virtual Machine for Cube.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "vm.h"
#include "compiler.h"
#include "debug.h"

/** The global virutal machine variable used by the Virtual Machine. */
VM vm;

/** @brief Reset the stack.
 *
 * Reset the stack.
 */
static void reset_stack();

/** @brief Display a runtime error message to stderr.
 *
 * Print a runtime error message to stderr and reset
 * the stack.
 *
 * @param format The standard printf style format string.
 */
static void runtime_error(const char *format, ...);

/** @brief Run the code in the Chunk array.
 *
 * The run() function cycles through all the the OpCodes in the Chunk array
 * and performs the necessary actions.
 *
 * @return The InterpretResult of running the code.
 */
static InterpretResult run();

/** @brief Perform the addition operation.
 *
 * Adds two number values. The values can be either integer or real
 * values.
 */
static void do_addition();

/** @brief Perform the subtraction operation.
 *
 * Subtracts two number values. The values can be either integer or
 * real values.
 */
static void do_subtraction();

/** @brief Perform the multiplication operation.
 *
 * Multiplies two number values. The values can be either integer or
 * real values.
 */
static void do_multiplication();

/** @brief Perform the division operation.
 *
 * Divide two number values. The values can be either integer or real
 * values.
 */
static void do_division();

/** @brief Perform the modulo operation.
 *
 * Take the modulus of two integers. The values must be integers.
 */
static void do_modulo();

/** @brief Perform the exponentiation operation.
 *
 * Raise an integer to a power of another integer.
 */
static void do_power();

/** @brief Peek at a value in the stack.
 *
 * Returns but does not remove a value at distance offset
 * from the top of the stack.
 *
 * @param distance The distance from the top of the stack to look at.
 * @return The value found in the stack.
 */
static Value peek(int distance);

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
    disassemble_chunk(&chunk, "code");
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

/** @brief Peek at a value in the stack.
 *
 * Returns but does not remove a value at distance offset
 * from the top of the stack.
 *
 * @param distance The distance from the top of the stack to look at.
 * @return The value found in the stack.
 */
static Value peek(int distance)
{
  return vm.stack_top[-1 - distance];
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
        if(!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))
        {
          runtime_error("Operands must be numeric.");
          return INTERPRET_RUNTIME_ERROR;
        }
        do_addition();
        break;
      }
      case OP_SUBTRACT:
      {
        if(!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))
        {
          runtime_error("Operands must be numeric.");
          return INTERPRET_RUNTIME_ERROR;
        }
        do_subtraction();
        break;
      }
      case OP_MULTIPLY:
      {
        if(!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))
        {
          runtime_error("Operands must be numeric.");
          return INTERPRET_RUNTIME_ERROR;
        }
        do_multiplication();
        break;
      }
      case OP_DIVIDE:
      {
        if(!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))
        {
          runtime_error("Operands must be numeric.");
          return INTERPRET_RUNTIME_ERROR;
        }
        // Check for divide by zero runtime error.
        if(IS_INTEGER(peek(0)) && AS_INTEGER(peek(0)) == 0)
        {
          runtime_error("Attempt to divide by zero.");
          return INTERPRET_RUNTIME_ERROR;
        }
        if(IS_REAL(peek(0)) && AS_REAL(peek(0)) == 0)
        {
          runtime_error("Attempt to divide by zero.");
          return INTERPRET_RUNTIME_ERROR;
        }
        do_division();
        break;
      }
      case OP_MODULUS:
      {
        if(!IS_INTEGER(peek(0)) && !IS_INTEGER(peek(1)))
        {
          runtime_error("Modulo operation can only be performed on integer values.");
          return INTERPRET_RUNTIME_ERROR;
        }
        do_modulo();
        break;
      }
      case OP_POWER:
      {
        if(!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))
        {
          runtime_error("Operands must be numeric.");
          return INTERPRET_RUNTIME_ERROR;
        }
        do_power();
        break;
      }
      case OP_NEGATE:
      {
        if(!IS_NUMBER(peek(0)))
        {
          runtime_error("Operand must be numeric.");
          return INTERPRET_RUNTIME_ERROR;
        }

        Value v = pop();
        switch(v.type)
        {
          case CUBE_INTEGER:
            push(INTEGER_VAL(-AS_INTEGER(v)));
            break;
          case CUBE_REAL:
            push(REAL_VAL(-AS_REAL(v)));
            break;
          case CUBE_BOOL:
          case CUBE_NIL:
          default:
            break;
        }
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
}

/** @brief Reset the stack.
 *
 * Reset the stack.
 */
static void reset_stack()
{
  vm.stack_top = vm.stack;
}

/** @brief Display a runtime error message to stderr.
 *
 * Print a runtime error message to stderr and reset
 * the stack.
 *
 * @param format The standard printf style format string.
 */
static void runtime_error(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  // TODO: This will need to be updated when better source code positioning information is available.
  size_t instruction = vm.ip - vm.chunk->code;
  fprintf(stderr, "[line %d] in script\n", vm.chunk->lines[instruction]);

  reset_stack();
}

/** @brief Perform the addition operation.
 *
 * Adds two number values. The values can be either integer or real
 * values.
 */
static void do_addition()
{
  if(IS_INTEGER(peek(0)) && IS_INTEGER(peek(1)))
  {
    long b = AS_INTEGER(pop());
    long a = AS_INTEGER(pop());

    push(INTEGER_VAL(a + b));
    return;
  }

  if(IS_REAL(peek(0)) && IS_REAL(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(a + b));
    return;
  }

  if(IS_INTEGER(peek(0)) && IS_REAL(peek(1)))
  {
    double b = (double)AS_INTEGER(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(a + b));
    return;
  }

  if(IS_REAL(peek(0)) && IS_INTEGER(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = (double)AS_INTEGER(pop());

    push(REAL_VAL(a + b));
    return;
  }
}

/** @brief Perform the subtraction operation.
 *
 * Subtracts two number values. The values can be either integer or
 * real values.
 */
static void do_subtraction()
{
  if(IS_INTEGER(peek(0)) && IS_INTEGER(peek(1)))
  {
    long b = AS_INTEGER(pop());
    long a = AS_INTEGER(pop());

    push(INTEGER_VAL(a - b));
    return;
  }

  if(IS_REAL(peek(0)) && IS_REAL(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(a - b));
    return;
  }

  if(IS_INTEGER(peek(0)) && IS_REAL(peek(1)))
  {
    double b = (double)AS_INTEGER(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(a - b));
    return;
  }

  if(IS_REAL(peek(0)) && IS_INTEGER(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = (double)AS_INTEGER(pop());

    push(REAL_VAL(a - b));
    return;
  }
}

/** @brief Perform the multiplication operation.
 *
 * Multiplies two number values. The values can be either integer or
 * real values.
 */
static void do_multiplication()
{
  if(IS_INTEGER(peek(0)) && IS_INTEGER(peek(1)))
  {
    long b = AS_INTEGER(pop());
    long a = AS_INTEGER(pop());

    push(INTEGER_VAL(a * b));
    return;
  }

  if(IS_REAL(peek(0)) && IS_REAL(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(a * b));
    return;
  }

  if(IS_INTEGER(peek(0)) && IS_REAL(peek(1)))
  {
    double b = (double)AS_INTEGER(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(a * b));
    return;
  }

  if(IS_REAL(peek(0)) && IS_INTEGER(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = (double)AS_INTEGER(pop());

    push(REAL_VAL(a * b));
    return;
  }
}

/** @brief Perform the division operation.
 *
 * Divide two number values. The values can be either integer or real
 * values.
 */
static void do_division()
{
  if(IS_INTEGER(peek(0)) && IS_INTEGER(peek(1)))
  {
    long b = AS_INTEGER(pop());
    long a = AS_INTEGER(pop());

    push(INTEGER_VAL(a / b));
    return;
  }

  if(IS_REAL(peek(0)) && IS_REAL(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(a / b));
    return;
  }

  if(IS_INTEGER(peek(0)) && IS_REAL(peek(1)))
  {
    double b = (double)AS_INTEGER(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(a / b));
    return;
  }

  if(IS_REAL(peek(0)) && IS_INTEGER(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = (double)AS_INTEGER(pop());

    push(REAL_VAL(a / b));
    return;
  }
}

/** @brief Perform the modulo operation.
 *
 * Take the modulus of two integers. The values must be integers.
 */
static void do_modulo()
{
  long b = AS_INTEGER(pop());
  long a = AS_INTEGER(pop());

  push(INTEGER_VAL(a % b));
}

/** @brief Perform the exponentiation operation.
 *
 * Raise an integer to a power of another integer.
 */
static void do_power()
{
  if(IS_INTEGER(peek(0)) && IS_INTEGER(peek(1)))
  {
    double b = (double)AS_INTEGER(pop());
    double a = (double)AS_INTEGER(pop());

    push(REAL_VAL(pow(a, b)));
    return;
  }

  if(IS_REAL(peek(0)) && IS_REAL(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(pow(a, b)));
    return;
  }

  if(IS_INTEGER(peek(0)) && IS_REAL(peek(1)))
  {
    double b = (double)AS_INTEGER(pop());
    double a = AS_REAL(pop());

    push(REAL_VAL(pow(a, b)));
    return;
  }

  if(IS_REAL(peek(0)) && IS_INTEGER(peek(1)))
  {
    double b = AS_REAL(pop());
    double a = (double)AS_INTEGER(pop());

    push(REAL_VAL(pow(a, b)));
    return;
  }
}
