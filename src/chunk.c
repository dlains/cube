#include <stdlib.h>
#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk *chunk)
{
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line)
{
  if(chunk->capacity < chunk->count + 1)
  {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    if(chunk->code)
      RESIZE(chunk->code, sizeof(uint8_t) * chunk->capacity);
    else
      chunk->code   = ALLOC(sizeof(uint8_t) * chunk->capacity);

    if(chunk->lines)
      RESIZE(chunk->lines, sizeof(int) * chunk->capacity);
    else
      chunk->lines  = ALLOC(sizeof(int) * chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

void freeChunk(Chunk *chunk)
{
  FREE(chunk->code);
  FREE(chunk->lines);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

int addConstant(Chunk *chunk, Value value)
{
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}
