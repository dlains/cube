/** @file memory.h
 *
 * @brief Memory management routines for the Cube project.
 *
 * Defines macros and functions for allocating and resizing
 * dynamic arrays used in Cube.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs
 */
#ifndef MEMORY_H
#define MEMORY_H

/** @def NEW(p)
 *
 * Allocate memory for a single struct.
 */
#define NEW(p) \
  ((p) = reallocate(NULL, 0, sizeof(*(p)), __FILE__, __LINE__))

/** @def ALLOC(type, count)
 *
 * Allocate memory for count amount of the given type.
 */
#define ALLOC(type, count) \
  (type*)reallocate(NULL, 0, sizeof(type) * (count), __FILE__, __LINE__)

/** @def FREE(type, pointer)
 *
 * Free a the memory associated with the given pointer.
 */
#define FREE(type, pointer) \
  reallocate(pointer, sizeof(type), 0, __FILE__, __LINE__)

/**
 * @def GROW_CAPACITY(capacity)
 *
 * Grow the capacity of a dynamic array. Starts with an array with eight
 * elements if it is currently empty. Grows by a factor of two when
 * needed.
 */
#define GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)

/**
 * @def GROW_ARRAY(previous, type, old_count, count)
 *
 * Grow a dynamic array to a new size.
 */
#define GROW_ARRAY(previous, type, old_count, count) \
  (type*)reallocate(previous, sizeof(type) * (old_count), sizeof(type) * (count), __FILE__, __LINE__)

/**
 * @def FREE_ARRAY(type, pointer, old_count)
 *
 * Free the memory used by a dynamic array.
 */
#define FREE_ARRAY(type, pointer, old_count) \
  reallocate(pointer, sizeof(type) * (old_count), 0, __FILE__, __LINE__)

/** @brief Reallocate memory to the given size.
 *
 * Works in four modes:
 *   1. new_size == 0 - free the memory.
 *   2. old_size == 0 and new_size > 0 - allocate new memory.
 *   3. old_size > 0 and new_size > old_size - grow to new size.
 *   4. old_size > 0 and new_size < old_size - reduce to new size.
 *
 * If the allocation fails an error message is displayed and the program
 * exits.
 *
 * @param previous A pointer to the existing memory.
 * @param old_size The existing size of the allocated memory.
 * @param new_size The new size to reallocate the memory to.
 * @param file Diagnostic source file of the reallocate call.
 * @param line Diagnostic source file line of the reallocate call.
 * @return A pointer to the reallocated memory.
 */
void *reallocate(void *previous, size_t old_size, size_t new_size, const char *file, int line);

#endif // MEMORY_H
