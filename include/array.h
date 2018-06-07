#ifndef ARRAY_H
#define ARRAY_H

typedef struct array_type *Array;

Array ArrayNew(int length, int size);
void ArrayFree(Array array);
int ArrayLength(Array array);
int ArraySize(Array array);
void *ArrayGet(Array array, int i);
void *ArrayPut(Array array, int i, void *elem);
void ArrayResize(Array array, int length);
Array ArrayCopy(Array array, int length);

#endif // ARRAY_H
