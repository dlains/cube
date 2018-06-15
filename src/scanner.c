#include "common.h"
#include "scanner.h"

typedef struct
{
  const char *start;
  const char *current;
  int line;
} Scanner;

Scanner scanner;

void initScanner(const char *source)
{
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

Token nextToken()
{
  skipWhitespace();

  scanner.start = scanner.current;

  if(isAtEnd())
    return makeToken(TOKEN_EOF);

  char c = advance();

  if(isAlpha(c))
    return identifier();
  if(isDigit(c))
    return number();

  switch(c)
  {
    case '(':
      return makeToken(TOKEN_LEFT_PAREN);
    case ')':
      return makeToken(TOKEN_RIGHT_PAREN);
    case '{':
      return makeToken(TOKEN_LEFT_BRACE);
    case '}':
      return makeToken(TOKEN_RIGHT_BRACE);
    case '[':
      return makeToken(TOKEN_LEFT_BRACKET);
    case ']':
      return makeToken(TOKEN_RIGHT_BRACKET);
    case '%':
      return makeToken(TOKEN_PERCENT);
    case ',':
      return makeToken(TOKEN_COMMA);
    case '^':
      return makeToken(TOKEN_CARET);
    case '.':
      return makeToken(TOKEN_DOT);
    case '-':
      return makeToken(TOKEN_MINUS);
    case '+':
      return makeToken(TOKEN_PLUS);
    case ';':
      return makeToken(TOKEN_SEMICOLON);
    case '/':
      return makeToken(TOKEN_SLASH);
    case '&':
      return makeToken(TOKEN_AND);
    case '|':
      return makeToken(TOKEN_OR);
    case '*':
      return makeToken(match('*') ? TOKEN_POWER : TOKEN_STAR);
    case '!':
      return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '>':
      return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '<':
      return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '"':
      return string();
  }

  return errorToken("Unexpected character.");
}

static bool isAtEnd()
{
  return *scanner.current == '\0';
}

static Token makeToken(TokenType type)
{
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;

  return token;
}

static Token errorToken(const char *message)
{
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;

  return token;
}

static char advance()
{
  scanner.current++;
  return scanner.current[-1];
}

static bool match(char expected)
{
  if(isAtEnd())
    return false;

  if(*scanner.current != expected)
    return false;

  scanner.current++;
  return true;
}

static char peek()
{
  return *scanner.current;
}

static char peekNext()
{
  if(isAtEnd())
    return '\0';
  return scanner.current[1];
}

static void skipWhitespace()
{
  for(;;)
  {
    char c = peek();
    switch(c)
    {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\n':
        scanner.line++;
        advance();
        break;
      case '#':
        while(peek() != '\n' && !isAtEnd())
          advance();
        break;
      default:
        return;
    }
  }
}

static Token string()
{
  while(peek() != '"' && !isAtEnd())
  {
    if(peek() == '\n')
      scanner.line++;
    advance();
  }

  if(isAtEnd())
    return errorToken("Unterminated string.");

  // Advance past the closing quote.
  advance();
  return makeToken(TOKEN_STRING);
}

static Token number()
{
  while(isDigit(peek()))
    advance();

  // Look for a fractional part.
  if(peek() == '.' && isDigit(peekNext()))
  {
    advance();
    while(isDigit(peek()))
      advance();
  }

  return makeToken(TOKEN_NUMBER);
}

static Token identifier()
{
  while(isAlpha(peek()) || isDigit(peek()))
    advance();

  return makeToken(identifierType());
}

static TokenType identifierType()
{
  switch(scanner.start[0])
  {
    case 'b':
      if(scanner.current - scanner.start > 1)
      {
        switch(scanner.start[1])
        {
          case 'e':
            return checkKeyword(2, 3, "gin", TOKEN_BEGIN);
          case 'r':
            return checkKeyword(2, 3, "eak", TOKEN_BREAK);
        }
      }
      break;
    case 'c':
      if(scanner.current - scanner.start > 1)
      {
        switch(scanner.start[1])
        {
          case 'a':
            return checkKeyword(2, 2, "se", TOKEN_CASE);
          case 'l':
            return checkKeyword(2, 3, "ass", TOKEN_CLASS);
        }
      }
      break;
    case 'd':
      if(scanner.current - scanner.start > 1)
      {
        switch(scanner.start[1])
        {
          case 'e':
            return checkKeyword(2, 1, "f", TOKEN_DEF);
          case 'o':
            return checkKeyword(2, 0, "", TOKEN_DO);
        }
      }
      break;
    case 'e':
      if(scanner.current - scanner.start > 1)
      {
        switch(scanner.start[1])
        {
          case 'l':
            return checkKeyword(2, 2, "se", TOKEN_ELSE);
          case 'n':
            return checkKeyword(2, 1, "d", TOKEN_END);
        }
      }
      break;
    case 'f':
      return checkKeyword(1, 4, "alse", TOKEN_FALSE);
      break;
    case 'i':
      if(scanner.current - scanner.start > 1)
      {
        switch(scanner.start[1])
        {
          case 'f':
            return checkKeyword(2, 0, "", TOKEN_IF);
          case 'm':
            return checkKeyword(2, 4, "port", TOKEN_IMPORT);
        }
      }
      break;
    case 'n':
      if(scanner.current - scanner.start > 1)
      {
        switch(scanner.start[1])
        {
          case 'e':
            return checkKeyword(2, 2, "xt", TOKEN_NEXT);
          case 'i':
            return checkKeyword(2, 1, "l", TOKEN_NIL);
        }
      }
      break;
    case 'r':
      return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's':
      if(scanner.current - scanner.start > 1)
      {
        switch(scanner.start[1])
        {
          case 'u':
            return checkKeyword(2, 3, "per", TOKEN_SUPER);
          case 'w':
            return checkKeyword(2, 4, "itch", TOKEN_SWITCH);
        }
      }
      break;
    case 't':
      if(scanner.current - scanner.start > 1)
      {
        switch(scanner.start[1])
        {
          case 'h':
            return checkKeyword(2, 2, "is", TOKEN_THIS);
          case 'r':
            return checkKeyword(2, 2, "ue", TOKEN_TRUE);
        }
      }
      break;
    case 'u':
      return checkKeyword(1, 5, "nless", TOKEN_UNLESS);
    case 'w':
      return checkKeyword(1, 4, "hile", TOKEN_WHILE);
  }

  return TOKEN_IDENTIFIER;
}

static TokenType checkKeyword(int start, int length, const char *rest, TokenType type)
{
  if(scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0)
    return type;

  return TOKEN_IDENTIFIER;
}

static bool isAlpha(char c)
{
  return(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c)
{
  return c >= '0' && c <= '9';
}