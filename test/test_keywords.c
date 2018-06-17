#include "unity.h"
#include "keywords.h"

// See the note in 'src/keywords.c' about adding new keywords.
/*
void test_show_hashes(void)
{
  show_keyword_hashes();
}
*/

void test_is_keyword_with_match(void)
{
  TEST_ASSERT_TRUE(is_keyword("begin"));
  TEST_ASSERT_TRUE(is_keyword("break"));
  TEST_ASSERT_TRUE(is_keyword("case"));
  TEST_ASSERT_TRUE(is_keyword("class"));
  TEST_ASSERT_TRUE(is_keyword("def"));
  TEST_ASSERT_TRUE(is_keyword("do"));
  TEST_ASSERT_TRUE(is_keyword("else"));
  TEST_ASSERT_TRUE(is_keyword("end"));
  TEST_ASSERT_TRUE(is_keyword("ensure"));
  TEST_ASSERT_TRUE(is_keyword("false"));
  TEST_ASSERT_TRUE(is_keyword("if"));
  TEST_ASSERT_TRUE(is_keyword("import"));
  TEST_ASSERT_TRUE(is_keyword("next"));
  TEST_ASSERT_TRUE(is_keyword("nil"));
  TEST_ASSERT_TRUE(is_keyword("rescue"));
  TEST_ASSERT_TRUE(is_keyword("return"));
  TEST_ASSERT_TRUE(is_keyword("super"));
  TEST_ASSERT_TRUE(is_keyword("switch"));
  TEST_ASSERT_TRUE(is_keyword("this"));
  TEST_ASSERT_TRUE(is_keyword("true"));
  TEST_ASSERT_TRUE(is_keyword("unless"));
  TEST_ASSERT_TRUE(is_keyword("until"));
  TEST_ASSERT_TRUE(is_keyword("while"));
}

void test_is_keyword_without_match(void)
{
  TEST_ASSERT_FALSE(is_keyword("Begin"));
  TEST_ASSERT_FALSE(is_keyword("breakk"));
  TEST_ASSERT_FALSE(is_keyword("cAse"));
  TEST_ASSERT_FALSE(is_keyword("klass"));
  TEST_ASSERT_FALSE(is_keyword("deef"));
  TEST_ASSERT_FALSE(is_keyword("Do"));
  TEST_ASSERT_FALSE(is_keyword("elsE"));
  TEST_ASSERT_FALSE(is_keyword("ensu"));
  TEST_ASSERT_FALSE(is_keyword("flase"));
  TEST_ASSERT_FALSE(is_keyword("fi"));
  TEST_ASSERT_FALSE(is_keyword("inport"));
  TEST_ASSERT_FALSE(is_keyword("netx"));
  TEST_ASSERT_FALSE(is_keyword("null"));
  TEST_ASSERT_FALSE(is_keyword("reque"));
  TEST_ASSERT_FALSE(is_keyword("retrun"));
  TEST_ASSERT_FALSE(is_keyword("spuer"));
  TEST_ASSERT_FALSE(is_keyword("swtch"));
  TEST_ASSERT_FALSE(is_keyword("thiss"));
  TEST_ASSERT_FALSE(is_keyword("ture"));
  TEST_ASSERT_FALSE(is_keyword("unles"));
  TEST_ASSERT_FALSE(is_keyword("unlit"));
  TEST_ASSERT_FALSE(is_keyword("whlie"));
}
