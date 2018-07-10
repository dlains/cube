#include <stdio.h>
#include "common.h"
#include "vm.h"
#include "compiler.h"
#include "debug.h"

VM vm;

// Comment out the run function for the time being. Not currently being used.
// static InterpretResult run();
static void reset_stack();
// static void printStack();

void vm_init()
{
  reset_stack();
}

void vm_free()
{
}

InterpretResult interpret()
{
  compile();
  // return run();
  return INTERPRET_OK;
}

void push(Value value)
{
  *vm.stack_top = value;
  vm.stack_top++;
}

Value pop()
{
  vm.stack_top--;
  return *vm.stack_top;
}

static void reset_stack()
{
  vm.stack_top = vm.stack;
}

/*
static void printStack()
{
  printf("          ");
  for(Value *slot = vm.stack; slot < vm.stackTop; slot++)
  {
    printf("[ ");
    printValue(*slot);
    printf(" ]");
  }
  printf("\n");
}
*/

/*
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
#ifdef DEBUG_TRACE_EXECUTION
    printStack();
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
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
      case OP_NEGATE:
      {
        push(-pop());
        break;
      }
      case OP_RETURN:
      {
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}
*/
