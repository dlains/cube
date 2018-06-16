#include "unity.h"
#include "keywords.h"

void test_hash(void)
{
  int this = hash("this");
  int that = hash("that");
  int other_this = hash("this");

  TEST_ASSERT_FALSE(this == that);
  TEST_ASSERT_TRUE(this == other_this);
}
