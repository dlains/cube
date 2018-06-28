#include "common.h"
#include "compiler.h"
#include "scanner.h"

void compile(const char *source)
{
  init_scanner(source);

  int line = -1;
  for(;;)
  {
    Token token = next_token();
    if(token.line != line)
    {
      printf("%4d ", token.line);
      line = token.line;
    }
    else
    {
      printf("   | ");
    }
    printf("%2d '%s'\n", token.type, token.lexeme);

    if(token.type == TOKEN_EOF)
      break;
  }
}
