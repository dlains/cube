#include "unity.h"
#include "keywords.h"
#include "memory.h"
#include "error.h"
#include "scanner.h"

int nextTokenTypeFor(const char *source)
{
  initScanner(source);
  Token token = nextToken();
  return token.type;
}

void test_empty_source_returns_eof(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_EOF, nextTokenTypeFor(""));
}

void test_recognize_lparen(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_PAREN, nextTokenTypeFor("("));
}

void test_recognize_rparen(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_PAREN, nextTokenTypeFor(")"));
}

void test_recognize_lbrace(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_BRACE, nextTokenTypeFor("{"));
}

void test_recognize_rbrace(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_BRACE, nextTokenTypeFor("}"));
}

void test_recognize_lbracket(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_BRACKET, nextTokenTypeFor("["));
}

void test_recognize_rbracket(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_BRACKET, nextTokenTypeFor("]"));
}

void test_recognize_percent(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_PERCENT, nextTokenTypeFor("%"));
}

void test_recognize_comma(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_COMMA, nextTokenTypeFor(","));
}

void test_recognize_caret(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_CARET, nextTokenTypeFor("^"));
}

void test_recognize_dot(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_DOT, nextTokenTypeFor("."));
}

void test_recognize_minus(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, nextTokenTypeFor("-"));
}

void test_recognize_plus(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_PLUS, nextTokenTypeFor("+"));
}

void test_recognize_semicolon(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_SEMICOLON, nextTokenTypeFor(";"));
}

void test_recognize_slash(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_SLASH, nextTokenTypeFor("/"));
}

void test_recognize_and(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_AND, nextTokenTypeFor("&"));
}

void test_recognize_or(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_OR, nextTokenTypeFor("|"));
}

void test_recognize_star(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_STAR, nextTokenTypeFor("*"));
}

void test_recognize_power(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_POWER, nextTokenTypeFor("**"));
}

void test_recognize_bang(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_BANG, nextTokenTypeFor("!"));
}

void test_recognize_bang_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_BANG_EQUAL, nextTokenTypeFor("!="));
}

void test_recognize_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL, nextTokenTypeFor("="));
}

void test_recognize_equal_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_EQUAL, nextTokenTypeFor("=="));
}

void test_recognize_greater(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_GREATER, nextTokenTypeFor(">"));
}

void test_recognize_greater_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_GREATER_EQUAL, nextTokenTypeFor(">="));
}

void test_recognize_less(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LESS, nextTokenTypeFor("<"));
}

void test_recognize_less_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LESS_EQUAL, nextTokenTypeFor("<="));
}

void test_recognize_string(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_STRING, nextTokenTypeFor("\"this is a string\""));
}

void test_recognize_keywords(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_BEGIN, nextTokenTypeFor("begin"));
  TEST_ASSERT_EQUAL_INT(TOKEN_BREAK, nextTokenTypeFor("break"));
  TEST_ASSERT_EQUAL_INT(TOKEN_CASE, nextTokenTypeFor("case"));
  TEST_ASSERT_EQUAL_INT(TOKEN_CLASS, nextTokenTypeFor("class"));
  TEST_ASSERT_EQUAL_INT(TOKEN_DEF, nextTokenTypeFor("def"));
  TEST_ASSERT_EQUAL_INT(TOKEN_DO, nextTokenTypeFor("do"));
  TEST_ASSERT_EQUAL_INT(TOKEN_ELSE, nextTokenTypeFor("else"));
  TEST_ASSERT_EQUAL_INT(TOKEN_END, nextTokenTypeFor("end"));
  TEST_ASSERT_EQUAL_INT(TOKEN_ENSURE, nextTokenTypeFor("ensure"));
  TEST_ASSERT_EQUAL_INT(TOKEN_FALSE, nextTokenTypeFor("false"));
  TEST_ASSERT_EQUAL_INT(TOKEN_IF, nextTokenTypeFor("if"));
  TEST_ASSERT_EQUAL_INT(TOKEN_IMPORT, nextTokenTypeFor("import"));
  TEST_ASSERT_EQUAL_INT(TOKEN_NEXT, nextTokenTypeFor("next"));
  TEST_ASSERT_EQUAL_INT(TOKEN_NIL, nextTokenTypeFor("nil"));
  TEST_ASSERT_EQUAL_INT(TOKEN_RESCUE, nextTokenTypeFor("rescue"));
  TEST_ASSERT_EQUAL_INT(TOKEN_RETURN, nextTokenTypeFor("return"));
  TEST_ASSERT_EQUAL_INT(TOKEN_SUPER, nextTokenTypeFor("super"));
  TEST_ASSERT_EQUAL_INT(TOKEN_SWITCH, nextTokenTypeFor("switch"));
  TEST_ASSERT_EQUAL_INT(TOKEN_THIS, nextTokenTypeFor("this"));
  TEST_ASSERT_EQUAL_INT(TOKEN_TRUE, nextTokenTypeFor("true"));
  TEST_ASSERT_EQUAL_INT(TOKEN_UNLESS, nextTokenTypeFor("unless"));
  TEST_ASSERT_EQUAL_INT(TOKEN_UNTIL, nextTokenTypeFor("until"));
  TEST_ASSERT_EQUAL_INT(TOKEN_WHILE, nextTokenTypeFor("while"));
}

void test_recognize_identifier(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_IDENTIFIER, nextTokenTypeFor("someName"));
}

void test_recognize_numbers(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_NUMBER, nextTokenTypeFor("12.3"));
}

void test_recognize_multiple_tokens(void)
{
  initScanner("def method()");
  Token token = nextToken();
  TEST_ASSERT_EQUAL_INT(TOKEN_DEF, token.type);
  token = nextToken();
  TEST_ASSERT_EQUAL_INT(TOKEN_IDENTIFIER, token.type);
  token = nextToken();
  TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_PAREN, token.type);
  token = nextToken();
  TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_PAREN, token.type);
  token = nextToken();
  TEST_ASSERT_EQUAL_INT(TOKEN_EOF, token.type);
}
