#ifndef VM_H
#define VM_H

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct
{
  Chunk *chunk;
  Byte *ip;
  Value stack[STACK_MAX];
  Value *stack_top;
} VM;

typedef enum
{
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void vm_init();
void vm_free();
InterpretResult interpret();
void push(Value value);
Value pop();

#endif // VM_H
