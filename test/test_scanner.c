#include "unity.h"
#include "keywords.h"
#include "memory.h"
#include "error.h"
#include "scanner.h"

int next_token_type_for(const char *source)
{
  init_scanner(source);
  Token token = next_token();
  return token.type;
}

void test_empty_source_returns_eof(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_EOF, next_token_type_for(""));
}

void test_recognize_lparen(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_PAREN, next_token_type_for("("));
}

void test_recognize_rparen(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_PAREN, next_token_type_for(")"));
}

void test_recognize_lbrace(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_BRACE, next_token_type_for("{"));
}

void test_recognize_rbrace(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_BRACE, next_token_type_for("}"));
}

void test_recognize_lbracket(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_BRACKET, next_token_type_for("["));
}

void test_recognize_rbracket(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_BRACKET, next_token_type_for("]"));
}

void test_recognize_percent(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_PERCENT, next_token_type_for("%"));
}

void test_recognize_comma(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_COMMA, next_token_type_for(","));
}

void test_recognize_caret(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_CARET, next_token_type_for("^"));
}

void test_recognize_dot(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_DOT, next_token_type_for("."));
}

void test_recognize_minus(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, next_token_type_for("-"));
}

void test_recognize_plus(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_PLUS, next_token_type_for("+"));
}

void test_recognize_semicolon(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_SEMICOLON, next_token_type_for(";"));
}

void test_recognize_slash(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_SLASH, next_token_type_for("/"));
}

void test_recognize_and(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_AND, next_token_type_for("&"));
}

void test_recognize_or(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_OR, next_token_type_for("|"));
}

void test_recognize_star(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_STAR, next_token_type_for("*"));
}

void test_recognize_power(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_POWER, next_token_type_for("**"));
}

void test_recognize_bang(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_BANG, next_token_type_for("!"));
}

void test_recognize_bang_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_BANG_EQUAL, next_token_type_for("!="));
}

void test_recognize_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL, next_token_type_for("="));
}

void test_recognize_equal_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_EQUAL, next_token_type_for("=="));
}

void test_recognize_greater(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_GREATER, next_token_type_for(">"));
}

void test_recognize_greater_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_GREATER_EQUAL, next_token_type_for(">="));
}

void test_recognize_less(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LESS, next_token_type_for("<"));
}

void test_recognize_less_equal(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_LESS_EQUAL, next_token_type_for("<="));
}

void test_recognize_string(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_STRING, next_token_type_for("\"this is a string\""));
}

void test_recognize_keywords(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_BEGIN, next_token_type_for("begin"));
  TEST_ASSERT_EQUAL_INT(TOKEN_BREAK, next_token_type_for("break"));
  TEST_ASSERT_EQUAL_INT(TOKEN_CASE, next_token_type_for("case"));
  TEST_ASSERT_EQUAL_INT(TOKEN_CLASS, next_token_type_for("class"));
  TEST_ASSERT_EQUAL_INT(TOKEN_DEF, next_token_type_for("def"));
  TEST_ASSERT_EQUAL_INT(TOKEN_DO, next_token_type_for("do"));
  TEST_ASSERT_EQUAL_INT(TOKEN_ELSE, next_token_type_for("else"));
  TEST_ASSERT_EQUAL_INT(TOKEN_END, next_token_type_for("end"));
  TEST_ASSERT_EQUAL_INT(TOKEN_ENSURE, next_token_type_for("ensure"));
  TEST_ASSERT_EQUAL_INT(TOKEN_FALSE, next_token_type_for("false"));
  TEST_ASSERT_EQUAL_INT(TOKEN_IF, next_token_type_for("if"));
  TEST_ASSERT_EQUAL_INT(TOKEN_IMPORT, next_token_type_for("import"));
  TEST_ASSERT_EQUAL_INT(TOKEN_NEXT, next_token_type_for("next"));
  TEST_ASSERT_EQUAL_INT(TOKEN_NIL, next_token_type_for("nil"));
  TEST_ASSERT_EQUAL_INT(TOKEN_RESCUE, next_token_type_for("rescue"));
  TEST_ASSERT_EQUAL_INT(TOKEN_RETURN, next_token_type_for("return"));
  TEST_ASSERT_EQUAL_INT(TOKEN_SUPER, next_token_type_for("super"));
  TEST_ASSERT_EQUAL_INT(TOKEN_SWITCH, next_token_type_for("switch"));
  TEST_ASSERT_EQUAL_INT(TOKEN_THIS, next_token_type_for("this"));
  TEST_ASSERT_EQUAL_INT(TOKEN_TRUE, next_token_type_for("true"));
  TEST_ASSERT_EQUAL_INT(TOKEN_UNLESS, next_token_type_for("unless"));
  TEST_ASSERT_EQUAL_INT(TOKEN_UNTIL, next_token_type_for("until"));
  TEST_ASSERT_EQUAL_INT(TOKEN_WHILE, next_token_type_for("while"));
}

void test_recognize_identifier(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_IDENTIFIER, next_token_type_for("someName"));
}

void test_recognize_numbers(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_NUMBER, next_token_type_for("12.3"));
}

void test_recognize_multiple_tokens(void)
{
  init_scanner("def method()");
  Token token = next_token();
  TEST_ASSERT_EQUAL_INT(TOKEN_DEF, token.type);
  token = next_token();
  TEST_ASSERT_EQUAL_INT(TOKEN_IDENTIFIER, token.type);
  token = next_token();
  TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_PAREN, token.type);
  token = next_token();
  TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_PAREN, token.type);
  token = next_token();
  TEST_ASSERT_EQUAL_INT(TOKEN_EOF, token.type);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_empty_source_returns_eof, 14);
  RUN_TEST(test_recognize_lparen, 19);
  RUN_TEST(test_recognize_rparen, 24);
  RUN_TEST(test_recognize_lbrace, 29);
  RUN_TEST(test_recognize_rbrace, 34);
  RUN_TEST(test_recognize_lbracket, 39);
  RUN_TEST(test_recognize_rbracket, 44);
  RUN_TEST(test_recognize_percent, 49);
  RUN_TEST(test_recognize_comma, 54);
  RUN_TEST(test_recognize_caret, 59);
  RUN_TEST(test_recognize_dot, 64);
  RUN_TEST(test_recognize_minus, 69);
  RUN_TEST(test_recognize_plus, 74);
  RUN_TEST(test_recognize_semicolon, 79);
  RUN_TEST(test_recognize_slash, 84);
  RUN_TEST(test_recognize_and, 89);
  RUN_TEST(test_recognize_or, 94);
  RUN_TEST(test_recognize_star, 99);
  RUN_TEST(test_recognize_power, 104);
  RUN_TEST(test_recognize_bang, 109);
  RUN_TEST(test_recognize_bang_equal, 114);
  RUN_TEST(test_recognize_equal, 119);
  RUN_TEST(test_recognize_equal_equal, 124);
  RUN_TEST(test_recognize_greater, 129);
  RUN_TEST(test_recognize_greater_equal, 134);
  RUN_TEST(test_recognize_less, 139);
  RUN_TEST(test_recognize_less_equal, 144);
  RUN_TEST(test_recognize_string, 149);
  RUN_TEST(test_recognize_keywords, 154);
  RUN_TEST(test_recognize_identifier, 181);
  RUN_TEST(test_recognize_numbers, 186);
  RUN_TEST(test_recognize_multiple_tokens, 191);
  UNITY_END();
}
