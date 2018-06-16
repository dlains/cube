#ifndef KEYWORDS_H
#define KEYWORDS_H

#include "common.h"

#define NUM_KEYWORDS 23
#define HASH_MAX 7919

// Return true if 'word' is one of the known keywords, false otherwise.
bool is_keyword(const char *word);

// Calculate a hash value for the word.
static int hash(const char *word);

// If the passed in words hash matches one of the keywords
// verify it is actually a keyword and not a hash colision.
static bool verify_keyword(const char *word, int keyword_index);

#endif // KEYWORDS_H
