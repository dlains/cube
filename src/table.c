/** @file table.c
 *
 * @brief Implementation of the Hash Table interface.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include "config.h"
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "memory.h"
#include "object.h"
#include "table.h"

#define TABLE_MAX_LOAD 0.75

/** @brief Look for an existing entry.
 *
 * Used both when searching for an entry and before
 * inserting a new entry.
 *
 * @param table The symbol table to search through.
 * @param key The Object key to search for.
 * @return The found entry or null if it was not found.
 */
static Entry table_find_entry(Table table, Object *key);

/** @brief Resize the symbol table.
 *
 * When the symbol table gets close to its capacity it will be resized
 * to make room for existing and new entries.
 *
 * @param table The symbol table to resize.
 */
static void table_resize(Table table);

/** @brief Initialize a new symbol table.
 *
 * Set the count and capacity to initial values
 * and allocate space for the initial Entries.
 *
 * @param table The Table pointer to initialize.
 */
void table_init(Table table)
{
  assert(table != NULL);

  table->count    = 0;
  table->capacity = 0;
  table->entries  = NULL;
  table->parent   = NULL;
}

/** @brief Free the resources used by this symbol table.
 *
 * Release any objects in the symbol table and then release
 * the table itself.
 *
 * @param table The symbol table to free.
 */
void table_free(Table table)
{
  assert(table != NULL);

  for(int i = 0; i < table->capacity; i++)
  {
    for(Entry e = table->entries[i]; e != NULL; e = e->next)
    {
      FREE(Entry*, e);
    }
  }
  FREE_ARRAY(Entry, table->entries, table->capacity);
  table_init(table);
}

/** @brief Create a new nested scope.
 *
 * Assign the current symbol table as the parent and
 * create a new symbol table to store information in the
 * current scope.
 *
 * @param table The new symbol table for the new scope.
 * @param parent The parent symbol table.
 */
void table_init_scope(Table table, Table parent)
{
  assert(table != NULL);
  assert(parent != NULL);

  table_init(table);
  table->parent = parent;
}

/** @brief Free a symbol table scope.
 *
 * Remove a nested symbol table and return the previous
 * parent symbol table.
 *
 * @param table The symbol table to free the scope for.
 * @return The parent symbol table which will now be the current table.
 */
Table table_free_scope(Table table)
{
  assert(table != NULL);

  Table parent = table->parent;
  table_free(table);
  return parent;
}

/** @brief Look for an existing entry.
 *
 * Used both when searching for an entry and before
 * inserting a new entry.
 *
 * @param table The symbol table to search through.
 * @param key The Object key to search for.
 * @return The found entry or null if it was not found.
 */
static Entry table_find_entry(Table table, Object *key)
{
  assert(table != NULL);
  assert(key != NULL);

  int hash = object_hash(key, table->capacity);

  for(Entry e = table->entries[hash]; e != NULL; e = e->next)
  {
    if(objects_equal(e->key, key))
      return e;
  }

  return NULL;
}

/** @brief Resize the symbol table.
 *
 * When the symbol table gets close to its capacity it will be resized
 * to make room for existing and new entries.
 *
 * @param table The symbol table to resize.
 */
static void table_resize(Table table)
{
  assert(table != NULL);

  int orig_capacity = table->capacity;
  table->capacity = GROW_CAPACITY(table->capacity);
  Entry *entries = ALLOC(Entry, table->capacity);

  // Re-hash the existing entries.
  table->count = 0;
  for(int i = 0; i < orig_capacity; i++)
  {
    for(Entry e = table->entries[i]; e != NULL; e = e->next)
    {
      int hash = object_hash(e->key, table->capacity);
      if(entries[hash])
      {
        e->next = entries[hash]->next;
        entries[hash] = e;
      }
      else
      {
        entries[hash] = e;
      }
    }
    table->count++;
  }

  // Free the existing entries.
  FREE_ARRAY(Entry, table->entries, orig_capacity);
  table->entries = entries;
}

/** @brief Search the symbol table.
 *
 * Search the symbol table for an entry matching the given key.
 *
 * @param table The symbol table to search in.
 * @param key The Object key to look for.
 * @return The Object value found for the key, or null if the key is not in the symbol table.
 */
Object *table_search(Table table, Object *key)
{
  assert(table != NULL);
  assert(key != NULL);

  if(table->entries == NULL)
  {
    if(table->parent)
      return table_search(table->parent, key);
    else
      return NULL;
  }

  Entry entry = table_find_entry(table, key);
  if(entry)
    return entry->value;

  if(table->parent)
    return table_search(table->parent, key);
  else
    return NULL;
}

/** @brief Search the symbol table for a C string.
 *
 * Search the symbol table for a C string entry.
 *
 * @param table The symbol table to search in.
 * @param string The C String to search for.
 * @param length The length of the C String..
 * @return The Object value found for the key, or null if the key is not in the symbol table.
 */
Object *table_search_string(Table table, const char *string, int length)
{
  assert(table != NULL);
  assert(string != NULL);
  assert(length > 0);

  if(table->entries == NULL)
    return NULL;

  int hash = string_hash(string, table->capacity);

  for(Entry e = table->entries[hash]; e != NULL; e = e->next)
  {
    ObjectString *s = AS_STRING(e->key);
    if(s->length == length && memcmp(s->chars, string, length) == 0)
      return e->value;
  }

  return NULL;
}

/** @brief Add a new entry to the symbol table.
 *
 * Create a new Entry with the given key and value and insert it
 * into the symbol table.
 *
 * @param table The symbol table to insert the new entry into.
 * @param key The Object key to assign to the Entry key.
 * @param value The Object value to assign to the Entry value.
 */
void table_insert(Table table, Object *key, Object *value)
{
  assert(table != NULL);
  assert(key != NULL);
  assert(value != NULL);

  if(table->count + 1 > table->capacity * TABLE_MAX_LOAD)
  {
    table_resize(table);
  }

  Entry entry = table_find_entry(table, key);
  if(entry)
    return;

  int hash = object_hash(key, table->capacity);

  Entry new_entry = NEW(entry);
  new_entry->key = key;
  new_entry->value = value;

  if(table->entries[hash])
  {
    new_entry->next = table->entries[hash]->next;
    table->entries[hash] = new_entry;
  }
  else
  {
    table->entries[hash] = new_entry;
  }

  table->count++;
}

/** @brief Remove an Entry from the symbol table.
 *
 * Find an Entry in the symbol table matching the given key and
 * remove it from the symbol table.
 *
 * @param table The symbol table to delete the Entry from.
 * @param key The Object key to find the Entry with.
 */
void table_delete(Table table, Object *key)
{
  assert(table != NULL);
  assert(key != NULL);

  if(table->count == 0)
    return;

  Entry entry = table_find_entry(table, key);
  if(entry == NULL)
    return;

  int hash = object_hash(key, table->capacity);
  Entry head = table->entries[hash];
  if(objects_equal(key, head->key))
  {
    table->entries[hash] = head->next;
    table->count--;
    return;
  }

  while(head->next != NULL)
  {
    if(objects_equal(key, head->next->key))
    {
      head->next = head->next->next;
      table->count--;
      return;
    }
    head = head->next;
  }
  return;
}

/** @brief Merge two symbol tables into a single table.
 *
 * Combine two separate symbol tables into a singe symbol table.
 *
 * @param from The symbol table to move entries from.
 * @param to The symbol table to move entries to.
 */
void table_merge(Table from, Table to)
{
  assert(from != NULL);
  assert(to != NULL);

  for(int i = 0; i < from->capacity; i++)
  {
    for(Entry e = from->entries[i]; e != NULL; e = e->next)
    {
      table_insert(to, e->key, e->value);
    }
  }
  table_free(from);
}
