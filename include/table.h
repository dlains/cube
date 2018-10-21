/** @file table.h
 *
 * @brief Define the Hash Table interface.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#ifndef TABLE_H
#define TABLE_H

#include "common.h"
#include "object.h"

/** @struct Entry
 * A Table entry consists of an Object* key and it's corresponding
 * Object* value.
 *
 * Ideally the key can be any type supported by the Cube language and
 * can reference a value of any type.
 */
typedef struct entry *Entry;

struct entry {
  Object *key;
  Object *value;
  Entry next;
};

/** @struct Table
 * The Table struct defines the Symbol Table. The parent table pointer
 * will only be used for nested scopes.
 */
typedef struct table *Table;

struct table {
  int count;
  int capacity;
  Entry *entries;
  Table parent;
};

/** @brief Initialize a new symbol table.
 *
 * Set the count and capacity to initial values
 * and allocate space for the initial Entries.
 *
 * @param table The Table pointer to initialize.
 */
void table_init(Table table);

/** @brief Free the resources used by this symbol table.
 *
 * Release any objects in the symbol table and then release
 * the table itself.
 *
 * @param table The symbol table to free.
 */
void table_free(Table table);

/** @brief Search the symbol table.
 *
 * Search the symbol table for an entry matching the given key.
 *
 * @param table The symbol table to search in.
 * @param key The Object key to look for.
 * @return The Object value found for the key, or null if the key is not in the symbol table.
 */
Object *table_search(Table table, Object *key);

/** @brief Search the symbol table for a C string.
 *
 * Search the symbol table for a C string entry.
 *
 * @param table The symbol table to search in.
 * @param string The C String to search for.
 * @param length The length of the C String..
 * @return The Object value found for the key, or null if the key is not in the symbol table.
 */
Object *table_search_string(Table table, const char *string, int length);

/** @brief Add a new entry to the symbol table.
 *
 * Create a new Entry with the given key and value and insert it
 * into the symbol table.
 *
 * @param table The symbol table to insert the new entry into.
 * @param key The Object key to assign to the Entry key.
 * @param value The Object value to assign to the Entry value.
 */
void table_insert(Table table, Object *key, Object *value);

/** @brief Remove an Entry from the symbol table.
 *
 * Find an Entry in the symbol table matching the given key and
 * remove it from the symbol table.
 *
 * @param table The symbol table to delete the Entry from.
 * @param key The Object key to find the Entry with.
 */
void table_delete(Table table, Object *key);

/** @brief Merge two symbol tables into a single table.
 *
 * Combine two separate symbol tables into a singe symbol table.
 *
 * @param from The symbol table to move entries from.
 * @param to The symbol table to move entries to.
 */
void table_merge(Table from, Table to);

#endif // TABLE_H
