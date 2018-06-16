#include "keywords.h"

int hash(const char *word)
{
  int h, a = 31415, b = 27183;
  for(h = 0; *word != '\0'; word++, a = a * b % (HASH_MAX - 1))
    h = (a * h + *word) % HASH_MAX;

  return h;
}
