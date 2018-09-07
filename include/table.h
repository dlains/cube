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

typedef struct {
  ObjectString *key;
  Object *value;
} Entry;

typedef struct {
  int count;
  int capacity;
  Entry *entries;
} Table;

void init_table(Table *table);
void free_table(Table *table);
bool table_get(Table *table, ObjectString *key, Object *value);
bool table_set(Table *table, ObjectString *key, Object *value);
bool table_delete(Table *table, ObjectString *key);
void table_merge(Table *from, Table *to);

ObjectString *table_find_string(Table *table, const char *chars, int length, Byte hash);

#endif // TABLE_H
