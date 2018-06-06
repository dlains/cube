#ifndef MEMORY_H
#define MEMORY_H

#define GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)

// Using the long type here to prevent issues from assigning a negative
// value to a size_t. Converting a negative number to a size_t usually
// ends up being a very large number.
void *MemAlloc(long num_bytes, const char *file, int line);
void *MemCalloc(long count, long num_bytes, const char *file, int line);
void *MemResize(void *ptr, long num_bytes, const char *file, int line);
void MemFree(void *ptr, const char *file, int line);

#define ALLOC(num_bytes) \
  MemAlloc((num_bytes), __FILE__, __LINE__)

#define CALLOC(count,num_bytes) \
  MemCalloc((count), (num_bytes), __FILE__, __LINE__)

#define RESIZE(ptr, num_bytes) \
  ((ptr) = MemResize((ptr), (num_bytes), __FILE__, __LINE__))

#define FREE(ptr) ((void)(MemFree((ptr), __FILE__, __LINE__), (ptr) = 0))

// This is a better idiom for allocating memory for a struct.
// Using 'sizeof *p' instead of 'sizeof (struct T)' works for
// any pointer type, except void pointers. If the type of p
// is changed this allocation remains correct.
#define  NEW(p) ((p) = ALLOC((long)sizeof *(p)))
#define NEWC(p) ((p) = CALLOC(1, (long)sizeof *(p)))

#endif // MEMORY_H
