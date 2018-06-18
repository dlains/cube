#include "keywords.h"
#include "token.h"

typedef struct
{
  int token_type;
  int hash;
  char *word;
} Keyword;

// The keywords used in Cube.
Keyword keywords[] = {
  { TOKEN_BEGIN,  2504, "begin"  },
  { TOKEN_BREAK,  3748, "break"  },
  { TOKEN_CASE,   3481, "case"   },
  { TOKEN_CLASS,  1018, "class"  },
  { TOKEN_DEF,    4235, "def"    },
  { TOKEN_DO,     2081, "do"     },
  { TOKEN_ELSE,   1128, "else"   },
  { TOKEN_END,    7082, "end"    },
  { TOKEN_ENSURE, 7205, "ensure" },
  { TOKEN_FALSE,  2595, "false"  },
  { TOKEN_IF,     6130, "if"     },
  { TOKEN_IMPORT, 3869, "import" },
  { TOKEN_NEXT,   1643, "next"   },
  { TOKEN_NIL,    4493, "nil"    },
  { TOKEN_RESCUE, 7137, "rescue" },
  { TOKEN_RETURN, 2618, "return" },
  { TOKEN_SUPER,  4786, "super"  },
  { TOKEN_SWITCH, 5077, "switch" },
  { TOKEN_THIS,   5937, "this"   },
  { TOKEN_TRUE,   749,  "true"   },
  { TOKEN_UNLESS, 7846, "unless" },
  { TOKEN_UNTIL,  1255, "until"  },
  { TOKEN_WHILE,  6530, "while"  },
};

int find_keyword(const char *word)
{
  // Can't be a keyword if it is NULL.
  if(word == NULL)
    return 0;

  // Can't be a keyword if it is 0 characters or greather than 6 characters.
  int length = strnlen(word, LEXEME_LEN);
  if(length < 1 || length > 6)
    return 0;

  int value = hash(word);
  for(int i = 0; i < NUM_KEYWORDS; i++)
  {
    if(keywords[i].hash == value)
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

static int verify_keyword(const char *word, int index)
{
  if(memcmp(word, keywords[index].word, strnlen(keywords[index].word, LEXEME_LEN)) == 0)
    return keywords[index].token_type;
  return 0;
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
