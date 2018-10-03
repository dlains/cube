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

/** @brief Initialize a new symbol table.
 *
 * Set the count and capacity to initial values
 * and allocate space for the initial Entries.
 *
 * @param table The Table pointer to initialize.
 */
void table_init(Table *table)
{
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
void table_free(Table *table)
{
  FREE_ARRAY(Entry, table->entries, table->capacity);
  init_table(table, 0);
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
void table_init_scope(Table *table, Table *parent)
{
}

/** @brief Free a symbol table scope.
 *
 * Remove a nested symbol table and return the previous
 * parent symbol table.
 *
 * @param table The symbol table to free the scope for.
 * @return The parent symbol table which will now be the current table.
 */
Table *table_free_scope(Table *table)
{
}

/** @brief Resize the symbol table.
 *
 * When the symbol table gets close to its capacity it will be resized
 * to make room for existing and new entries.
 *
 * @param table The symbol table to resize.
 */
void table_resize(Table *table)
{
}

/** @brief Search the symbol table.
 *
 * Search the symbol table for an entry matching the given key.
 *
 * @param table The symbol table to search in.
 * @param key The Object key to look for.
 * @return The Object value found for the key, or null if the key is not in the symbol table.
 */
Object *table_search(Table *table, Object *key)
{
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
void table_insert(Table *table, Object *key, Object *value)
{
}

/** @brief Remove an Entry from the symbol table.
 *
 * Find an Entry in the symbol table matching the given key and
 * remove it from the symbol table.
 *
 * @param table The symbol table to delete the Entry from.
 * @param key The Object key to find the Entry with.
 */
void table_delete(Table *table, Object *key)
{
}

/** @brief Merge two symbol tables into a single table.
 *
 * Combine two separate symbol tables into a singe symbol table.
 *
 * @param from The symbol table to move entries from.
 * @param to The symbol table to move entries to.
 */
void table_merge(Table *from, Table *to)
{
}

