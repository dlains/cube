#ifndef VM_H
#define VM_H

#include "chunk.h"

typedef struct
{
  Chunk *chunk;
  Byte *ip;
} VM;

typedef enum
{
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void VMInit();
void VMFree();
InterpretResult interpret(Chunk *chunk);

#endif // VM_H
