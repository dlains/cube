#include <stdlib.h>
#include <assert.h>
#include "common.h"
#include "memory.h"

void *MemAlloc(long num_bytes, const char *file, int line)
{
  void *ptr;

  assert(num_bytes > 0);
  ptr = malloc(num_bytes);

  if(ptr == NULL)
  {
    die("Memory allocation failed", file, line);
  }

  return ptr;
}

void *MemCalloc(long count, long num_bytes, const char *file, int line)
{
  void *ptr;

  assert(count > 0);
  assert(num_bytes > 0);
  ptr = calloc(count, num_bytes);

  if(ptr == NULL)
  {
    die("Memory allocation failed", file, line);
  }

  return ptr;
}

void *MemResize(void *ptr, long num_bytes, const char *file, int line)
{
  assert(ptr);
  assert(num_bytes > 0);
  ptr = realloc(ptr, num_bytes);

  if(ptr == NULL)
  {
    die("Memory resize failed", file, line);
  }

  return ptr;
}

void MemFree(void *ptr, const char *file, int line)
{
  if(ptr == NULL)
  {
    die("Attempt to free a null pointer.", file, line);
  }

  free(ptr);
}

