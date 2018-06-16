#include "keywords.h"

char *keywords[] = {
  "begin",
  "break",
  "case",
  "class",
  "def",
  "do",
  "else",
  "end",
  "ensure",
  "false",
  "if",
  "import",
  "next",
  "nil",
  "rescue",
  "return",
  "super",
  "switch",
  "this",
  "true",
  "unless",
  "until",
  "while",
};

int keyword_hashes[] = {
  2504,
  3748,
  3481,
  1018,
  4235,
  2081,
  1128,
  7082,
  7205,
  2595,
  6130,
  3869,
  1643,
  4493,
  7137,
  2618,
  4786,
  5077,
  5937,
  749,
  7846,
  1255,
  6530,
};

bool is_keyword(const char *word)
{
  int value = hash(word);
  for(int i = 0; i < NUM_KEYWORDS; i++)
  {
    if(keyword_hashes[i] == value)
      return verify_keyword(word, i);
  }
  return false;
}

static int hash(const char *word)
{
  int h, a = 31415, b = 27183;
  for(h = 0; *word != '\0'; word++, a = a * b % (HASH_MAX - 1))
    h = (a * h + *word) % HASH_MAX;

  return h;
}

static bool verify_keyword(const char *word, int keyword_index)
{
  return memcmp(word, keywords[keyword_index], strlen(keywords[keyword_index])) == 0;
}

// Use along with the first test in 'test/test_keywords.c' to show all keyword
// hash values if new keywords are added. Uncomment this function and the test.
// Run the test and update keyword_hashes appropriately.
/*
void show_keyword_hashes(void)
{
  for(int i = 0; i < NUM_KEYWORDS; i++)
  {
    printf("Keyword: %s, value: %d\n", keywords[i], hash(keywords[i]));
  }
}
*/
