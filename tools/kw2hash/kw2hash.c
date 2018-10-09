#include <stdio.h>

/** @brief A maximum value for the hash function to use.
 *
 * @def HASH_MAX Generate hash values between 0 and 7919.
 */
#define HASH_MAX 7919

/** @brief Generate a hash value for the given string.
 *
 * Uses the universal string hash from (Algorithms in C)[https://www.amazon.com/Algorithms-Parts-1-4-Fundamentals-Structures/dp/0201314525].
 * Generates a hash value between 0 and HASH_MAX for the given word.
 *
 * @param word A string to generate a hash value for.
 * @return The generated hash value.
 */
static int hash(const char *word)
{
  int h, a = 31415, b = 27183;
  for(h = 0; *word != '\0'; word++, a = a * b % (HASH_MAX - 1))
    h = (a * h + *word) % HASH_MAX;

  return h;
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Usage: kw2hash keyword");
    return 1;
  }

  int result = hash(argv[1]);

  printf("The hash value for %s is %d\n", argv[1], result);

  return 0;
}
