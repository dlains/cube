#include "keywords.h"
#include "scanner.h"

// The keywords used in Cube.
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

// A hash value for each of the keywords. The index matches the
// index in 'keywords'.
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

// Seems like there should be a better way to do this. I could assign the hash values
// as the token type values in 'scanner.h' then have is_keyword return the hash value
// which will already be the correct token type id. Is that too tricky? Thinking about
// this some more.
int keyword_type(const char *word)
{
  int value = hash(word);
  switch(value)
  {
    case 2504:
      return TOKEN_BEGIN;
    case 3748:
      return TOKEN_BREAK;
    case 3481:
      return TOKEN_CASE;
    case 1018:
      return TOKEN_CLASS;
    case 4235:
      return TOKEN_DEF;
    case 2081:
      return TOKEN_DO;
    case 1128:
      return TOKEN_ELSE;
    case 7082:
      return TOKEN_END;
    case 7205:
      return TOKEN_ENSURE;
    case 2595:
      return TOKEN_FALSE;
    case 6130:
      return TOKEN_IF;
    case 3869:
      return TOKEN_IMPORT;
    case 1643:
      return TOKEN_NEXT;
    case 4493:
      return TOKEN_NIL;
    case 7137:
      return TOKEN_RESCUE;
    case 2618:
      return TOKEN_RETURN;
    case 4786:
      return TOKEN_SUPER;
    case 5077:
      return TOKEN_SWITCH;
    case 5937:
      return TOKEN_THIS;
    case 749:
      return TOKEN_TRUE;
    case 7846:
      return TOKEN_UNLESS;
    case 1255:
      return TOKEN_UNTIL;
    case 6530:
      return TOKEN_WHILE;
    default:
      return TOKEN_ERROR;
  }
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
