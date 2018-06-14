#include "unity.h"
#include "scanner.h"

void test_empty_source_returns_eof(void)
{
  initScanner("");
  Token token = scanToken();
  TEST_ASSERT_EQUAL_INT(TOKEN_EOF, token.type);
}

void test_recognize_lparen(void)
{
  initScanner("(");
  Token token = scanToken();
  TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_PAREN, token.type);
}
