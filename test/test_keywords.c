#include "unity.h"
#include "keywords.h"
#include "token.h"

// See the note in 'src/keywords.c' about adding new keywords.
/*
void test_show_hashes(void)
{
  show_keyword_hashes();
}
*/

void test_find_keyword_with_match(void)
{
  TEST_ASSERT_EQUAL_INT(TOKEN_BEGIN,  find_keyword("begin"));
  TEST_ASSERT_EQUAL_INT(TOKEN_BREAK,  find_keyword("break"));
  TEST_ASSERT_EQUAL_INT(TOKEN_CASE,   find_keyword("case"));
  TEST_ASSERT_EQUAL_INT(TOKEN_CLASS,  find_keyword("class"));
  TEST_ASSERT_EQUAL_INT(TOKEN_DEF,    find_keyword("def"));
  TEST_ASSERT_EQUAL_INT(TOKEN_DO,     find_keyword("do"));
  TEST_ASSERT_EQUAL_INT(TOKEN_ELSE,   find_keyword("else"));
  TEST_ASSERT_EQUAL_INT(TOKEN_END,    find_keyword("end"));
  TEST_ASSERT_EQUAL_INT(TOKEN_ENSURE, find_keyword("ensure"));
  TEST_ASSERT_EQUAL_INT(TOKEN_FALSE,  find_keyword("false"));
  TEST_ASSERT_EQUAL_INT(TOKEN_IF,     find_keyword("if"));
  TEST_ASSERT_EQUAL_INT(TOKEN_IMPORT, find_keyword("import"));
  TEST_ASSERT_EQUAL_INT(TOKEN_NEXT,   find_keyword("next"));
  TEST_ASSERT_EQUAL_INT(TOKEN_NIL,    find_keyword("nil"));
  TEST_ASSERT_EQUAL_INT(TOKEN_RESCUE, find_keyword("rescue"));
  TEST_ASSERT_EQUAL_INT(TOKEN_RETURN, find_keyword("return"));
  TEST_ASSERT_EQUAL_INT(TOKEN_SUPER,  find_keyword("super"));
  TEST_ASSERT_EQUAL_INT(TOKEN_SWITCH, find_keyword("switch"));
  TEST_ASSERT_EQUAL_INT(TOKEN_THIS,   find_keyword("this"));
  TEST_ASSERT_EQUAL_INT(TOKEN_TRUE,   find_keyword("true"));
  TEST_ASSERT_EQUAL_INT(TOKEN_UNLESS, find_keyword("unless"));
  TEST_ASSERT_EQUAL_INT(TOKEN_UNTIL,  find_keyword("until"));
  TEST_ASSERT_EQUAL_INT(TOKEN_WHILE,  find_keyword("while"));
}

void test_find_keyword_without_match(void)
{
  TEST_ASSERT_FALSE(find_keyword("Begin"));
  TEST_ASSERT_FALSE(find_keyword("breakk"));
  TEST_ASSERT_FALSE(find_keyword("cAse"));
  TEST_ASSERT_FALSE(find_keyword("klass"));
  TEST_ASSERT_FALSE(find_keyword("deef"));
  TEST_ASSERT_FALSE(find_keyword("Do"));
  TEST_ASSERT_FALSE(find_keyword("elsE"));
  TEST_ASSERT_FALSE(find_keyword("ensu"));
  TEST_ASSERT_FALSE(find_keyword("flase"));
  TEST_ASSERT_FALSE(find_keyword("fi"));
  TEST_ASSERT_FALSE(find_keyword("inport"));
  TEST_ASSERT_FALSE(find_keyword("netx"));
  TEST_ASSERT_FALSE(find_keyword("null"));
  TEST_ASSERT_FALSE(find_keyword("reque"));
  TEST_ASSERT_FALSE(find_keyword("retrun"));
  TEST_ASSERT_FALSE(find_keyword("spuer"));
  TEST_ASSERT_FALSE(find_keyword("swtch"));
  TEST_ASSERT_FALSE(find_keyword("thiss"));
  TEST_ASSERT_FALSE(find_keyword("ture"));
  TEST_ASSERT_FALSE(find_keyword("unles"));
  TEST_ASSERT_FALSE(find_keyword("unlit"));
  TEST_ASSERT_FALSE(find_keyword("whlie"));
}

void test_find_keyword_with_unterminated_string(void)
{
  char string[] = { 'u','n','l','e','s','s' };
  TEST_ASSERT_FALSE(find_keyword(string));
}

void test_find_keyword_with_null(void)
{
  TEST_ASSERT_FALSE(find_keyword(NULL));
}
