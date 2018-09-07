/** @file table.c
 *
 * @brief Implementation of the Hash Table interface.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"

#define TABLE_MAX_LOAD 0.75

void init_table(Table *table)
{
  table->count    = 0;
  table->capacity = 0;
  table->entries  = NULL;
}

void free_table(Table *table)
{
  FREE_ARRAY(Entry, table->entries, table->capacity);
  init_table(table);
}

static Byte find_entry(Entry *entries, int capacity, ObjectString *key)
{
  Byte index = key->hash % capacity;

  for(;;)
  {
    Entry *entry = &entries[index];
    if(entry->key == NULL || entry->key == key)
      return index;

    index = (index + 1) % capacity;
  }
}
